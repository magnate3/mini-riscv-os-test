CROSS_COMPILE = riscv64-linux-gnu-
CFLAGS = -nostdlib -fno-builtin  -g -Wall -mcmodel=medany
#CFLAGS = -nostdlib -fno-builtin -march=rv32ima -mabi=ilp32 -g -Wall

QEMU = qemu-system-riscv64
QFLAGS = -nographic -smp 1 -machine virt -bios none

GDB = gdb-multiarch
CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump
LD = $(CROSS_COMPILE)ld
