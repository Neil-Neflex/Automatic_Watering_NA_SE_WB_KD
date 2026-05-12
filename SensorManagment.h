//SensorManagment.h

#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>

#include "Case.h"
#include "SensorCase.h"
#define HUMIDITY_PIN 35
const int AirValue = 2500;    
const int WaterValue = 1150;

extern Case* listeData[8];
// listeData[0] = SensorCase objet Temperature
// listeData[1] = SensorCase objet Humidite
// listeData[2] = SensorCase objet Pluviometrie
// listeData[3] = SensorCase objet Humidite_sol

//Rôle : cast le pointeur sur listData[idx] pour les rendre utilisable
inline SensorCase* asSensor(int idx) {
  return static_cast<SensorCase*>(listeData[idx]);
}

void setData();

void setTemperature();
void setHumidite();
void setPluvio(float x);
void setHumiditeGround();

void TemperatureBgColor();
void HumiditeBgColor();
void PluvioBgColor();
void HumiditeGroundBgColor();



 