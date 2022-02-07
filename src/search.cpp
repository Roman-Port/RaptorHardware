#include "defines.h"
#include <stdlib.h>
#include <cassert>

#include "devices/rtlsdr/include.h"
#include "devices/airspy/include.h"

int raptorhw_search(raptorhw_candidate_t** candidates, int max) {
	//Since we'll end this will a null terminator, make sure it is at least 1 element long
	if (max <= 1)
		return 0;

	//Search all device classes
	int count = 0;
	count += raptorhw_rtlsdr_search(&candidates[count], max - 1 - count);
	count += raptorhw_airspy_search(&candidates[count], max - 1 - count);

	//Finally, set null terminator at the end of the candidates collection
	assert(count < max);
	candidates[count] = 0;

	return count;
}

void raptorhw_search_delete(raptorhw_candidate_t** candidates) {
	//Delete all items until we hit the null terminator
	for (int i = 0; candidates[i] != 0; i++) {
		delete (candidates[i]);
		candidates[i] = 0;
	}
}