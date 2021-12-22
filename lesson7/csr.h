#ifndef RISCV_INCLUDE_CSR_H_
#define RISCV_INCLUDE_CSR_H_

static inline unsigned long mhartid_get(void)
{
    unsigned long cpu;
    asm volatile("csrr %0, mhartid" : "=r" (cpu) );
    return cpu;
}
#define PMP_R     0x01
#define PMP_W     0x02
#define PMP_X     0x04
#define PMP_A     0x18
#define PMP_L     0x80
#define PMP_SHIFT 2

#define PMP_TOR   0x08
#define PMP_NA4   0x10
#define PMP_NAPOT 0x18
#define MSTATUS_MPP_MASK (3L << 11) // previous mode.
#define     MSTATUS_MPP_M (3L << 11)
#define     MSTATUS_MPP_S (1L << 11)
#define     MSTATUS_MPP_U (0L << 11)
#define MSTATUS_MIE (1L << 3)    // machine-mode interrupt enable.
#define MSTATUS_SIE (1L << 1)

static inline unsigned long mstatus_get(void)
{
    unsigned long status;
    asm volatile("csrr %0, mstatus" : "=r" (status) );
    return status;
}

static inline void mstatus_set(unsigned long status)
{
    asm volatile("csrw mstatus, %0" : : "r" (status));
}

#define MIE_MEIP (1L << 11) // external
#define MIE_MTIP (1L << 7)  // timer
#define MIE_MSIP (1L << 3)  // software

#define MIE_SEIP (1L << 9)  // external
#define MIE_STIP (1L << 5)  // timer
#define MIE_SSIP (1L << 1)  // softwares

static inline unsigned long mip_get(void)
{
    unsigned long pend;
    asm volatile("csrr %0, mip" : "=r" (pend) );
    return pend;
}

static inline void mip_set(unsigned long pend)
{
    asm volatile("csrw mip, %0" : : "r" (pend));
}

// Machine-mode Interrupt Enable
#define MIE_MEIE (1L << 11) // external
#define MIE_MTIE (1L << 7)  // timer
#define MIE_MSIE (1L << 3)  // software

#define MIE_SEIE (1L << 9)  // external
#define MIE_STIE (1L << 5)  // timer
#define MIE_SSIE (1L << 1)  // softwares

static inline unsigned long mie_get(void)
{
    unsigned long int_mask;
    asm volatile("csrr %0, mie" : "=r" (int_mask) );
    return int_mask;
}

static inline void mie_set(unsigned long int_mask)
{
    asm volatile("csrw mie, %0" : : "r" (int_mask));
}

// machine exception program counter, holds the
// instruction address to which a return from
// exception will go.
static inline void mepc_set(unsigned long address)
{
    asm volatile("csrw mepc, %0" : : "r" (address));
}

static inline unsigned long mepc_get(void)
{
    unsigned long val;
    asm volatile("csrr %0, mepc" : "=r" (val) );
    return val;
}

// Machine Exception Delegation
static inline unsigned long medeleg_get(void)
{
    unsigned long val;
    asm volatile("csrr %0, medeleg" : "=r" (val) );
    return val;
}

static inline void medeleg_set(unsigned long val)
{
    asm volatile("csrw medeleg, %0" : : "r" (val));
}

// Machine Interrupt Delegation
static inline unsigned long mideleg_get(void)
{
    unsigned long val;
    asm volatile("csrr %0, mideleg" : "=r" (val) );
    return val;
}

static inline void mideleg_set(unsigned long val)
{
    asm volatile("csrw mideleg, %0" : : "r" (val));
}

// Machine-mode Trap Cause
static inline unsigned long mcause_get(void)
{
    unsigned long val;
    asm volatile("csrr %0, mcause" : "=r" (val) );
    return val;
}

static inline unsigned long mtval_get(void)
{
    unsigned long val;
    asm volatile("csrr %0, mtval" : "=r" (val) );
    return val;
}

// Machine-mode interrupt vector
static inline void mtvec_set(unsigned long addr)
{
    asm volatile("csrw mtvec, %0" : : "r" (addr));
}

static inline void mscratch_set(unsigned long val)
{
    asm volatile("csrw mscratch, %0" : : "r" (val));
}

// Machine-mode Counter-Enable
static inline unsigned long mcounteren_get(void)
{
    unsigned long val;
    asm volatile("csrr %0, mcounteren" : "=r" (val) );
    return val;
}

static inline void mcounteren_set(unsigned long val)
{
    asm volatile("csrw mcounteren, %0" : : "r" (val));
}

// machine-mode cycle counter
static inline unsigned long mtime_get(void)
{
    unsigned long val;
    asm volatile("csrr %0, time" : "=r" (val) );
    return val;
}

// Supervisor Status Register, sstatus

#define SSTATUS_SPP (1L << 8)  // Previous mode, 1=Supervisor, 0=User
#define SSTATUS_SPIE (1L << 5) // Supervisor Previous Interrupt Enable
#define SSTATUS_UPIE (1L << 4) // User Previous Interrupt Enable
#define SSTATUS_SIE (1L << 1)  // Supervisor Interrupt Enable
#define SSTATUS_UIE (1L << 0)  // User Interrupt Enable

static inline unsigned long sstatus_get(void)
{
    unsigned long status;
    asm volatile("csrr %0, sstatus" : "=r" (status) );
    return status;
}

static inline void sstatus_set(unsigned long status)
{
    asm volatile("csrw sstatus, %0" : : "r" (status));
}

// Supervisor Interrupt Pending
#define SIP_SEIP (1L << 9) // external
#define SIP_STIP (1L << 5) // timer
#define SIP_SSIP (1L << 1) // software
static inline unsigned long sip_get(void)
{
    unsigned long pend;
    asm volatile("csrr %0, sip" : "=r" (pend) );
    return pend;
}

static inline void sip_set(unsigned long pend)
{
    asm volatile("csrw sip, %0" : : "r" (pend));
}

// Supervisor Interrupt Enable
#define SIE_SEIE (1L << 9) // external
#define SIE_STIE (1L << 5) // timer
#define SIE_SSIE (1L << 1) // software
static inline unsigned long sie_get(void)
{
    unsigned long int_mask;
    asm volatile("csrr %0, sie" : "=r" (int_mask) );
    return int_mask;
}

static inline void sie_set(unsigned long int_mask)
{
    asm volatile("csrw sie, %0" : : "r" (int_mask));
}

// supervisor exception program counter, holds the
// instruction address to which a return from
// exception will go.
static inline unsigned long sepc_get(void)
{
    unsigned long address;
    asm volatile("csrr %0, sepc" : "=r" (address) );
    return address;
}

static inline void sepc_set(unsigned long address)
{
    asm volatile("csrw sepc, %0" : : "r" (address));
}

// Supervisor Trap-Vector Base Address
// low two bits are mode.
static inline unsigned long stvec_get(void)
{
    unsigned long base;
    asm volatile("csrr %0, stvec" : "=r" (base) );
    return base;
}

static inline void stvec_set(unsigned long base)
{
    asm volatile("csrw stvec, %0" : : "r" (base));
}

// Supervisor Scratch register, for early trap handler in trampoline.S.
static inline void sscratch_set(unsigned long val)
{
    asm volatile("csrw sscratch, %0" : : "r" (val));
}

// Supervisor Trap Cause
static inline unsigned long scause_get(void)
{
    unsigned long val;
    asm volatile("csrr %0, scause" : "=r" (val) );
    return val;
}

// Supervisor Trap Value
static inline unsigned long stval_get(void)
{
    unsigned long val;
    asm volatile("csrr %0, stval" : "=r" (val) );
    return val;
}

// enable device interrupts
static inline void intr_on(void)
{
    sstatus_set(sstatus_get() | SSTATUS_SIE);
}

// disable device interrupts
static inline void intr_off(void)
{
    sstatus_set(sstatus_get() & ~SSTATUS_SIE);
}

// are device interrupts enabled?
static inline int is_intr_enable(void)
{
    unsigned long val = sstatus_get();
    return (val & SSTATUS_SIE) != 0;
}

static inline unsigned long sp_get(void)
{
    unsigned long addr;
    asm volatile("mv %0, sp" : "=r" (addr) );
    return addr;
}

// read and write tp, the thread pointer, which holds
// this core's hartid (core number), the index into cpus[].
static inline unsigned long tp_get(void)
{
    unsigned long val;
    asm volatile("mv %0, tp" : "=r" (val) );
    return val;
}

static inline void tp_set(unsigned long val)
{
    asm volatile("mv tp, %0" : : "r" (val));
}

static inline unsigned long ra_get(void)
{
    unsigned long val;
    asm volatile("mv %0, ra" : "=r" (val) );
    return val;
}

// supervisor address translation and protection;
// holds the address of the page table.
static inline unsigned long satp_get(void)
{
    unsigned long addr;
    asm volatile("csrr %0, satp" : "=r" (addr) );
    return addr;
}

static inline void satp_set(unsigned long addr)
{
    asm volatile("csrw satp, %0" : : "r" (addr));
}

// flush the TLB.
static inline void sfence_vma(void)
{
    // the zero, zero means flush all TLB entries.
    asm volatile("sfence.vma zero, zero");
}

#endif /* RISCV_INCLUDE_CSR_H_ */
