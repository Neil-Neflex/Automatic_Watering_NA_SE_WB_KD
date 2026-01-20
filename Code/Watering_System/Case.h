//Case.h
/*#########################################
* @Project :   Watering System 
* @School :    Poytech Nice Sophia
* @Training :  ELSE 4 FISA 
* @Authors :   Neil Amrane
*              Ndeye Khady  Diop
*              Wissal Bellahcen
*              Salma Elfiache
*##########################################
*/

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
