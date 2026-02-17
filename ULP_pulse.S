#include "soc/rtc_cntl_reg.h"
#include "soc/rtc_io_reg.h"
#include "soc/soc_ulp.h"


  /* Define variables, which go into .bss section (zero-initialized data) */
  .bss
  /* Next input signal edge expected: 0 (negative) or 1 (positive) */
  .global ulp_pulse_edge
ulp_pulse_edge:
  .long 0


  /* Next input signal edge expected: 0 (negative) or 1 (positive) */
  .global ulp_next_edge
ulp_next_edge:
  .long 0

  /* Counter started when signal value changes.
     Edge is "debounced" when the counter reaches zero. */
  .global ulp_debounce_counter
ulp_debounce_counter:
  .long 0

  /* Value to which debounce_counter gets reset.
     Set by the main program. */
  .global ulp_debounce_max_count
ulp_debounce_max_count:
  .long 0

  /* Reset value for pulse length recognition */
  .global ulp_pulse_res
ulp_pulse_res:
  .long 0

  /* Current pulse length count */
  .global ulp_pulse_cur
ulp_pulse_cur:
  .long 0

  /* Shortest pulse length count */
  .global ulp_pulse_min
ulp_pulse_min:
  .long 0

  /* Total number of signal edges acquired */
  .global ulp_edge_count
ulp_edge_count:
  .long 0

  /* RTC IO number used to sample the input signal.
     Set by main program. */
  .global ulp_io_number
ulp_io_number:
  .long 0

  /* Code goes into .text section */
  .text
  .global ulp_entry
ulp_entry:
  jump pulse_tick

  .global ulp_read_now
ulp_read_now:
  /* Load io_number */
  move r3, ulp_io_number
  ld r3, r3, 0

  /* Lower 16 IOs and higher need to be handled separately,
   * because r0-r3 registers are 16 bit wide.
   * Check which IO this is.
   */
  move r0, r3
  jumpr ulp_read_io_high, 16, ge

  /* Read the value of lower 16 RTC IOs into R0 */
  READ_RTC_REG(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT_S, 16)
  rsh r0, r0, r3
  jump ulp_read_done

  /* Read the value of RTC IOs 16-17, into R0 */
ulp_read_io_high:
  READ_RTC_REG(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT_S + 16, 2)
  sub r3, r3, 16
  rsh r0, r0, r3

ulp_read_done:
  and r0, r0, 1
  /* State of input changed? */
  move r3, ulp_next_edge
  ld r3, r3, 0
  add r3, r0, r3
  and r3, r3, 1
  jump changed, eq
  /* Not changed */
  /* Reset debounce_counter to debounce_max_count */
  move r3, ulp_debounce_max_count
  move r2, ulp_debounce_counter
  ld r3, r3, 0
  st r3, r2, 0
  /* End program */
  halt

  .global ulp_changed
ulp_changed:
  /* Input state changed */
  /* Has debounce_counter reached zero? */
  move r3, ulp_debounce_counter
  ld r2, r3, 0
  add r2, r2, 0 /* dummy ADD to use "jump if ALU result is zero" */
  jump edge_detected, eq
  /* Not yet. Decrement debounce_counter */
  sub r2, r2, 1
  st r2, r3, 0
  /* End program */
  halt

  .global edge_detected
edge_detected:
  /* Reset debounce_counter to debounce_max_count */
  move r3, ulp_debounce_max_count
  move r2, ulp_debounce_counter
  ld r3, r3, 0
  st r3, r2, 0
  /* Flip next_edge */
  move r3, ulp_next_edge
  ld r2, r3, 0
  add r2, r2, 1
  and r2, r2, 1
  st r2, r3, 0
  /* Increment edge_count */
  move r3, ulp_edge_count
  ld r2, r3, 0
  add r2, r2, 1
  st r2, r3, 0
  /* Full pulse detected? */
  move r3, ulp_pulse_edge
  ld r3, r3, 0
  add r3, r0, r3
  and r3, r3, 1
  jump ulp_pulse_detected, eq
  halt

  .global ulp_pulse_tick
ulp_pulse_tick:
  /* Increment pulse_cur */
  move r3, ulp_pulse_cur
  ld r2, r3, 0
  add r2, r2, 1
  st r2, r3, 0
  jump ulp_read_now

  .global ulp_pulse_detected
ulp_pulse_detected:
  /* Jump to pulse_lower when pulse_cur is lower than pulse_min */
  move r3, ulp_pulse_min
  move r2, ulp_pulse_cur
  ld r3, r3, 0
  ld r2, r2, 0
  sub r3, r2, r3
  jump pulse_lower, ov
  /* Jump to pulse_lower when pulse_min is zero */
  move r3, ulp_pulse_min
  ld r2, r3, 0
  add r2, r2, 0
  jump pulse_lower, eq
  jump pulse_reset

  .global pulse_lower
pulse_lower:
  /* Set pulse_min to pulse_cur */
  move r3, ulp_pulse_cur
  move r2, ulp_pulse_min
  ld r3, r3, 0
  st r3, r2, 0
  jump pulse_reset

  .global pulse_reset
pulse_reset:
  /* Reset pulse_cur to zero */
  move r3, ulp_pulse_res
  move r2, ulp_pulse_cur
  ld r3, r3, 0
  st r3, r2, 0
  halt
