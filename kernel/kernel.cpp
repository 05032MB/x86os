#include <_kmaster.hpp>

 
// First, let's do some basic checks to make sure we are using our x86-elf cross-compiler correctly
#if defined(__linux__)
	#error "This code must be compiled with a cross-compiler"
#elif !defined(__i386__)
	#error "This code must be compiled with an x86-elf compiler"
#endif

#if __STDC_HOSTED__ == 1
	#warning "This code should be compiled with gcc -ffreestanding option"
#endif
 



//__attribute__((optimize("-O0")))
 void triple_fault()
 {
	//_push(10);
	asm("INT $101");//9 is good
 }
 void page_fault()
 {
	int * pointerofdoom = (int*)0xFFFFFFF;
	*pointerofdoom = 23;
 
 }
 

#include <stdlib.h>

extern "C" void kernel_main()
{
	term_init();
	term_print("VGA display ready\n");

	init_gdt();
	term_print("Memory segmented\n");
	init_interrupts(); //add support for APIC if available
	term_print("Interrupts are configured\n");
	init_paging();
	term_print("Paging is ready\n");

	init_heap();
	term_print("Sysheap ready\n");
	
	init_paging_phase_2();
	
	init_syscalls();
	term_print("System calls initialized\n");
	
	term_print("Hello, World!\n", VGA_COLOR_GREEN);
	term_print("Welcome to the kernel.\n", VGA_COLOR_CYAN << 4);
	
	term_print("Boot completed\n");
	term_print("----------------------\n",VGA_COLOR_MAGENTA);
	
	switch_to_ring_3(_lets_err);
	term_print("\nSuccessfully tested ring3");
	
	term_print("\nPress any key (3 to GP fault)\n");
	while(1){
	char c = get();
		term_putc(c);
		if(c=='3')triple_fault();
		if(c=='4')page_fault();
		//char c = getScancode();
		//term_putc(c);
	}
	
}