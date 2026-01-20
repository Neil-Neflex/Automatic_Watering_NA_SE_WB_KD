#pragma once

#include <Arduino.h>
#include <PNGdec.h>
#include <TFT_eSPI.h>

#include "Case.h"
#include "ModuleHorloge.h"



// Définis dans le .ino
extern TFT_eSPI tft;
extern PNG png;
extern int16_t pngX, pngY;
extern Case* listeData[8];
// listeData[4] = VanneCase1
// listeData[5] = VanneCase2
// listeData[6] = VanneCase3
// listeData[7] = VanneCase4

enum EtatEcran {
  Principal,
  Vanne1,
  Vanne2,
  Vanne3,
  Vanne4,
  Parametre
};

void AfficherHeure();

int pngDraw(PNGDRAW *pDraw);
void drawPngFlash(const uint8_t *data, uint32_t len, int16_t x, int16_t y);
uint8_t Alpha(uint8_t fg, uint8_t bg, uint8_t alpha);

void DrawPrincipal();
void DrawVanPara(int VanNumber);
void GestionnaireEcran(EtatEcran E);
