#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <vga.hpp>

enum{

	LOG_MINOR=VGA_COLOR_LIGHT_GREY,
	LOG_NORMAL=VGA_COLOR_WHITE,
	LOG_CRITICAL=VGA_COLOR_LIGHT_RED,
	LOG_WARNING=VGA_COLOR_MAGENTA,
	LOG_OK=VGA_COLOR_GREEN

};

void term_log_internal(const char * str, unsigned logtype, char prefix = '>');
void term_log(const char * str, unsigned logtype);
void term_log(const char * str, int value, unsigned logtype);

#endif