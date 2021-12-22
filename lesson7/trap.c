#include "uart.h"
#include "clint.h"
#include "csr.h"

#define S_SOFT_INT      (1)
#define M_SOFT_INT      (3)
#define S_TIMER_INT     (5)
#define M_TIMER_INT     (7)
#define S_EXT_INT       (9)
#define M_EXT_INT       (11)

#define INSTRUCTION_ADDR_MISALIGNED     (0)
#define INSTRUCTION_ACCESS_FAULT        (1)
#define ILLEGAL_INSTRUCTION             (2)
#define BREAK_POINT                     (3)
#define LOAD_ADDR_MISALIGNED            (4)
#define LOAD_ACCESS_FAULT               (5)
#define STORE_ADDR_MISALIGNED           (6)
#define STORE_ACCESS_FAULT              (7)
#define ECALL_FROM_UMODE                (8)
#define ECALL_FROM_SMODE                (9)
#define ECALL_FROM_MMODE                (11)
#define INSTRUCTION_PAGE_FAULT          (12)
#define LOAD_PAGE_FAULT                 (13)
#define STORE_PAGE_FAULT                (15)

static char *interrupt_cause[] = {
    "Reserved",
    "Supervisor software interrupt",
    "Reserved",
    "Machine software interrupt",
    "Reserved",
    "Supervisor timer interrupt",
    "Reserved",
    "Machine timer interrupt",
    "Reserved",
    "Supervisor external interrupt",
    "Reserved",
    "Machine external interrupt",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

static char *exception_cause[] = {
    "Instruction address misaligned",
    "Instruction access fault",
    "Illegal instruction",
    "Breakpoint",
    "Load address misaligned",
    "Load access fault",
    "Store/AMO address misaligned",
    "Store/AMO access fault",
    "Environment call from U-mode",
    "Environment call from S-mode",
    "Reserved",
    "Environment call from M-mode",
    "Instruction page fault",
    "Load page fault",
    "Reserved",
    "Store/AMO page fault"
};

// interrupts and exceptions from kernel code go here via machine_trap_entry.
void machine_trap(void)
{ 
    unsigned long cause = mcause_get();
    unsigned long mepc  = mepc_get();
    unsigned long tval  = mtval_get();

    int is_int = (cause & (1l << 63l)) ? 1 : 0;
    int mcode = cause & 0xff;

    if (mcode >= 16) {
        printf("%s : %s.\r\n", is_int ? "Interrupt" : "Exception", "Unknown code");
        return;
    }

    if (is_int) {
        printf("Interrupt : %s.\r\n", interrupt_cause[mcode]); 
        switch (mcode) {
        case M_SOFT_INT:
            msoftint_clear();
            break;
        case M_TIMER_INT:
            timer_set(timer_get() + TIMER_CLK_RATE);
            break;
        }
    } else {
        printf("Exception : %s.\r\n", exception_cause[mcode]); 
        switch (mcode) {
        case ILLEGAL_INSTRUCTION:
            printf("tval = %p\r\n", tval);
            printf("mepc = %p\r\n", mepc);
            break;
        case ECALL_FROM_SMODE:
            break;
        }

        mepc_set(mepc + 4);
    }

    return;
}