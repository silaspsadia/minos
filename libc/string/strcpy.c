#include <string.h>

char *strcpy(char *to, const char *from) {
	int i = 0;
	while((to[i] = from[i]))
		i++;
	return to[i];
}
