#include <logger.hpp>


void term_log_internal(const char * str, unsigned logtype, char prefix)
{
	if(prefix != 0)term_putc(prefix, logtype);
	term_print(str, logtype);
}
void term_log(const char * str, unsigned logtype)
{
	term_log_internal(str, logtype, '>');
	term_print("\n");
}
void term_log(const char * str, int val, unsigned logtype)
{
	term_log_internal(str, logtype, '>');
	term_print_dec(val, logtype);
	term_print("\n");
}
