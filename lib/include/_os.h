#ifndef _OS_H
#define _OS_H

#include <types.hpp>


#ifdef __cplusplus 
extern "C" {
#endif
uint32_t * can_cpuid();
uint32_t cpu_vendor();
uint32_t cpu_features_edx();
uint32_t cpu_features_ecx();
void halt();
void enable();
void disable();
#ifdef __cplusplus 
}
#endif 

// I wonder if c++11 syntax [[optimize("O0")]] would do the work
__attribute__((optimize("O0")))
inline void syscall(int code_eax, int ebx, int ecx, int edx, int esi, int edi)
{
	asm volatile(
	"mov %0, %%eax;"
	"mov %1, %%ebx;"
	"mov %2, %%ecx;"
	"mov %3, %%edx;"
	"mov %4, %%esi;"
	"mov %5, %%edi;"
	"int $80;"
    : // Idk where is the output
	: "g"(code_eax), "g"(ebx), "g"(ecx), "g"(edx), "g"(esi), "g"(edi)
    : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi");
}

#ifdef __cplusplus
#include "_list.h"
struct args_registers
{
	int ebx;
	int ecx;
	int edx;
	int esi;
	int edi;
};

template <class... Ts>
union args_list_union
{
    list<Ts...> l;
    args_registers ar;
};

template <class... Ts>
inline void syscall(int code_eax, Ts... args)
{
	args_list_union<Ts...> alu;
	alu.l = make_list(args...);
	syscall(code_eax, alu.ar.ebx, alu.ar.ecx, alu.ar.edx, alu.ar.esi, alu.ar.edi);
}
#endif

#if defined(__is_libk)

void term_clear(char bchar);
void term_init();
void term_putc(char c, uint8_t color);
void term_print(const char* str, uint8_t color);

#endif

#ifdef __cplusplus
	int test_cpp_lib(int a);
#endif

#endif