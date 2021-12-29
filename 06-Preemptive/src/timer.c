#include "timer.h"
#include "lib.h"

// a scratch area per CPU for machine-mode timer interrupts.
reg_t timer_scratch[NCPU][5];

#define interval  20000000 // cycles; about 2 second in qemu.
void timer_init()
{
  // each CPU has a separate source of timer interrupts.
  int id = r_mhartid();
  // ask the CLINT for a timer interrupt.
  // int interval = 1000000; // cycles; about 1/10th second in qemu.

  *(reg_t *)CLINT_MTIMECMP(id) = *(reg_t *)CLINT_MTIME + interval;
// assembly code in kernelvec.S for machine-mode timer interrupt.
  w_mie(r_mie() | MIE_MTIE);
}

static int timer_count = 0;

void timer_handler()
{
  lib_printf("timer_handler: %d\n", ++timer_count);
  int id = r_mhartid();
  *(reg_t *)CLINT_MTIMECMP(id) = *(reg_t *)CLINT_MTIME + interval;
}
