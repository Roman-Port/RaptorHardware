#include <raptorhw_util.h>
#include <cstring>

size_t raptorhw_strcpy(char* dst, const char* src, size_t max) {
	//Calculate size of source
	size_t len = strlen(src);
	if (len > max - 1)
		len = max - 1;

	//Copy
	memcpy(dst, src, len);

	//Set null terminator
	dst[len] = 0;

	return len;
}