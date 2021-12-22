#include "uart.h"
#include "csr.h"
#include "clint.h"

// in kernel.ld.
extern char __stack_start[];

// in entry.S.
void machine_trap_entry(void);

static void machine_trap_init(void)
{
    //this stack is used for machine_trap_entry in entry.S
    mscratch_set((unsigned long)(__stack_start + 4096 * 2));

    // set the machine-mode trap handler.
    mtvec_set((unsigned long)machine_trap_entry);

    // enable machine-mode interrupts.
    mstatus_set(mstatus_get() | MSTATUS_MIE);

    // enable machine-mode timer and soft interrupts.
    mie_set(mie_get() | MIE_MTIE | MIE_MSIE);
}

static void main(void)
{
    printf("%s %d.\r\n", __func__, __LINE__);

    while(1);
}

void setup_pmp(void)
{
  // Set up a PMP to permit access to all of memory.
  // Ignore the illegal-instruction trap if PMPs aren't supported.
  unsigned long pmpc = PMP_NAPOT | PMP_R | PMP_W | PMP_X;
  asm volatile ("la t0, 1f\n\t"
                "csrrw t0, mtvec, t0\n\t"
                "csrw pmpaddr0, %1\n\t"
                "csrw pmpcfg0, %0\n\t"
                ".align 2\n\t"
                "1: csrw mtvec, t0"
                : : "r" (pmpc), "r" (-1UL) : "t0");
}
static void machine_switchto_supervisor(void)
{
    setup_pmp();
    // set M Previous Privilege mode to Supervisor, for mret.
    unsigned long x = mstatus_get();
    x &= ~MSTATUS_MPP_MASK;
    //x |= MSTATUS_MPP_M;
    x |= MSTATUS_MPP_S;
    mstatus_set(x);

    // set M Exception Program Counter to main, for mret.
    // requires gcc -mcmodel=medany
    mepc_set((unsigned long)main);

    // disable paging for now.
    satp_set(0);

    // delegate interrupts and exceptions to supervisor mode.
    medeleg_set(0xb109);
    mideleg_set(0x222);

    printf("%s %d.\r\n", __func__, __LINE__);
    // switch to supervisor mode and jump to main().
    asm volatile("mret");
}

void start(void)
{
    printf("%s %d.\r\n", __func__, __LINE__);

    machine_trap_init();

    //msoftint_make();
    //timer_set(timer_get() + TIMER_CLK_RATE);

    machine_switchto_supervisor();
}
