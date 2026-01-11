
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <RTClib.h>
#include <PNGdec.h> // PNG Décoder - permet la décompression des fichier PNG et l'extraction RGBA

#include "images.h" //images a afficher
#include "Case.h"
#include "SensorCase.h"
#include "VanneCase.h"
#include "SensorManagment.h"

#define SCREEN_LED 13
TFT_eSPI tft = TFT_eSPI();
RTC_DS3231 rtc;
PNG png;

int16_t pngX, pngY; // Position courante d’affichage du PNG (utilisée par PngDraw)

#define DELAY_ONESEC 1000

//-------- Déclaration des bouttons ---------
#define LeftButton 39
#define CenterLeftButton 35
#define CenterRightButton 33
#define RightButton 26


//-------- Déclaration des différents objets d'affichage --------
//Vannes
VanneCase V1(
  0,            // numCase 
  0, 140, 80, 100,   // x, y, w, h
  "V1",         // nom
  2,            // fréquence (2 jours)
  15,            // durée (15 minutes)
  16,       //GPIO D16 pour l'ouverture de la vanne
  17        //GPIO D17 pour la fermeture de la vanne
);
VanneCase V2(1, 80, 140, 80, 100, "V2", 3, 10, 25, 26);
VanneCase V3(2, 160, 140, 80, 100, "V3", 3, 60, 27, 14);
VanneCase V4(3, 240, 140, 80, 100, "V3", 3, 10, 32, 33);

//Capteurs
SensorCase TEMPERATURE(
  0,                   // numCase
  23,                // value
  TFT_ORANGE,           // bgColor
  0, 40, 80, 100,      // x, y, w, h
  temperature,         // logo (tableau PROGMEM)
  sizeof(temperature), // logoSize
  5, 45,               // img_x, img_y
  " C"                 // unite
);
SensorCase HUMIDITE(1, 0, TFT_YELLOW, 80, 40, 80, 100, humidite, sizeof(humidite), 95, 45,"%");
SensorCase PLUVIOMET(1, 0, TFT_WHITE, 160, 40, 80, 100, pluie, sizeof(pluie), 165, 45, "mm");
SensorCase HUMIDITE_SOL(1, 0, TFT_WHITE, 240, 40, 80, 100, terre, sizeof(terre), 245, 45, " %");

//Liste de gestion de l'affichage  (pointeur vers case, qui est une classe abstraite)
Case* listeData[8] = {&TEMPERATURE, &HUMIDITE, &PLUVIOMET, &HUMIDITE_SOL, &V1, &V2, &V3, &V4};




void setup() {
  Serial.begin(9600);
  // Initialisation I2C
  Wire.begin(22, 21);   // broches ESP32 SDA=22, SCL=21
  Serial.println("Initialisation RTC...\n");
  //---------- Initialisation de la clock -----------------
  CLK_INIT();
  //------------Initialisation de l'écran----------------
  SCREEN_INIT();
  //-----------Initialisation boutons ---------------
  BUTTON_INIT();

  //---------- Ecran de démarage ----------------
  SCREEN_BEGIN();
 
  tft.fillRect(0, 0, 320, 40, TFT_LIGHTGREY); //pour que le bandeau de l'heure soit gris
  Update();
  delay(3000);
  setData();

}
//Boucle 
void loop() {
  Update();
  //Update_Vannes() -> fonction à créer pour gerer les ouvertures de vanne, voir fichier info deep sleep à la fin 
  //Tous les delays seront à modifier en millis (voir fonction update) ATTENTION reflexion derrière les actions en parallèle et les vérifications
  //Routine(); -> Ne plus appeler, ne marche plus car update mis à jour correctement 
}
//############### PROCEDURES & FONCTIONS #####################
//Setup - Procédure d'initialisation de l'horloge
void CLK_INIT(){
  
  while(!rtc.begin()) {
    Serial.println("RTC non détecté !");
  }
  // Si l'RTC a perdu son horloge → on met l'heure du compilateur
  if (rtc.lostPower()) {
    Serial.println("RTC a perdu l'heure, initialisation...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

//Setup - Procédure d'initialisation de l'écran
void SCREEN_INIT(){
  tft.init();
  pinMode(SCREEN_LED, OUTPUT);
  digitalWrite(SCREEN_LED, HIGH);
  tft.setRotation(1);  // portrait: 0 ou 2, paysage: 1 ou 3.
}

//Setup - Procédure de l'affichage de démarage
void SCREEN_BEGIN(){
  tft.fillScreen(TFT_WHITE); // on efface tout (fond blanc)
  tft.setFreeFont(&FreeSans12pt7b);  // police de caractère
  tft.setTextSize(1); //taille du text
  tft.setTextColor(TFT_BLACK, TFT_WHITE); //couleur du text

  tft.setCursor(20, 70); // position du début du message
  tft.println("Notre équipe : ");
  tft.setTextColor(TFT_RED, TFT_WHITE);
  tft.println("Neil A");
  tft.println("Khady D");
  tft.println("Salma E");
  tft.println("Wissal B");
  delay(1500);
  tft.fillScreen(TFT_WHITE);
}

//Setup - Initialise tous les boutons
void BUTTON_INIT(){
  pinMode(LeftButton, INPUT_PULLUP);
  pinMode(CenterLeftButton, INPUT_PULLUP);
  pinMode(CenterRightButton, INPUT_PULLUP);
  pinMode(RightButton, INPUT_PULLUP);

}

//Rôle : bloque toutes les vannes
void VannesOff(){
  //boucle pour parcourir la liste des cases (les 4 dernières)
  for (int i = 4; i<8;i++){
    //Cast de listeData[i] (type "Case" -> type "VanneCase")
    auto* vanne = static_cast<VanneCase*>(listeData[i]);
    //désactivation de la vanne
    vanne->disableUpdate();
    }
} 
//Rôle : débloque toutes les vannes
void VannesOn(){
  //boucle pour parcourir la liste des cases (les 4 dernières)
  for (int i = 4; i<8;i++){
    //Cast de listeData[i] (type "Case" -> type "VanneCase")
    auto* vanne = static_cast<VanneCase*>(listeData[i]);
    //activation de la vanne
    vanne->enableUpdate();
    }
}
//Rôle : Ouvre la vanne passée en argument en vérifiant que toutes les autres vannes sont fermées
void OUVERTURE(VanneCase* V){
  for (int i = 4; i<8;i++){
    //Cast de listeData[i] (type "Case" -> type "VanneCase")
    auto* vanne = static_cast<VanneCase*>(listeData[i]);
    //si vanne ouverte -> on ferme la vanne
    if (vanne->getState()){vanne->fermeture();}
  }
  V->ouverture();
}
//Rôle : Ferme la vanne passée en argument
void FERMETURE(VanneCase* V){
  V->fermeture();
}

//############### ROUTINES #####################
//Rôle : actualise l'affichage des parties de l'écran qui on changées
void Update(){
  static unsigned long previousMillis = 0; //variable de temps, début du compteur
  unsigned long currentMillis = millis(); //variable de temps, temps présent
  //si le délais écoulé depuis le début est supérieur à DELAY_ONESEC (1000ms)
  if (currentMillis - previousMillis >= DELAY_ONESEC){
    //mise à jour de previousMillis
    previousMillis = currentMillis;
    //parcours de toute les parties de l'affichage (toutes les cases)
    for (int i = 0; i<8; i++){
      //si un flag est à 1 -> on redessine la case
      if (listeData[i]->getFlag()){
        listeData[i]->draw(tft);
      }
    }
    //On actualise l'heure
    AfficherHeure();
  }
}
//A supprimer plus tard
void Routine(){
  //Simulation vérouillage et dévérouillage des vannes + ouverture vanne
  Update();
  delay(2000);
  VannesOff();
  Update();
  delay(2000);
  VannesOn();
  V3.ouverture();
  Update();
  delay(2000);
  //Simulation baisse de température
  for (int i = 0; i<5;i++){
    setTemperature(TEMPERATURE.getValue()-3);
    setHumidite(HUMIDITE.getValue()+5);
    Update();
    delay(1500);
  }
  //Simulation pluie
  for (int i = 0; i<6;i++){
    setPluvio(i*2);
    setHumidite(70+5*i);
    Update();
    delay(2000);
  }
  setData();
}

//############### PROCEDURES & FONCTIONS AUXILIAIRES #####################
//Rôle : Affiche l'heure et la date dans la partie supérieur de l'affichage
void AfficherHeure(){
  //MAJ param affichage (taille texte + couleurs)
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK, TFT_LIGHTGREY);

  DateTime now = rtc.now(); //Récupération de l'heure
  char TimeBuffer[64];    //Buffer pour récuperer la date à afficher
  sprintf(TimeBuffer,"%02d/%02d/%04d %02d:%02d:%02d", 
  now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());
  //on affiche temporairement la date dans la console série
  Serial.printf(TimeBuffer);
  tft.drawString(TimeBuffer, 5, 10, 2); // affichage Heure écran

}

//Rôle : fonction CallBack appelée par drawPng, permettant la gestion des image png avec composant Alpha (Transparence)
int pngDraw(PNGDRAW *pDraw) {
    static uint16_t lineBuf[320]; // ligne en RGB565
    static uint8_t mask[40];      // masque alpha (1 bit par pixel)

    // 1) Récupération de la ligne en RGB565
    png.getLineAsRGB565(pDraw, lineBuf, PNG_RGB565_BIG_ENDIAN, 0xFFFFFFFF);

    // 2) Récupération du masque alpha (détermine quels pixels sont visibles)
    bool hasOpaque = png.getAlphaMask(pDraw, mask, 255);

    if (!hasOpaque) {
        return 1; // Ligne totalement transparente → ne rien afficher
    }

    // 3) Écriture pixel par pixel selon le masque
    for (int x = 0; x < pDraw->iWidth; x++) {
        int byteIndex = x >> 3;       // 8 pixels par octet
        int bitIndex  = 7 - (x & 7);  // MSB first

        if (mask[byteIndex] & (1 << bitIndex)) {
            // pixel opaque → on dessine
            tft.drawPixel(pngX + x, pngY + pDraw->y, lineBuf[x]);
        }
    }

    return 1;
}

//Rôle : fonction utilitaire pour dessiner les image png
void drawPngFlash(const uint8_t *data, uint32_t len, int16_t x, int16_t y) {
    pngX = x;
    pngY = y;

    int rc = png.openFLASH((uint8_t*)data, len, pngDraw);
    if (rc == PNG_SUCCESS) {
        png.decode(NULL, 0);
        png.close();
    } else {
        Serial.print("Erreur PNG rc=");
        Serial.println(rc);
    }
}

