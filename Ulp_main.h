/*
    Put your ULP globals here you want visibility
    for your sketch. Add "ulp_" to the beginning
    of the variable name and must be size 'uint32_t'
*/
#include "Arduino.h"

extern uint32_t ulp_entry=0;

extern uint32_t ulp_pulse_edge=0;
extern uint32_t ulp_next_edge=0;
extern uint32_t ulp_debounce_counter;
extern uint32_t ulp_debounce_max_count=400;
extern uint32_t ulp_pulse_res=0;
extern uint32_t ulp_pulse_cur=0;
extern uint32_t ulp_pulse_min=0;
extern uint32_t ulp_edge_count =0;
extern uint32_t ulp_io_number=34;
