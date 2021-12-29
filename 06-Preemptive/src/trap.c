#include "os.h"
#include "riscv.h"
extern void trap_vector();
#define CLINT_BASE 0x2000000
#define MTIME (volatile unsigned long long int *)(CLINT_BASE + 0xbff8)
#define MTIMECMP (volatile unsigned long long int *)(CLINT_BASE + 0x4000)
void setup_pmp(void)
{
  // Set up a PMP to permit access to all of memory.
  // Ignore the illegal-instruction trap if PMPs aren't supported.
  uintptr_t pmpc = PMP_NAPOT | PMP_R | PMP_W | PMP_X;
  asm volatile ("la t0, 1f\n\t"
                "csrrw t0, mtvec, t0\n\t"
                "csrw pmpaddr0, %1\n\t"
                "csrw pmpcfg0, %0\n\t"
                ".align 2\n\t"
                "1: csrw mtvec, t0"
                : : "r" (pmpc), "r" (-1UL) : "t0");
}
void trap_init()
{
  //setup_pmp();
  w_mtvec((reg_t)trap_vector);
  // enable machine-mode interrupts.
  w_mstatus(r_mstatus() | MSTATUS_MIE);
}

int count = 0;
void handle_interrupt(uint64_t mcause) {
    unsigned long x = r_mstatus();
    if ((mcause << 1 >> 1) == 0x7) {
        lib_puts("Timer Interrupt: ");

        *MTIMECMP = *MTIME + 0xfffff * 5;
        if (count == 10) {
            unsigned long long int mie;
            asm volatile("csrr %0, mie" : "=r"(mie));
            mie &= ~(1 << 7);
            asm volatile("csrw mie, %0" : "=r"(mie));
        }
      reg_t mepc = (reg_t)&os_kernel;
      asm volatile("csrw mepc, %0" : : "r"(mepc));
    } else {
        while (1)
            ;
    }
}

void handle_exception(uint64_t mcause) {
    unsigned long long int mie;

    if (mcause == 0x8) {
        *MTIMECMP = *MTIME + 0xfffff * 5;

        asm volatile("csrr %0, mie" : "=r"(mie));
        mie |= (1 << 7);
        asm volatile("csrw mie, %0" : "=r"(mie));
    } else {
        while (1)
            ;
    }
}

void handle_trap() {
    uint64_t mcause, mepc;
    asm volatile("csrr %0, mcause" : "=r"(mcause));
    asm volatile("csrr %0, mepc" : "=r"(mepc));

    if (mcause >> 63) {
        handle_interrupt(mcause);
    } else {
        handle_exception(mcause);
        asm volatile("csrr t0, mepc");
        asm volatile("addi t0, t0, 0x4");
        asm volatile("csrw mepc, t0");
    }
}
reg_t trap_handler(reg_t epc, reg_t cause)
{
  reg_t return_pc = epc;
  reg_t cause_code = cause & 0xfff;
  //reg_t cause_code = cause & 0xfff;

  //lib_puts("trap_handler!\n");
#if 1
  if (cause >> 63)
  {
    /* Asynchronous trap - interrupt */
    switch (cause << 1 >> 1)
    {
    case 3:
      lib_puts("software interruption!\n");
      int id = r_mhartid();
      *(uint32_t*)CLINT_MSIP(id) = 0;
      break;
    case 7:
      lib_puts("timer interruption!\n");
      // disable machine-mode timer interrupts.
      w_mie(~((~r_mie()) | (1 << 7)));
      timer_handler();
      return_pc = (reg_t)&os_kernel;
      // enable machine-mode timer interrupts.
      w_mie(r_mie() | MIE_MTIE);
      break;
    case 11:
      lib_puts("external interruption!\n");
      break;
    default:
      lib_puts("unknown async exception!\n");
      break;
    }
  }
  else
  {
    /* Synchronous trap - exception */
    // https://juejin.cn/post/7006347707144994853
    // risc v store/amo access fault
    lib_printf("Sync exceptions! ,  code = %d, epc = %x\n", cause_code, epc);
    lib_puts("OOPS! What can I do! \n");
    //return_pc += 4;
   // while (1)
   // {
   //   /* code */
   // }
  }
#endif
  return return_pc;
}
