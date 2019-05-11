#include <input.hpp>
#include <types.hpp>
#include <ports.hpp>
#include <interrupts.hpp>
#include <logger.hpp>

#define ARRSIZE(x)  (sizeof(x) / sizeof((x)[0]))

uint8_t ps2_keyboard_get() //get raw keyboard input
{
	return inb(0x60); //reads from data port, see "8042" PS/2 Controller
}
char ps2_keyboard_resolve(uint8_t c)
{
	if(c>ARRSIZE(set1_map))return 0;
	return set1_map[c];
}
char ps2_keyboard_listen()
{
	while(!(inb(0x64) & 1));//control bit is not set
	return ps2_keyboard_get();
}
char get()
{
	char ret;
	do{
		ret = ps2_keyboard_resolve(ps2_keyboard_listen());
	}while(!ret);

	return ret;
}

void get_wrapper(char * c)
{
	_sti(); //to nie skończy się dobrze
 	char r = get();
 	term_log("Read from keyboard: ", r, LOG_MINOR);
 	*c = r;
 	_cli();
}

#undef ARRSIZE