#include <string.h>

char *strncpy(const char *to, const char *from, size_t len) {
	i = 0;
	while ((to[i] = from[i]) && i < len)
		i++;
	return to[i];	
}
