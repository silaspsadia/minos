#include <string.h>

char *strncpy(char *to, const char *from, size_t len) {
	int i = 0;
	while ((to[i] = from[i]) && i < len)
		i++;
	return to[i];	
}
