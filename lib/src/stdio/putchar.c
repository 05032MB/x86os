#include <stdio.h>
 
/*#if defined(__is_libk)
#include <_kmaster.h>
#endif*/
#include <_os.h>
 
int putchar(int ic) {
#if defined(__is_libk)
	char c = (char) ic;
	//terminal_write(&c, sizeof(c));
	term_putc(c, 0x0F);
#else
	// TODO: Implement stdio and the write system call.
#endif
	return ic;
}