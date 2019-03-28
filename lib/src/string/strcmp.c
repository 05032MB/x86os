#include <string.h>

int strcmp(const char *s1, const char *s2)
{
	/*unsigned i;
	for(i=0; s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0'; i++);
	if(s1[i] == s2[i] == '\0')return 0;
	if(s1[i] < s2[i]) return -1;
	if(s1[i] > s2[i]) return 1;*/
    while(*s1 && (*s1==*s2))
        s1++,s2++;
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}