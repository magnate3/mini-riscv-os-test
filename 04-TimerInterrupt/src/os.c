#include "os.h"

int os_main(void)
{
	 uart_puts("OS start\n");
	timer_init(); // start timer interrupt ...
	while (1) {} // os : do nothing, just loop!
	return 0;
}

