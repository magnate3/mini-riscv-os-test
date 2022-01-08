#include "os.h"
#include "init.h"

int os_main(void)
{
        lib_printf("OS start\n");
        //timer_init(); // start timer interrupt ...
	//enter_machine_mode(machine_mode_fn, 0, 0);
	enter_supervisor_mode(supervisor_mode_fn, 99, 199);
	while(1) {}
	//while (1) {uart_puts("OS idle\n");} // os : do nothing, just loop!
	return 0;
}

