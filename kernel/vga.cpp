#include <vga.hpp>

// This is the x86's VGA textmode buffer. To display text, we write data to this memory location
volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;
// By default, the VGA textmode buffer has a size of 80x25 characters
const int VGA_COLS = 80;
const int VGA_ROWS = 25;

bool scrolling = 1; //do we want to "scroll?"
 
// We start displaying text in the top-left of the screen (column = 0, row = 0)
int term_col = 0;
int term_row = 0;
byte term_color = 0x0F; // Black background, White foreground
 
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
void term_putc(char c)
{
	term_putc(c, term_color);
}
void term_putc(char c, uint8_t color)
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
 
void term_print(const char* str)
{
	term_print(str, term_color);
}
void term_print(const char* str, byte color)
{
	for (size_t i = 0; str[i] != '\0'; i ++) 
		term_putc(str[i], color);
}
void term_print_num(int num, unsigned base)
{
	char dl[30];
	itoa(num ,dl, base);
	term_print(dl);
}
void term_print_dec(int num)
{
	term_print_num(num, 10);
}
void term_print_hex(int num)
{
	term_print_num(num, 16);
}