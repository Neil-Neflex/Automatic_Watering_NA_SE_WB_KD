//SensorManagment.h
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

//Rôle : cast le pointeur sur listData[idx] pour les rendre utilisable
inline SensorCase* asSensor(int idx) {
  return static_cast<SensorCase*>(listeData[idx]);
}

void setData();

void setTemperature(int x);
void setHumidite(int x);
void setPluvio(int x);
void setHumiditeGround(int x);

void TemperatureBgColor();
void HumiditeBgColor();
void PluvioBgColor();
void HumiditeGroundBgColor();




/*
*   _______    ______    ___       ___  ___  ___________  _______   ______    __    __      
*  |   __ "\  /    " \  |"  |     |"  \/"  |("     _   ")/"     "| /" _  "\  /" |  | "\     
*  (. |__) :)// ____  \ ||  |      \   \  /  )__/  \\__/(: ______)(: ( \___)(:  (__)  :)    
*  |:  ____//  /    ) :)|:  |       \\  \/      \\_ /    \/    |   \/ \      \/      \/     
*  (|  /   (: (____/ //  \  |___    /   /       |.  |    // ___)_  //  \ _   //  __  \\     
* /|__/ \   \        /  ( \_|:  \  /   /        \:  |   (:      "|(:   _) \ (:  (  )  :)    
*(_______)   \"_____/    \_______)|___/          \__|    \_______) \_______) \__|  |__/     
*                                                                                           
*      _____  ___       __   ___      __   __  ___       ________                                
*     (\"   \|"  \     |/"| /  ")    |"  |/  \|  "|     /"       )                               
*     |.\\   \    |    (: |/   /     |'  /    \:  |    (:   \___/                                
*     |: \.   \\  |    |    __/      |: /'        |     \___  \                                  
*     |.  \    \. |    (// _  \       \//  /\'    |      __/  \\                                 
*     |    \    \ |    |: | \  \      /   /  \\   |     /" \   :)                                
*      \___|\____\)    (__|  \__)    |___/    \___|    (_______/                                                                                                                           
*/
