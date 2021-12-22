.section .text

.global _entry
_entry:

# sp = __stack_start + ((hartid + 1) * 4096)
    la sp, __stack_start
    li a0, 4096
    csrr a1, mhartid
    addi a1, a1, 1
    mul a0, a0, a1
    add sp, sp, a0

    call start
    
loop:
    j loop

#
# machine-mode trap entry.
#
.globl machine_trap_entry
.align 4
machine_trap_entry:

	# mscratch holds mmode trap stack address,
	# swap current sp and mmode trap stack address
	csrrw sp, mscratch, sp

	// call the C trap handler in trap.c
	call machine_trap

	# restore old sp value
	csrrw sp, mscratch, sp

	mret