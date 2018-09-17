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

#if defined(__is_libk)

void term_clear(char bchar);
void term_init();
void term_putc(char c, uint8_t color);
void term_print(const char* str, uint8_t color);

#endif


#endif