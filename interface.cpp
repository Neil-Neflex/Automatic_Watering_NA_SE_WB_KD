
#include "Interface.h"
#define FSS9  &FreeSans9pt7b
#define FSS12 &FreeSans12pt7b
//Rôle : Affiche l'heure et la date dans la partie supérieur de l'affichage
void AfficherHeure(){
  // On utilise l'alias FSS12 défini dans le .ino
  tft.setFreeFont(FSS9);
  tft.setTextSize(1);
  tft.fillRect(0, 0, 320, 35, TFT_LIGHTGREY);
  
  tft.setTextColor(TFT_BLACK);
  const char* TB = GetTimeNow();
  
  // Ajuste les coordonnées (x, y) car les polices GFX s'écrivent 
  // depuis la ligne de base (le bas des lettres).
  tft.setCursor(5, 25);
  tft.print(TB);
  
  // On affiche aussi dans la console pour vérifier
  Serial.println(TB);
}

void DrawPrincipal(){
  //parcours de toute les parties de l'affichage (toutes les cases)
    for (int i = 0; i<8; i++){
      //si un flag est à 1 -> on redessine la case
      if (listeData[i]->getFlag()){
        listeData[i]->draw(tft);
      }
    }
    //On actualise l'heure
    AfficherHeure();
}

void DrawVanPara(Case* c){
    Serial.printf("NOT IMPLEMENTED YET");
}

void GestionnaireEcran(EtatEcran E){
  switch (E){
    case Principal : DrawPrincipal(); break;
    case Vanne1 : DrawVanPara(listeData[4]); break;
    case Vanne2 : DrawVanPara(listeData[5]); break;
    case Vanne3 : DrawVanPara(listeData[6]); break;
    case Vanne4 : DrawVanPara(listeData[7]); break;
    default : DrawPrincipal(); break;
  }
}

//Rôle : fonction CallBack appelée par drawPng, permettant la gestion des image png avec composant Alpha (Transparence)
int pngDraw(PNGDRAW *pDraw) {
    static uint16_t lineBuf[320]; // ligne en RGB565
    static uint8_t mask[40];      // masque alpha (1 bit par pixel)

    // 1) Récupération de la ligne en RGB565
    png.getLineAsRGB565(pDraw, lineBuf, PNG_RGB565_BIG_ENDIAN, 0xFFFFFFFF);

    // 2) Récupération du masque alpha (détermine quels pixels sont visibles)
    bool hasOpaque = png.getAlphaMask(pDraw, mask, 255);

    if (!hasOpaque) {
        return 1; // Ligne totalement transparente → ne rien afficher
    }

    // 3) Écriture pixel par pixel selon le masque
    for (int x = 0; x < pDraw->iWidth; x++) {
        int byteIndex = x >> 3;       // 8 pixels par octet
        int bitIndex  = 7 - (x & 7);  // MSB first

        if (mask[byteIndex] & (1 << bitIndex)) {
            // pixel opaque → on dessine
            tft.drawPixel(pngX + x, pngY + pDraw->y, lineBuf[x]);
        }
    }

    return 1;
}

//Rôle : fonction utilitaire pour dessiner les image png
void drawPngFlash(const uint8_t *data, uint32_t len, int16_t x, int16_t y) {
    pngX = x;
    pngY = y;

    int rc = png.openFLASH((uint8_t*)data, len, pngDraw);
    if (rc == PNG_SUCCESS) {
        png.decode(NULL, 0);
        png.close();
    } else {
        Serial.print("Erreur PNG rc=");
        Serial.println(rc);
    }
}







