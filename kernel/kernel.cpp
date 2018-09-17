#include <_kmaster.hpp>

#include "../lib/include/stdlib.h"
#include <stdio.h>


  #define ARRSIZE(x)  (sizeof(x) / sizeof((x)[0]))
  
#include "ports.hpp"
#include "input.hpp"
 
// First, let's do some basic checks to make sure we are using our x86-elf cross-compiler correctly
#if defined(__linux__)
	#error "This code must be compiled with a cross-compiler"
#elif !defined(__i386__)
	#error "This code must be compiled with an x86-elf compiler"
#endif

#if __STDC_HOSTED__ == 1
	#warning "This code should be compiled with gcc -ffreestanding option"
#endif
 

 
// This is the x86's VGA textmode buffer. To display text, we write data to this memory location
volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;
// By default, the VGA textmode buffer has a size of 80x25 characters
const int VGA_COLS = 80;
const int VGA_ROWS = 25;

bool scrolling = 1; //do we want to "scroll?"

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};
 
// We start displaying text in the top-left of the screen (column = 0, row = 0)
int term_col = 0;
int term_row = 0;
uint8_t term_color = 0x0F; // Black background, White foreground
 
class terminal{


};

void term_clear(char bchar)
{
	// Clear the textmode buffer
	for (int col = 0; col < VGA_COLS; col ++)
	{
		for (int row = 0; row < VGA_ROWS; row ++)
		{
			// The VGA textmode buffer has size (VGA_COLS * VGA_ROWS).
			// Given this, we find an index into the buffer for our character
			const size_t index = (VGA_COLS * row) + col;
			// Entries in the VGA buffer take the binary form BBBBFFFFCCCCCCCC, where:
			// - B is the background color
			// - F is the foreground color
			// - C is the ASCII character
			vga_buffer[index] = ((uint16_t)term_color << 8) | /*'.'*/ bchar;
		}
	}
}

void term_init()
{
	term_clear('.');
}
void term_rewind()
{
	for (int i = 0; i < VGA_COLS*VGA_ROWS - VGA_COLS; i ++)
	{
		vga_buffer[i] = vga_buffer[i+VGA_COLS];
	}
	for(int i = VGA_COLS*VGA_ROWS - VGA_COLS; i< VGA_COLS*VGA_ROWS; i++)vga_buffer[i] = 0;
	
	
}
void term_putc(char c, uint8_t color = 0x0F)
{
	switch (c)
	{
	case '\n': 
		{
			term_col = 0;
			term_row ++;
			break;
		}
	case '\f':
		{
			term_col=0;
			term_row=0;
			break;
		}
	case '\b':
		{
			term_col--;
			if(term_col < 0){
				term_col = 0;
				term_row --;
			}
			break;
		}
 
	default: 
		{
			const size_t index = (VGA_COLS * term_row) + term_col; 
			vga_buffer[index] = ((uint16_t)color << 8) | c;
			term_col ++;
			break;
		}
	}
 
	if (term_col >= VGA_COLS)
	{
		term_col = 0;
		term_row ++;
	}
	
	if(term_row <0)
	{
		term_row = 0;
	}
 
	// What happens if we get past the last row? We need to reset both column and row to 0 in order to loop back to the top of the screen
	if (term_row >= VGA_ROWS)
	{
		if(scrolling){
			term_rewind();
			term_row--;
		
		}else{
			term_row = 0;
		}
	term_col = 0;

	}
}
 
void term_print(const char* str, uint8_t color = term_color)
{
	for (size_t i = 0; str[i] != '\0'; i ++) 
		term_putc(str[i], color);
}

//__attribute__((optimize("-O0")))
 void triple_fault()
 {
	//_push(10);
	asm("INT $101");//9 is good
 }
 
extern "C" void halt();

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
	//asm("INT $102");
 
	term_print("Hello, World!\n", VGA_COLOR_GREEN);
	term_print("Welcome to the kernel.\n", VGA_COLOR_CYAN << 4);
	
	term_print("Boot completed\n");
	
	
	term_print("\nPress any key (3 to triple fault)\n");
	while(1){
	char c = get();
		term_putc(c);
		if(c=='3')triple_fault();
		//char c = getScancode();
		//term_putc(c);
	}
	
}