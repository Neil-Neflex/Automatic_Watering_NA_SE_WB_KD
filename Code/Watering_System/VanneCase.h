//VanneCase.h
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

class VanneCase : public Case {
  private:
    char nom[3];     // "V1", "V2"... + '\0'
    int frequence;   // fréquence d'arrosage (ex: en jours)
    int duree;       // durée d'arrosage (ex: en minutes)
    bool enable;       // signal d'activation (1 = active 0 = désactivée)
    bool state;       // état 1 = vanne ouverte 0 = vanne fermée 

    int GPIO_Ouverture; //GPIO de l'ESP32 auquel est relié le cable d'activation de l'ouverture de la vanne
    int GPIO_Fermeture; //GPIO de l'ESP32 auquel est relié le cable d'activation de la fermeture de la vanne

  public:
    //Rôle : Constructeur de VanneCase 
    VanneCase(int nCase, int _x, int _y, int _w, int _h, const char *_nom, int _frequence, int _duree, int Gouv, int Gferm)
      : Case(nCase, 1, 
        TFT_WHITE, 
       _x, _y, _w, _h),
       frequence(_frequence),
       duree(_duree),
       enable(1), //quand la vanne est initialisée, elle est activée
       state(0),//qand la vanne est initialisée, elle est fermée
       GPIO_Ouverture(Gouv),
       GPIO_Fermeture(Gferm)
    {
      // Copie sécurisée du nom
      nom[0] = _nom[0];
      nom[1] = _nom[1];
      nom[2] = '\0';
    }

    //Rôle : Dessin complet de la case et met à jour le flag
    void draw(TFT_eSPI &tft) override {
      // Fond + cadre
      tft.fillRect(x, y, w, h, bgColor);
      tft.drawRect(x, y, w, h, TFT_BLACK);

      // ----- Titre : "V1", "V2", ... -----
      // (gros, au centre en haut)
      tft.setFreeFont(&FreeSans9pt7b); 
      tft.setTextSize(2);
      tft.setTextColor(TFT_BLACK, bgColor);
      // tft.setFreeFont(&FreeSansOblique12pt7b);

      int titleX = x + 10;         // ajustable
      int titleY = y + 40;         // ajustable
      tft.setCursor(titleX, titleY);
      tft.print(nom);

      // ----- Texte fréquence + durée -----
      tft.setTextSize(1);
      tft.setTextColor(TFT_BLACK, bgColor);

      char buf[32];
      // Ligne fréquence
      snprintf(buf, sizeof(buf), "F : %d h", frequence);
      tft.setCursor(x + 5, y + h/1.5);
      tft.print(buf);
      // Ligne durée
      snprintf(buf, sizeof(buf), "D: %d m", duree);
      tft.setCursor(x + 5, y + h/1.5 + 15);
      tft.print(buf);

      this->flag = 0;
    }

    //Rôle : met à jour la couleur de la case en fonction des deux booléens d'état
    void updateColor() {
      if (!this->enable){ //La vanne est désactivée -> affichée en rouge
        this->bgColor = TFT_RED;
      }
      else if (this->state){ //La vanne est activée et ouverte -> affichée en vert
        this->bgColor = TFT_GREEN;
      }
      else {this->bgColor = TFT_WHITE;} //La vanne est activée et fermée -> blanc
      this->flag = 1;
    }
    //Rôle : Ferme la vanne
    void fermeture(){
      if (this->enable){
        digitalWrite(this->GPIO_Fermeture, 1);
        delay(5000); // !!!! Modifier mettre MILIS()
        digitalWrite(this->GPIO_Fermeture, 0);
        this->state = 0; //mise à jour de l'indicateur d'état
        this->updateColor(); //mise à jour de l'affichage 
      }
    }
    //Rôle : Ouvre la vanne
    void ouverture(){
      if (this->enable){ //Vérification de l'activation de la vanne
      //envoi d'une impultion de 5s sur le port d'ouverture de la vanne
        digitalWrite(this->GPIO_Ouverture, 1);
        delay(5000);// !!!! Modifier mettre MILIS()
          digitalWrite(this->GPIO_Ouverture, 0);
        this->state = 1; //mise à jour de l'indicateur d'état
        this->updateColor(); //mise à jour de l'affichage 
      }
    }
    //Rôle : désactive la vanne de manière sécurisée et met à jour l'affichage
    void disableUpdate(){
      if(this->state){this->fermeture();}
      this->enable = 0;
      this->updateColor();
    }
    //Rôle : réactive la vanne
    void enableUpdate(){
      this->enable = 1;
      this->updateColor();
    }
 
    //Rôle : Renvoie les valeurs de frequence et de duree
    int getFreq()  {return this->frequence;}
    int getDuree() {return this->duree;}

    //Rôle : Setter la Fréquence et actualise le flag
    void setFrequence(int f) { 
      this->frequence = f; 
      this->flag = 1;
    }
    //Rôle : Setter la durée et actualise le flag
    void setDuree(int d) {
      this->duree = d; 
      this->flag = 1;
    }

    bool getState(){return this->state;}
};




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
