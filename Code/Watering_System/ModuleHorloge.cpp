/*#########################################
* @Project :   Watering System 
* @School :    Poytech Nice Sophia
* @Training :  ELSE 4 FISA 
* @Authors :   Neil Amrane
*             Khady Ndyée Diop
*             Wissal Bellahcen
*             Salma Elfiache
*##########################################
*/

#include "ModuleHorloge.h"


//Setup - Procédure d'initialisation de l'horloge
void CLK_INIT(){
  // Initialisation I2C
  Wire.begin(22, 21);   // broches ESP32 SDA=22, SCL=21
  Serial.println("Initialisation RTC...\n");

  while(!rtc.begin()) {
    Serial.println("RTC non détecté !");
  }
  // Si l'RTC a perdu son horloge → on met l'heure du compilateur
  if (rtc.lostPower()) {
    Serial.println("RTC a perdu l'heure, initialisation...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

char* GetTimeNow(){
  DateTime now = rtc.now(); //Récupération de l'heure
  char TimeBuffer[64];    //Buffer pour récuperer la date à afficher
  sprintf(TimeBuffer,"%02d/%02d/%04d %02d:%02d:%02d", 
  now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());

  return TimeBuffer;
}