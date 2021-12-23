//#include "os.h"
#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv2.h"
extern void trap_vector();
kerneltrap()
{
    lib_puts("kerneltrap\n");
    uint64 sepc = r_sepc();
    uint64 sstatus = r_sstatus();
    uint64 scause = r_scause();
    uint64 cause_code = scause & 0xfff;
    if((sstatus & SSTATUS_SPP) == 0)
	    lib_puts("kerneltrap: not from supervisor mode");
  // to avoid  Sync exceptions! ,  code = 2
 // if (scause & 0x80000000)
 // {
 //   /* Asynchronous trap - interrupt */
 //   switch (cause_code)
 //   {
 //   case 3:
 //     lib_puts("software interruption!\n");
 //     break;
 //   case 7:
 //     lib_puts("timer interruption!\n");
 //     // disable machine-mode timer interrupts.
 //     //w_sie(~((~r_sie()) | (1 << 7)));
 //     //timer_handler();
 //     // enable machine-mode timer interrupts.
 //     //w_sie(r_sie() | SIE_STIE);
 //     break;
 //   case 11:
 //     lib_puts("external interruption!\n");
 //     break;
 //   default:
 //     lib_puts("unknown async exception!\n");
 //     break;
 //   }
 // }
 // else
 // {
 //   /* Synchronous trap - exception */
 //   // https://juejin.cn/post/7006347707144994853
 //   lib_printf("Sync exceptions! ,  code = %d\n", cause_code);
 //   lib_puts("OOPS! What can I do!");
 //   while (1)
 //   {
 //     /* code */
 //   }
 // }
     // the yield() may have caused some traps to occur,
     //   // so restore trap registers for use by kernelvec.S's sepc instruction.
     w_sepc(sepc);
     w_sstatus(sstatus);
}
