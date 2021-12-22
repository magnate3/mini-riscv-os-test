# compile objects set
KERNEL_IMAGE_NAME=kernelimage

OBJS = \
  entry.o \
  start.o \
  uart.o \
  trap.o \
  clint.o

# cross compiler and flag set
CROSS_COMPILER =  riscv64-linux-gnu-
CC = $(CROSS_COMPILER)gcc
AS = $(CROSS_COMPILER)gas
LD = $(CROSS_COMPILER)ld
OBJCOPY = $(CROSS_COMPILER)objcopy
OBJDUMP = $(CROSS_COMPILER)objdump

CFLAGS = -Wall -Werror -O -fno-omit-frame-pointer -ggdb
CFLAGS += -mcmodel=medany
CFLAGS += -ffreestanding -fno-common -nostdlib -mno-relax
CFLAGS += -I.
CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)

# Disable PIE when possible (for Ubuntu 16.10 toolchain)
ifneq ($(shell $(CC) -dumpspecs 2>/dev/null | grep -e '[^f]no-pie'),)
CFLAGS += -fno-pie -no-pie
endif
ifneq ($(shell $(CC) -dumpspecs 2>/dev/null | grep -e '[^f]nopie'),)
CFLAGS += -fno-pie -nopie
endif

LDFLAGS = -z max-page-size=4096

# compile kernel
$(KERNEL_IMAGE_NAME): $(OBJS) kernel.ld
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL_IMAGE_NAME) $(OBJS) 
	$(OBJDUMP) -S $(KERNEL_IMAGE_NAME) > kernel.asm
	$(OBJDUMP) -t $(KERNEL_IMAGE_NAME) | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > kernel.sym
	
clean: 
	rm -f *.tex *.dvi *.idx *.aux *.log *.ind *.ilg \
	$(OBJS) *.asm *.sym \
	$(KERNEL_IMAGE_NAME)
	
# qemu set
QEMU = qemu-system-riscv64

ifndef CPUS
CPUS := 1
endif

QEMUOPTS = -machine virt -bios none -kernel $(KERNEL_IMAGE_NAME) -m 128M -smp $(CPUS) -nographic

qemu: $(KERNEL_IMAGE_NAME)
	$(QEMU) $(QEMUOPTS)
