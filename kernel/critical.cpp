#include <critical.hpp>
#include <types.hpp>
#include <stdio.h>

__ASM_IMPORT{
void _cli();
void _sti();
void halt();
}

__nooptimize __noreturn
void kpanic(const char *s)
{	
	_cli();
	printf("Kernel Panic!: ");
	puts(s);
	puts("System is aborting; manually restart machine");
	halt();
	while(1);
	__builtin_unreachable();

}