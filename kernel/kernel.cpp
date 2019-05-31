#include <stddef.h>
#include <stdint.h>
#include <types.hpp>
#include <interrupts.hpp>
#include <kmemory.hpp>
#include <gdt.hpp>
#include <paging.hpp>

#include <stdlib.h>
#include <stdio.h>


  #define ARRSIZE(x)  (sizeof(x) / sizeof((x)[0]))
 
#include <input.hpp>
#include <vga.hpp>
#include <syscalls.hpp>


//Assembler functions

extern "C" 
{
void halt();
void _cli();
void _sti();
void _nop();
void init_idt();
void _lets_err();
void __attribute__((fastcall))switch_to_ring_3(void(*)());
void __attribute__((fastcall))switch_to_ring_0();
}

#include <multiboot.hpp>
#include <vfs.hpp>
#include <bfs.hpp>
#include <elf.hpp>
#include <logger.hpp>
#include <mtracker.hpp>
#include <task.hpp>
 
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

extern "C" void kernel_main(multiboot_info_t *mbinfo)
{
	term_init();

	term_log("Kernel ends at: " , to_addr_t(&endkernel) ,LOG_MINOR);

	term_print("VGA display ready\n");
	//asm("hlt");
	term_print("Found GRUB modules: ");
	term_print_dec(mbinfo->mods_count);
	
	unsigned short modsnum  = mbinfo->mods_count; 
	
	dword trav_helper = mbinfo->mods_addr;
	
	for(decltype(modsnum) i = 0; i < modsnum; i++){ //counts grub modules
	
		dword modbeg = *reinterpret_cast<dword*>(trav_helper) ;
		dword modend = *reinterpret_cast<dword*>(trav_helper+4);
		
		endkernel = reinterpret_cast<void*>(modend);
		
		term_print("\n->Module: ",VGA_COLOR_LIGHT_GREY );
		term_print_dec(i,VGA_COLOR_LIGHT_GREY );
		term_print(",beg=",VGA_COLOR_LIGHT_GREY );
		term_print_dec(modbeg,VGA_COLOR_LIGHT_GREY );
		term_print(",end=",VGA_COLOR_LIGHT_GREY );
		term_print_dec(modend,VGA_COLOR_LIGHT_GREY );
		term_print(",name=",VGA_COLOR_LIGHT_GREY );
		term_print(reinterpret_cast<const char*>(trav_helper+8),VGA_COLOR_LIGHT_GREY );
		term_print(",found and taken into account",VGA_COLOR_LIGHT_GREY );
		
		trav_helper += 16;
   
	}
	
	
	init_gdt();
	term_print("\nMemory segmented\n");
	init_interrupts(); //add support for APIC if available
	term_print("Interrupts are configured\n");
	init_paging();
	term_print("Paging is ready\n");
	init_heap();
	term_print("Sysheap ready\n");
	
	init_paging_phase_2();
	
	init_syscalls();
	term_print("System calls initialized\n");
	
	init_vfs();
	term_print("VFS initialized\n");
	
	first_fs = init_initrd(*reinterpret_cast<dword*>(mbinfo->mods_addr));
	term_print("Ramdisk ready\n");
	
	
	f_node* noder = fs_findindir(first_fs , static_cast<char*>("Kat002") );  //działa
	//term_print("\n");
	//term_print_dec((dword)noder); 
	//term_print("\n");
	//term_print(noder->oid); term_print(":");
	//term_print_dec(noder->inode);
	auto btstrp = ELF::load_elf(reinterpret_cast< ELF::ELF32_Header* >( *reinterpret_cast<dword*> (  mbinfo->mods_addr +16  ) ) );

	only_task.prepare_task(nullptr, nullptr, to_addr_t(btstrp), -1);

	term_print("Hello, World!\n", VGA_COLOR_GREEN);
	term_print("Welcome to the kernel.\n", VGA_COLOR_CYAN << 4);
	
	term_print("Boot completed\n");
	term_print("----------------------\n",VGA_COLOR_MAGENTA);
	
	only_task.launch_task();
	//switch_to_ring_3((void(*)())btstrp/*_lets_err*/);
	//switch_to_ring_0();
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