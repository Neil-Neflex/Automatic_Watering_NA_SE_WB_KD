#include <SPI.h>
#include <TFT_eSPI.h>
#include <Arduino.h>
#include <DHT.h>
#include "images.h"           
#include "Case.h"
#include "SensorCase.h"       
#include "VanneCase.h"        
#include "SensorManagment.h"  
#include "Interface.h"        
#include "ModuleHorloge.h"    
#include "Gestion_ULP.h"     

#define MM_PAR_BASCULEMENT 0.2794
RTC_DATA_ATTR float pluie_totale_mm = 0.0;

#define SCREEN_LED 13
#define DHTPIN 14 
#define DHTTYPE DHT22 

TFT_eSPI tft = TFT_eSPI();
RTC_DS3231 rtc;
PNG png;
DHT dht(DHTPIN, DHTTYPE);

#define FSS9  &FreeSans9pt7b
#define FSS12 &FreeSans12pt7b

int16_t pngX, pngY; 
EtatEcran ETE;      
#define DELAY_ONESEC 1000

// Planning des vannes
const int HEURES_DEPART[4]  = {20, 20, 21, 21}; 
const int MINUTES_DEPART[4] = {00, 30, 00,  30}; 
uint32_t finArrosage[4] = {0, 0, 0, 0};

// Objets d'affichage
VanneCase V1(0, 0, 140, 80, 100, "V1", 2, 15, 16, 17);
VanneCase V2(1, 80, 140, 80, 100, "V2", 3, 10, 25, 26);
VanneCase V3(2, 160, 140, 80, 100, "V3", 3, 60, 27, 14);
VanneCase V4(3, 240, 140, 80, 100, "V4", 3, 10, 32, 33);

SensorCase TEMPERATURE(0, 23, TFT_ORANGE, 0, 40, 80, 100, temperature, sizeof(temperature), 5, 45, " C");
SensorCase HUMIDITE(1, 0, TFT_YELLOW, 80, 40, 80, 100, humidite, sizeof(humidite), 95, 45, "%");
SensorCase PLUVIOMET(2, 0, TFT_WHITE, 160, 40, 80, 100, pluie, sizeof(pluie), 165, 45, "mm");
SensorCase HUMIDITE_SOL(3, 0, TFT_WHITE, 240, 40, 80, 100, terre, sizeof(terre), 245, 45, " %");

Case* listeData[8] = {&TEMPERATURE, &HUMIDITE, &PLUVIOMET, &HUMIDITE_SOL, &V1, &V2, &V3, &V4};

// ---------- Logique métier ----------
void gererVannesAutomatiques(DateTime now) {
  int tempActuelle = asSensor(0)->getValue();
  int humActuelle  = asSensor(1)->getValue();
  int humSolActuelle = asSensor(3)->getValue(); // Ajout de la lecture du sol

  for (int i = 0; i < 4; i++) {
    VanneCase* v = static_cast<VanneCase*>(listeData[i + 4]);
    
    if (now.hour() == HEURES_DEPART[i] && now.minute() == MINUTES_DEPART[i] && now.second() == 0) {
      int joursIntervalle = v->getFreq() / 24;
      if (joursIntervalle <= 0) joursIntervalle = 1; 
      long joursDepuis1970 = now.unixtime() / 86400L;
      
      if (joursDepuis1970 % joursIntervalle == 0) {
        Serial.printf("\n--ANALYSE MÉTÉO POUR VANNE %d--\n", i+1);
        bool arrosageAutorise = true;

        // 
        if (pluie_totale_mm >= 5.0) {
          Serial.println("ANNULATION : Pluie > 5 mm.");
          arrosageAutorise = false;
        } 
        else if (tempActuelle <= 4) {
          Serial.println("ANNULATION : Gel (<=4°C).");
          arrosageAutorise = false;
        } 
        else if (humSolActuelle >= 60) {
          Serial.println("ANNULATION : Sol déjà humide (>=60%).");
          arrosageAutorise = false;
        }
        
        // ---
        if (arrosageAutorise) {
          float dureeCalculee = v->getDuree();
          
          if (tempActuelle >= 30) {
            dureeCalculee *= 1.30;
            Serial.println("Forte chaleur : +30%");
          }
          if (humActuelle >= 80) {
            dureeCalculee *= 0.80;
            Serial.println("Air très humide : -20%");
          }
          if (humSolActuelle <= 20) {
            dureeCalculee *= 1.20;
            Serial.println("Sol très sec : +20%");
          }
          
          int dureeFinaleMin = (int)dureeCalculee;
          if (dureeFinaleMin < 1) dureeFinaleMin = 1;

          Serial.printf("DÉMARRAGE Vanne %d pour %d min\n", i+1, dureeFinaleMin);
          v->ouverture();
          finArrosage[i] = now.unixtime() + (dureeFinaleMin * 60);
        }
      }
    }

    if (v->getState() && now.unixtime() >= finArrosage[i]) {
      Serial.printf("FIN cycle : Fermeture Vanne %d\n", i+1);
      v->fermeture(); 
    }
  }
}

// ---------- Setup ----------
void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("\n--- DÉMARRAGE ---");

  dht.begin();
  
  // Récupération de l'ULP
  uint32_t impulsionsPendantSommeil = getPulseCount();
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  
  if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) {
      pluie_totale_mm += (impulsionsPendantSommeil * MM_PAR_BASCULEMENT);
      Serial.printf("Réveil Timer : +%d basculements ULP.\n", impulsionsPendantSommeil);
  } else {
      Serial.println("Premier démarrage : Init ULP");
      initULP();      
      pluie_totale_mm = 0.0;
  }

  //  Écran initialisé AVANT l'Horloge RTC
  SCREEN_INIT();
  if (wakeup_reason != ESP_SLEEP_WAKEUP_TIMER) {
    SCREEN_BEGIN();
  }
  
  //  Initialisation Horloge RTC
  CLK_INIT();   
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //  Interface graphique
  tft.fillRect(0, 0, 320, 40, TFT_LIGHTGREY);
  PLUVIOMET.setValue((int)pluie_totale_mm); 
 
  ETE = Principal;
  Update(); 

  delay(3000);
  setData();   
}

// ---------- Loop ----------
void loop() {
  Update();                     
  setData();                    

  // 
  static unsigned long tempsDerniereActivite = millis();
  bool arrosageEnCours = false;

  // Gestion des impulsions vannes
  for (int i = 4; i < 8; i++) {
    VanneCase* v = static_cast<VanneCase*>(listeData[i]);
    v->checkTimer();
    // Si une vanne est activée, on bloque la mise en veille !
    if (v->getState() == true) {
      arrosageEnCours = true;
      tempsDerniereActivite = millis(); // On reset le chrono de sommeil
    }
  }

  // Lecture ULP
  static unsigned long dernierCheckPluvio = 0;
  if (millis() - dernierCheckPluvio > 2000) {
    dernierCheckPluvio = millis();
    uint32_t nouvelles_impulsions = getPulseCount();
    
    if (nouvelles_impulsions > 0) {
      
      //
      if (nouvelles_impulsions > 5) {
        Serial.printf("⚠️ BRUIT ÉLECTRIQUE : %d impulsions ignorées !\n", nouvelles_impulsions);
        nouvelles_impulsions = 1; 
      }

      pluie_totale_mm += (nouvelles_impulsions * MM_PAR_BASCULEMENT);
      setPluvio(pluie_totale_mm);
      Serial.printf("Pluie : %.2f mm\n", pluie_totale_mm);
      tempsDerniereActivite = millis(); 
    }
  }

  // Planification arrosage 
  static unsigned long dernierCheckRTC = 0;
  static int derniereMinuteVerifiee = -1;
  if (millis() - dernierCheckRTC > 1000) {
    dernierCheckRTC = millis();
    DateTime now = rtc.now();
    gererVannesAutomatiques(now);
    
    if (now.hour() == 0 && now.minute() == 0 && now.minute() != derniereMinuteVerifiee) {
      derniereMinuteVerifiee = now.minute();
      pluie_totale_mm = 0.0;
      setPluvio(0);
      Serial.println("00:00 – Remise à zéro du compteur pluie");
      tempsDerniereActivite = millis(); // Activité détectée
    }
  }

  // --- LE GESTIONNAIRE DE DEEP SLEEP ---
  unsigned long tempsEcoule = millis() - tempsDerniereActivite;
  
  // Affichage du compte à rebours toutes les secondes
  static unsigned long dernierDebug = 0;
  if (millis() - dernierDebug > 1000) {
    dernierDebug = millis();
    
    //  (2 minutes)
    int secondesRestantes = (120000 - tempsEcoule) / 1000;
    
    if (arrosageEnCours) {
      Serial.println("SOMMEIL BLOQUÉ : Une vanne est ouverte !");
    } else if (secondesRestantes > 0) {
      Serial.printf("Mise en veille dans %d secondes...\n", secondesRestantes);
    }
  }

  // L'ESP32 s'endort SI ET SEULEMENT SI :
  //  Il s'est écoulé 120 secondes SANS activité
  // ET aucune vanne n'est en train d'arroser
  if (tempsEcoule > 120000 && arrosageEnCours == false) {
    Serial.println(">>> Entrée en deep sleep (5 min) <<<");
    digitalWrite(SCREEN_LED, LOW); // Éteindre l'écran
    delay(100); 
    esp_sleep_enable_timer_wakeup(5 * 60 * 1000000ULL);
    esp_deep_sleep_start();
  }
}

// ---------- Fonctions auxiliaires (inchangées) ----------
void SCREEN_INIT(){
  tft.init();
  pinMode(SCREEN_LED, OUTPUT);
  digitalWrite(SCREEN_LED, HIGH);
  tft.setRotation(1); 
}

void SCREEN_BEGIN(){
  tft.fillScreen(TFT_WHITE);
}

void VannesOff(){
  for (int i = 4; i<8; i++){
    static_cast<VanneCase*>(listeData[i])->disableUpdate();
  }
}

void VannesOn(){
  for (int i = 4; i<8; i++){
    static_cast<VanneCase*>(listeData[i])->enableUpdate();
  }
}

void OUVERTURE(VanneCase* V){
  for (int i = 4; i<8; i++){
    VanneCase* v = static_cast<VanneCase*>(listeData[i]);
    if (v->getState()) v->fermeture();
  }
  V->ouverture();
}

void FERMETURE(VanneCase* V){
  V->fermeture();
}

void Update(){
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= DELAY_ONESEC){
    previousMillis = currentMillis;
    GestionnaireEcran(ETE);
  }
}

void Routine(){
  Update();
  delay(2000);
  VannesOff();
  Update();
  delay(2000);
  VannesOn();
  V3.ouverture();
  Update();
  delay(2000);
  
  for (int i = 0; i<5; i++){
    setTemperature(); 
    setHumidite();    
    Update();
    delay(1500);
  }
  
  for (int i = 0; i<6; i++){
    setPluvio(i*2);
    setHumidite();
    Update();
    delay(2000);
  }
  setData();
}