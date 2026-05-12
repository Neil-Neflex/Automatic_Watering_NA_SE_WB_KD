#pragma once
#include <Arduino.h>
#include "esp32/ulp.h"
#include "driver/rtc_io.h"
#include "esp_sleep.h"

#define PLUVIO_PIN GPIO_NUM_34        
#define ULP_COUNT_ADDR      20

void initULP();
uint32_t getPulseCount();

static const ulp_insn_t ulp_program[] = {
    // Récupère l'état actuel de la broche 34 (Bit 18)
    I_RD_REG(RTC_GPIO_IN_REG, 18, 18),   

    // Compare avec l'état précédent 
    I_MOVI(R3, ULP_COUNT_ADDR + 1),
    I_LD(R2, R3, 0),
    I_SUBR(R1, R0, R2),
    M_BXZ(1),     // Si identique -> on saute

    // Mémorise le nouvel état
    I_ST(R0, R3, 0),

    // Incrémente le compteur
    I_MOVI(R3, ULP_COUNT_ADDR),
    I_LD(R2, R3, 0),
    I_ADDI(R2, R2, 1),
    I_ST(R2, R3, 0),

    // Fin du cycle
    M_LABEL(1),
    I_HALT()
};

void initULP() {
    rtc_gpio_init(PLUVIO_PIN);
    rtc_gpio_set_direction(PLUVIO_PIN, RTC_GPIO_MODE_INPUT_ONLY);
    rtc_gpio_hold_en(PLUVIO_PIN);      

    if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER) {
        uint32_t etat_initial = (REG_READ(RTC_GPIO_IN_REG) >> 18) & 0x1;
        RTC_SLOW_MEM[ULP_COUNT_ADDR] = 0;
        RTC_SLOW_MEM[ULP_COUNT_ADDR + 1] = etat_initial;
        Serial.println("ULP: Premier demarrage - module initialisé.");
    } 

    size_t size = sizeof(ulp_program) / sizeof(ulp_insn_t);
    ulp_process_macros_and_load(0, ulp_program, &size);
    
    // CORRECTION MAJEURE 2 : On règle la vitesse de l'ULP à 20 millisecondes !
    // Cela crée un "Debounce" (anti-rebond) matériel parfait.
    ulp_set_wakeup_period(0, 20000); 
    
    ulp_run(0);
    Serial.println("ULP: Programme chargé et démarré avec succès.");
}

uint32_t getPulseCount() {
    // CORRECTION MAJEURE 1 : Le masque & 0xFFFF pour ignorer l'adresse de l'ULP !
    uint32_t imp = RTC_SLOW_MEM[ULP_COUNT_ADDR] & 0xFFFF;
    RTC_SLOW_MEM[ULP_COUNT_ADDR] = 0;
    
    // CORRECTION MAJEURE 3 : 1 basculement = 1 aller + 1 retour. On divise donc par 2.
    return imp / 2;
}