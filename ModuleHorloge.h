

#include <Wire.h>
#include <RTClib.h>
#include <Arduino.h>

extern RTC_DS3231 rtc;
//Setup - Procédure d'initialisation de l'horloge
void CLK_INIT();

const char* GetTimeNow();

