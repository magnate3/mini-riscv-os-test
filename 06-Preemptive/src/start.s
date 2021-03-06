.equ REGBYTES, 8
#.equ STACK_SIZE,  ((1 << 12) - 128) 

.section .text.start

.globl _start
_start:
    csrr   t0, mhartid
    lui    t1, 0
    beq    t0, t1, 2f

1:  wfi
    j      1b

2:
    # initialize global pointer
    la gp, _gp
  
    # initialize stack pointer
    la sp, stack_top

    # At the end of start_kernel, schedule() will call MRET to switch
    # to the first task, so we parepare the mstatus here.
    # Notice: default mstatus is 0
    # Set mstatus.MPP to 3, so we still run in Machine mode after MRET.
    # Set mstatus.MPIE to 1, so MRET will enable the interrupt.
    li      t0, 3 << 11 | 1 << 7
    csrr    a1, mstatus
    or      t0, t0, a1
    csrw    mstatus, t0
    call   os_main
