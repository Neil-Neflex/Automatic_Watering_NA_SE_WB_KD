#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>

#include "Case.h"
#include "SensorCase.h"


extern Case* listeData[8];
// listeData[0] = SensorCase objet Temperature
// listeData[1] = SensorCase objet Humidite
// listeData[2] = SensorCase objet Pluviometrie
// listeData[3] = SensorCase objet Humidite_sol

// Prototypes des fonctions pour annoncer leur existance
inline void setData();
inline void setTemperature(int x);
inline void setHumidite(int x);
inline void setPluvio(int x);
inline void setHumiditeGround(int x);

inline void TemperatureBgColor();
inline void HumiditeBgColor();
inline void PluvioBgColor();
inline void HumiditeGroundBgColor();

//Rôle : cast le pointeur sur listData[idx] pour les rendre utilisable
inline SensorCase* asSensor(int idx) {
  return static_cast<SensorCase*>(listeData[idx]);
}

//Rôle : Appel les procédures permetant la mise à jour des valeurs sensors
inline void setData(){
  setTemperature(35);
  setHumidite(47);
  setPluvio(0);
  setHumiditeGround(0);
}
//######### FONCTIONS MAJ VALUES ##############
/*
 A l'implémentation du code pour la récupération de la valeur du capteur : 
 1- supprimez l'argument de la fonction et de la signature 
 2- implémentez votre code en envoyant la valeur de type int dans la variable tmp
 3- remplacez la variable x par la variable tmp dans le code 
 ATTENTION : ne modifiez rien d'autre !!!
*/
inline void setTemperature(int x){
  int tmp = 32; //nouvelle valeur
  auto* sensor = asSensor(0); //pointeur vers la case température

  //... 
  if (sensor->getValue() != x){
    sensor->setValue(x);
    TemperatureBgColor();
  }
}

inline void setHumidite(int x){
  int tmp = 39; //nouvelle valeur
  auto* sensor = asSensor(1);

  if (sensor->getValue() != x){
    sensor->setValue(x);
    HumiditeBgColor();
  }
}

inline void setPluvio(int x){
  int tmp = 3; //nouvelle valeur
  auto* sensor = asSensor(2);

  if (sensor->getValue() != x){
    sensor->setValue(x);
    PluvioBgColor();
  }
}

inline void setHumiditeGround(int x){
  int tmp =0; //nouvelle valeur
  auto* sensor = asSensor(3);

  if (sensor->getValue() != x){
    sensor->setValue(x);
    HumiditeGroundBgColor();
  }

}

//########## FONCTIONS MAJ COLORS ##############

inline void TemperatureBgColor(){
  auto* sensor = asSensor(0);
  int val = sensor -> getValue();

  if (val <= 0){if (sensor -> getColor() != TFT_BLUE){sensor -> setColor(TFT_BLUE);}}
  else if (val > 25 && val < 30){if (sensor -> getColor() != TFT_ORANGE) {sensor -> setColor(TFT_ORANGE);}}
  else if (val >= 30){
    if (sensor -> getColor() != TFT_RED){sensor -> setColor(TFT_RED);}
    //sensor -> setLock(1);
    }
  else { sensor -> setColor(TFT_WHITE);}
}

inline void HumiditeBgColor(){
  auto* sensor = asSensor(1);
  int val = sensor -> getValue();

  if (val < 40){if (sensor -> getColor() != TFT_RED) {sensor -> setColor(TFT_RED);}}
  else if (val >= 60 && val<80){if (sensor -> getColor() != TFT_YELLOW) { sensor -> setColor(TFT_YELLOW);}}
  else if (val >= 80){
    if (sensor -> getColor() != TFT_RED){sensor -> setColor(TFT_RED);}
    //sensor -> setLock(1);
  }
  else { sensor -> setColor(TFT_WHITE);}
}

inline void PluvioBgColor(){
  auto* sensor = asSensor(2);
  int val = sensor -> getValue();

  if (val > 5 && val<10){if (sensor -> getColor() != TFT_CYAN){sensor -> setColor(TFT_CYAN);}}
  else if (val >= 10){
    if (sensor -> getColor() != TFT_BLUE){ sensor -> setColor(TFT_BLUE);}
    //sensor -> setLock(1);
  }
  else { sensor -> setColor(TFT_WHITE);}
}

inline void HumiditeGroundBgColor(){
  auto* sensor = asSensor(3);
  int val = sensor -> getValue();

  if (val < 40 ){if (sensor -> getColor() != TFT_RED){sensor -> setColor(TFT_RED);}}
  else if (val >= 60 && val<80){if (sensor -> getColor() != TFT_YELLOW){sensor -> setColor(TFT_YELLOW);}}
  else if (val >= 80){
    if (sensor -> getColor() != TFT_RED){sensor -> setColor(TFT_RED);}
    //sensor -> setLock(1);
  }
  else { sensor -> setColor(TFT_WHITE);}
}



