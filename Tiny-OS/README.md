# Tiny-OS

[Tiny OS is a operating system written from scratch in C that runs on RISC-V.](https://github.com/johnyob/Tiny-OS/blob/db683c9acc7abecc600088469f5f3fd69f31ed64/src/threads/thread.c)

# problem1
add access  mem  permission
```

    li      t0, 0xffffffff
    csrw    pmpaddr0, t0
    li      t0, 0xf
    csrw    pmpcfg0, t0
	
```


# problem2 

![can not enter _start](https://github.com/magnate3/mini-riscv-os-test/blob/main/Tiny-OS/pic/start.png)

```
0x0000000000001000 in ?? ()
(gdb) b _start
Note: breakpoint 1 also set at pc 0x8000020c.
Breakpoint 2 at 0x8000020c: file ../src/boot.S, line 18.
(gdb) c
Continuing.
```
![can not enter init](https://github.com/magnate3/mini-riscv-os-test/blob/main/Tiny-OS/pic/init.png)

```
(gdb) n
64          csrw    pmpaddr0, t0
(gdb) n
65          li      t0, 0xf
(gdb) n
66          csrw    pmpcfg0, t0
(gdb) n
71          call init
(gdb) n
```


change  linker.ld
```
 linker.ld 
    .text :
    {
        *(.text.start);
        /*(.text .text.*)*/
        PROVIDE(__TEXT_START = .);
        *(.text.init) *(.text .text.*)
        /*(.text)*/
        . =  ALIGN(0x1000);
        PROVIDE(__TEXT_END = .);
    } >ram
```
change  src/boot.S

```
cat ../src/boot.S

#.section .text
.section .text.start

```

#  run

```
qemu-system-riscv64 -machine virt -m 128M -nographic -serial mon:stdio -kernel kernel -bios none
Hello World :)
[INFO] Threads initialized.
[INFO] UART initialized.
[INFO] PMM initializing...
[INFO] PMM initialized.
[INFO] VMM initializing...
[INFO] text:    0x80000060 -> 0x80006000
[INFO] rodata:  0x80006000 -> 0x80008000
[INFO] data:    0x80008000 -> 0x80009000
[INFO] bss:     0x80009108 -> 0x8000a000
[INFO] stack:   0x8000a000 -> 0x8000b000
[INFO] heap:    0x8000b000 -> 0x88000000
[INFO] uart:    0x10000000 -> 0x10001000
[INFO] kproc pagetable set.
[INFO] VMM initialized.
[INFO] malloc initializing...
[INFO] malloc initialized.
[INFO] Traps initializing...
[INFO] plic:    0xc000000 -> 0x10000000
[INFO] clint:   0x2000000 -> 0x2010000
[INFO] Traps initialized.
[INFO] tf 0x87fbdde0
[INFO] status 0x120
```