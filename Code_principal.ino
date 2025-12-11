
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <RTClib.h>
#include <PNGdec.h> // PNG Décoder - permet la décompression des fichier PNG et l'extraction RGBA

#include "images.h" //images a afficher

#define SCREEN_LED 13

TFT_eSPI tft = TFT_eSPI();
RTC_DS3231 rtc;
PNG png;

int16_t pngX, pngY; // Position courante d’affichage du PNG (utilisée par PngDraw)

//Variables globales des capteurs environementaux
float TEMPERATURE;
float HUMIDITE;
float PLUVIOMET;
float HUMIDITE_SOL;

void setup() {
  //---------- CLK INIT -----------------
  Serial.begin(9600);
  // Initialisation I2C
  Wire.begin(22, 21);   // broches ESP32 SDA=22, SCL=2 
  Serial.println("Initialisation RTC...");
  while(!rtc.begin()) {
    Serial.println("RTC non détecté !");
  }
  // Si l'RTC a perdu son horloge → on met l'heure du compilateur
  if (rtc.lostPower()) {
    Serial.println("RTC a perdu l'heure, initialisation...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }


  //------------Initialisation de l'écran----------------
  tft.init();
  pinMode(SCREEN_LED, OUTPUT);
  digitalWrite(SCREEN_LED, HIGH);
  tft.setRotation(3);  // portrait: 0 ou 2, paysage: 1 ou 3.

  // affichage d'un message à l'écran:
  tft.fillScreen(TFT_WHITE); // on efface tout (fond blanc)
  tft.setFreeFont(&FreeSansOblique12pt7b); // police de caractère
  tft.setCursor(20, 70); // position du début du message
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.println("Notre équipe : ");
  tft.setTextColor(TFT_RED, TFT_WHITE);
  tft.println("Neil A");
  tft.println("Khady D");
  tft.println("Salma E");
  tft.println("Wissal B");
    delay(1500);

    //----------------------
  tft.fillScreen(TFT_WHITE);
  //TFT_LIGHTGREY   //TFT_PURPLE
  tft.fillRect(0, 0, 320, 40, TFT_LIGHTGREY);
  for (int i=0;i<4;i++){
    tft.fillRect(80*i, 40, 80, 100, TFT_GREEN);
    tft.drawRect(80*i, 40, 80, 100, TFT_BLACK);// Contour noir de 1px
    tft.fillRect(80*i, 140, 80, 100, TFT_YELLOW);
    tft.drawRect(80*i, 140, 80, 100, TFT_BLACK);
  }

  

}


//Boucle 
void loop() {

  Routine();


}

void Routine(){
  AfficherHeure();
  dessinnerCarre(TFT_RED, TFT_BLUE);
  affichageIcon();
  delay(1000);
  AfficherHeure();
  dessinnerCarre(TFT_BLUE, TFT_RED);
  affichageIcon();
  delay(1000);
}
//Affiche l'heure et la date dans la partie supérieur de l'affichage
void AfficherHeure(){
  //MAJ param affichage (taille texte + couleurs)
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK, TFT_LIGHTGREY);

  DateTime now = rtc.now(); //Récupération de l'heure
  char TimeBuffer[64];    //Buffer pour récuperer la date à afficher
  sprintf(TimeBuffer,"%02d/%02d/%04d %02d:%02d:%02d", 
  now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());
  //on affiche temporairement la date dans la console série
  Serial.printf(TimeBuffer);
  tft.drawString(TimeBuffer, 5, 10, 2); // affichage Heure écran

}
//fonction auxiliaire - Interface 
void dessinnerCarre(uint32_t colorUp, uint32_t colorDown){
  for (int i=0;i<4;i++){
    tft.fillRect(80*i, 40, 80, 100, colorUp);
    tft.drawRect(80*i, 40, 80, 100, TFT_BLACK);// Contour noir de 1px
    tft.fillRect(80*i, 140, 80, 100, colorDown);
    tft.drawRect(80*i, 140, 80, 100, TFT_BLACK);
  }
}
// fonction auxiliaire - Interface - 
//affiche les 4 icons de l'a
void affichageIcon(){
  drawPngFlash(temperature, sizeof(temperature), 15, 45);
  drawPngFlash(pluie, sizeof(pluie), 90, 45);
  drawPngFlash(humidite, sizeof(humidite), 180, 45);
  drawPngFlash(terre, sizeof(terre), 260, 45);
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
