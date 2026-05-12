#include "SensorManagment.h"
#include <DHT.h>

extern DHT dht; // On utilise l'objet défini dans le .ino

// Rôle : Appel les procédures permettant la mise à jour des valeurs sensors
void setData() {
  static unsigned long chronoDHT = 0;
  
  // On lit la météo (Air et Sol) toutes les 5 secondes
  if (millis() - chronoDHT > 5000) { 
    chronoDHT = millis();
    
    // --- LECTURE DHT22 ---
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    Serial.print("DEBUG CAPTEUR -> T: ");
    Serial.print(t);
    Serial.print(" C | H: ");
    Serial.print(h);
    Serial.println(" %");

    if (!isnan(t) && !isnan(h)) {
      // Mise à jour des valeurs
      asSensor(0)->setValue((int)t);
      asSensor(1)->setValue((int)h);

      // Mise à jour des couleurs
      TemperatureBgColor();
      HumiditeBgColor();
      
      Serial.printf("Ecran mis a jour ! Temp: %d C | Hum: %d %%\n", (int)t, (int)h);
    } 
    else {
      Serial.println("ERREUR : Le DHT22 a renvoyé NaN (Impossible de lire) !");
    }

    // --- LECTURE HUMIDITÉ SOL ---
    setHumiditeGround();
  }
}

// ######### FONCTIONS MAJ VALUES ##############

void setPluvio(float x) {
  auto* sensor = asSensor(2);
  // Si la nouvelle quantité de pluie est différente
  if (sensor->getValue() != x) {
    sensor->setValue(x);
    PluvioBgColor(); 
  }
}

void setHumiditeGround() {
  int valeurBrute = analogRead(HUMIDITY_PIN);
  
  // Conversion en pourcentage
  int pourcentage = map(valeurBrute, AirValue, WaterValue, 0, 100);
  pourcentage = constrain(pourcentage, 0, 100);
  
  // Mise à jour de la valeur et de la couleur
  asSensor(3)->setValue((float)pourcentage);
  HumiditeGroundBgColor();
}

// ########## FONCTIONS MAJ COLORS ##############

void TemperatureBgColor(){
  auto* sensor = asSensor(0);
  int val = sensor->getValue();

  if (val <= 0) {
    if (sensor->getColor() != TFT_BLUE) sensor->setColor(TFT_BLUE);
  }
  else if (val > 25 && val < 30) {
    if (sensor->getColor() != TFT_ORANGE) sensor->setColor(TFT_ORANGE);
  }
  else if (val >= 30) {
    if (sensor->getColor() != TFT_RED) sensor->setColor(TFT_RED);
  }
  else { 
    sensor->setColor(TFT_WHITE);
  }
}

void HumiditeBgColor(){
  auto* sensor = asSensor(1);
  int val = sensor->getValue();

  if (val < 40) {
    if (sensor->getColor() != TFT_RED) sensor->setColor(TFT_RED);
  }
  else if (val >= 60 && val < 80) {
    if (sensor->getColor() != TFT_YELLOW) sensor->setColor(TFT_YELLOW);
  }
  else if (val >= 80) {
    if (sensor->getColor() != TFT_RED) sensor->setColor(TFT_RED); // Note: Peut-être TFT_BLUE serait plus logique ?
  }
  else { 
    sensor->setColor(TFT_WHITE);
  }
}

void PluvioBgColor(){
  auto* sensor = asSensor(2);
  float val = sensor->getValue(); // Modifié en float

  if (val > 5.0 && val < 10.0) {
    if (sensor->getColor() != TFT_CYAN) sensor->setColor(TFT_CYAN);
  }
  else if (val >= 10.0) {
    if (sensor->getColor() != TFT_BLUE) sensor->setColor(TFT_BLUE);
  }
  else { 
    sensor->setColor(TFT_WHITE);
  }
}

void HumiditeGroundBgColor(){
  auto* sensor = asSensor(3);
  int val = sensor->getValue();

  // Logique visuelle adaptée pour le sol
  if (val >= 80) {
    if (sensor->getColor() != TFT_BLUE) sensor->setColor(TFT_BLUE);       // Détrempé
  }
  else if (val >= 40) {
    if (sensor->getColor() != TFT_DARKGREEN) sensor->setColor(TFT_DARKGREEN); // Idéal
  }
  else if (val >= 20) {
    if (sensor->getColor() != TFT_ORANGE) sensor->setColor(TFT_ORANGE);     // Sec
  }
  else { 
    if (sensor->getColor() != TFT_RED) sensor->setColor(TFT_RED);         // Alerte sécheresse
  }
}