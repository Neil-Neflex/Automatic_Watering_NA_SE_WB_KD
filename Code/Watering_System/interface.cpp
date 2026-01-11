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
#include "Interface.h"

//Rôle : Affiche l'heure et la date dans la partie supérieur de l'affichage
void AfficherHeure(){
  //MAJ param affichage (taille texte + couleurs)
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK, TFT_LIGHTGREY);
  char* TB = GetTimeNow();
  
  //on affiche temporairement la date dans la console série
  Serial.printf(TB);
  tft.drawString(TB, 5, 10, 2); // affichage Heure écran

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
