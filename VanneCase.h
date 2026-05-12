#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Case.h"

class VanneCase : public Case {
  private:
    char nom[3];     
    int frequence;   
    int duree;       
    bool enable;     
    bool state;      

    int GPIO_Ouverture; 
    int GPIO_Fermeture; 

    // --- VARIABLES DE TEMPS AJOUTÉES ICI ---
    unsigned long timerVanne = 0;
    bool enAction = false; 

  public:
    VanneCase(int nCase, int _x, int _y, int _w, int _h, const char *_nom, int _frequence, int _duree, int Gouv, int Gferm)
      : Case(nCase, 1, TFT_WHITE, _x, _y, _w, _h),
       frequence(_frequence),
       duree(_duree),
       enable(1),
       state(0),
       GPIO_Ouverture(Gouv),
       GPIO_Fermeture(Gferm)
    {
      nom[0] = _nom[0];
      nom[1] = _nom[1];
      nom[2] = '\0';
    }

    void draw(TFT_eSPI &tft) override {
      tft.fillRect(x, y, w, h, bgColor);
      tft.drawRect(x, y, w, h, TFT_BLACK);

      tft.setFreeFont(&FreeSans9pt7b); 
      tft.setTextSize(2);
      tft.setTextColor(TFT_BLACK, bgColor);

      tft.setCursor(x + 10, y + 40);
      tft.print(nom);

      tft.setTextSize(1);
      char buf[32];
      snprintf(buf, sizeof(buf), "F : %d h", frequence);
      tft.setCursor(x + 5, y + h/1.5);
      tft.print(buf);
      snprintf(buf, sizeof(buf), "D: %d m", duree);
      tft.setCursor(x + 5, y + h/1.5 + 15);
      tft.print(buf);

      this->flag = 0;
    }

    void updateColor() {
      if (!this->enable) { this->bgColor = TFT_RED; }
      else if (this->state) { this->bgColor = TFT_GREEN; }
      else { this->bgColor = TFT_WHITE; }
      this->flag = 1;
    }

    // --- NOUVELLE FERMETURE SANS DELAY ---
    void fermeture(){
      if (this->enable && !this->enAction){
        digitalWrite(this->GPIO_Fermeture, HIGH); 
        this->timerVanne = millis();          
        this->enAction = true;
        this->state = 0; 
        this->updateColor(); 
      }
    }

    // --- NOUVELLE OUVERTURE SANS DELAY ---
    void ouverture(){
      if (this->enable && !this->enAction){
        digitalWrite(this->GPIO_Ouverture, HIGH);
        this->timerVanne = millis();
        this->enAction = true;
        this->state = 1;
        this->updateColor();
      }
    }

    // --- LE SURVEILLANT DU TEMPS ---
    void checkTimer() {
      if (this->enAction) {
        if (millis() - this->timerVanne >= 5000) { 
          digitalWrite(this->GPIO_Ouverture, LOW);    
          digitalWrite(this->GPIO_Fermeture, LOW);
          this->enAction = false;                   
          Serial.print("Fin impulsion ");
          Serial.println(nom);
        }
      }
    }

    void disableUpdate(){
      if(this->state){this->fermeture();}
      this->enable = 0;
      this->updateColor();
    }

    void enableUpdate(){
      this->enable = 1;
      this->updateColor();
    }
 
    int getFreq()  {return this->frequence;}
    int getDuree() {return this->duree;}

    void setFrequence(int f) { this->frequence = f; this->flag = 1; }
    void setDuree(int d) { this->duree = d; this->flag = 1; }
    bool getState(){return this->state;}
};