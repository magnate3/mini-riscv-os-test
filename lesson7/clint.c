#include "address.h"

// core local interruptor (CLINT), which contains the timer.
#define CLINT_MSIP             (CLINT_REG_BASE + 0x0)
#define CLINT_MTIMECMP         (CLINT_REG_BASE + 0x4000)
#define CLINT_MTIME            (CLINT_REG_BASE + 0xBFF8) // cycles since boot.

void msoftint_make(void)
{
    *(volatile unsigned int*)CLINT_MSIP = 1;
}

void msoftint_clear(void)
{
    *(volatile unsigned int*)CLINT_MSIP = 0;
}

unsigned long timer_get(void)
{
    return (*(volatile unsigned long*)CLINT_MTIME);
}

void timer_set(unsigned long interval)
{
    *(volatile unsigned long*)CLINT_MTIMECMP = interval;
}