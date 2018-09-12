#include <critical.hpp>

__attribute__((noinline, noreturn))
void kpanic(const char *s)
{	
	_cli();
	printf("Kernel Panic!: ");
	puts(s);
	puts("System is aborting; manually restart machine");
	halt();
	__builtin_unreachable();

}