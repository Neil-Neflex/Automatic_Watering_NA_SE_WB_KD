#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>

class Case {
protected:
    int numCase;
    
    int flag; //indicateur de changement de value 
    uint16_t bgColor; //couleur du fond de la case
    int x, y, w, h; //paramètres position et taille

public:
    Case(int nCase, int _f, uint16_t bg, int _x, int _y, int _w, int _h)
        : numCase(nCase), flag(_f), bgColor(bg), x(_x), y(_y), w(_w), h(_h) {}

    virtual void draw(TFT_eSPI &tft) = 0;     // méthode abstraite
    //Rôle : met à jour la couleur du fond et actualise le flag
    virtual void setColor(uint16_t color = TFT_WHITE){
        this->bgColor = color;
        this->flag = 1;
    }   
    virtual uint16_t getColor(){return this -> bgColor;}
    virtual int getFlag() {return this -> flag;}
};
