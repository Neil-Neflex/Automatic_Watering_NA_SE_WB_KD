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
#include "SensorManagment.h"




//Rôle : Appel les procédures permetant la mise à jour des valeurs sensors
void setData(){
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
void setTemperature(int x){
  int tmp = 32; //nouvelle valeur
  auto* sensor = asSensor(0); //pointeur vers la case température

  //... 
  if (sensor->getValue() != x){
    sensor->setValue(x);
    TemperatureBgColor();
  }
}

void setHumidite(int x){
  int tmp = 39; //nouvelle valeur
  auto* sensor = asSensor(1);

  if (sensor->getValue() != x){
    sensor->setValue(x);
    HumiditeBgColor();
  }
}

void setPluvio(int x){
  int tmp = 3; //nouvelle valeur
  auto* sensor = asSensor(2);

  if (sensor->getValue() != x){
    sensor->setValue(x);
    PluvioBgColor();
  }
}

void setHumiditeGround(int x){
  int tmp =0; //nouvelle valeur
  auto* sensor = asSensor(3);

  if (sensor->getValue() != x){
    sensor->setValue(x);
    HumiditeGroundBgColor();
  }

}

//########## FONCTIONS MAJ COLORS ##############

void TemperatureBgColor(){
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

void HumiditeBgColor(){
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

void PluvioBgColor(){
  auto* sensor = asSensor(2);
  int val = sensor -> getValue();

  if (val > 5 && val<10){if (sensor -> getColor() != TFT_CYAN){sensor -> setColor(TFT_CYAN);}}
  else if (val >= 10){
    if (sensor -> getColor() != TFT_BLUE){ sensor -> setColor(TFT_BLUE);}
    //sensor -> setLock(1);
  }
  else { sensor -> setColor(TFT_WHITE);}
}

void HumiditeGroundBgColor(){
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
