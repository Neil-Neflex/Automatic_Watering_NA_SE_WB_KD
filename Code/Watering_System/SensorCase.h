#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <PNGdec.h>
#include "Case.h"

// Déclaration de la fonction globale définie dans le .ino
extern void drawPngFlash(const uint8_t *data, uint32_t len, int16_t x, int16_t y);

class SensorCase : public Case{
  private:
      const uint8_t *logo;
      int value; //Valeur courante du capteur
      uint32_t logoSize;
      int  img_x, img_y;
      char unite[3]; 

  public:
      SensorCase(int nCase, 
                int val, 
                uint16_t bg, 
                int _x, int _y, int _w, int _h, 
                const uint8_t *_logo, 
                uint32_t _logoSize,
                int  _img_x, 
                int _img_y, 
                char * _unite)
          : Case(nCase, 1, bg, _x, _y, _w, _h), 
          value(val),
          logo(_logo), 
          logoSize(_logoSize),
          img_x(_img_x), 
          img_y(_img_y)           
        {
          // Copie sécurisée du nom
          unite[0] = _unite[0];
          unite[1] = _unite[1];
          unite[2] = '\0';
        }
      
      //Rôle : Dessiner la case ou la mettre à jour et actualise le flag
      void draw(TFT_eSPI &tft) override{
        tft.fillRect(x, y, w, h, bgColor); // zone colorée (ou non)
        tft.drawRect(x, y, w, h, TFT_BLACK); // Cadre noir contour noir de 1px

        if (logo != nullptr && logoSize > 0) {
        drawPngFlash(logo, logoSize, img_x, img_y);
        }

        tft.setFreeFont(&FreeSans9pt7b); 
        tft.setTextSize(1);
        tft.setTextColor(TFT_BLACK, bgColor);

        tft.setCursor(x + 10, y + h - 10);

        String txt = String(value) + " " + unite;
        tft.println(txt);
        
        //mise à jour du flag 
        this-> flag = 0;
      }
      
      //Rôle : renvoie la valeur du capteur
      int getValue() {
        return this->value;
      };
      //Rôle : modifie la valeur affichée à l'écran
      void setValue(int v){
        if (this->value != v){
          this->value = v;
          this->flag = 1;
        }  
      }
};
