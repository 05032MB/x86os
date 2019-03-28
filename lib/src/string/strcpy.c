#include <string.h>

char* strcpy(char* dest, const char* src)
{
	char* cpy = dest;

	do{
		*dest = *src;
		dest++; 
	}while(*src++ != '\0');

	return cpy;
}