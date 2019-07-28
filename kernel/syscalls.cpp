#include <syscalls.hpp>
#include <input.hpp>
#include <overseer.hpp>
#include <logger.hpp>

__ASM_IMPORT{
	 __fastcall void _syscall_exec(const int_iden); //pewnie nie istnieje
}

void* syscalls[SYSCALL_MAX];
 
void test_syscall(char, byte)
{
	halt();
}

void init_syscalls()
{
	register_interrupt_handler(80, syscall_handler);
	
	//void *test = (void*)static_cast<void (*) (char, byte)>(&test_syscall);
	//term_print_dec((int)test);

	register_syscall(1,  (void*)static_cast<void (*) (char, byte)>(&term_putc));
	register_syscall(2,  (void*)static_cast<void (*) (const char*, byte)>(&term_print));
	register_syscall(3,  (void*)static_cast<void (*) (int)>(&term_print_hex));
	register_syscall(8,  (void*)static_cast<void (*) (char *)>(&get_wrapper));
	register_syscall(10,  (void*)static_cast<void (*) (byte)>(&sched_manager::exit_call));
}


bool register_syscall(int num, void *func)
{
	if(num >= SYSCALL_MAX)return false;
	
	syscalls[num] = func;
	return true;
}

__ASM_IMPORT void halt();

__nooptimize
void syscall_handler(const int_iden ii)
{

	term_log("SYSCALL NOW!", ii.eax, LOG_MINOR);

	if(ii.eax >= SYSCALL_MAX)return;
	
	void* jmploc = syscalls[ii.eax];
	if(to_addr_t(jmploc) == 0)return;
	
	int val;

	
 asm volatile (" \
     push %1; \
     push %2; \
     push %3; \
     push %4; \
     push %5; \
	 call *%6; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
	 " : "=a" (val) : "r" (ii.edi), "r" (ii.esi), "r" (ii.edx), "r" (ii.ecx), "r" (ii.ebx), "r" (jmploc) );

	
}