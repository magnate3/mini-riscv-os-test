#include "os.h"

void user_task0(void)
{
	uart_puts("Task0: Created!\n");
	uart_puts("Task0: Now, return to kernel mode\n");
	os_kernel();
	while (1) {
		uart_puts("Task0: Running...\n");
		lib_delay(1000);
		os_kernel();
	}
}

void user_task1(void)
{
	uart_puts("Task1: Created!\n");
	uart_puts("Task1: Now, return to kernel mode\n");
	os_kernel();
	while (1) {
		uart_puts("Task1: Running...\n");
		lib_delay(1000);
		os_kernel();
	}
}

void user_init() {
	task_create(&user_task0);
	task_create(&user_task1);
}
