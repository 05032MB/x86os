#ifndef _VGA_H
#define _VGA_H

#include <ports.hpp>
#include <stddef.h>
#include <stdint.h>
#include <types.hpp>
#include <stdlib.h>

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

void term_clear(char bchar);
void term_init();
void term_putc(char c, byte color);
void term_putc(char c);
void term_print(const char* str, byte color);
void term_print(const char* str);
void term_print_num(int, unsigned);
void term_print_dec(int);
void term_print_hex(int);


extern volatile uint16_t* vga_buffer;
extern const int VGA_COLS ;
extern const int VGA_ROWS ;

extern bool scrolling;
 
extern int term_col;
extern int term_row ;
extern byte term_color;

#endif