#include <stdlib.h>

static void swap(char *a, char *b)
{
	char p = *b;
	*b = *a;
	*a = p;
}
static void reverse(char *str, size_t len)
{
	size_t p1 = 0;
	size_t p2 = len-1;
	
	while(p1<p2)
		swap(&str[p1++], &str[p2--]);
}

char *itoa (int value, char * str, int base)
{
	bool neg = 0;

	if(value ==0){str[0] = '0';str[1]= '\0';return str;}
	if(value < 0 && base == 10){neg = 1;value *= -1;}
	
	size_t i = 0;
	while(value != 0)
	{
		int code = value % base;
		str[i++] = (code > 9)? code-10 + 'A' : code + '0';
		value /= base;
	}
	if(neg)str[i++] = '-';
	str[i] = '\0';
	reverse(str, i);
	
	return str;
	
}