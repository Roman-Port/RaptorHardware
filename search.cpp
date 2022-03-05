#include "context.h"
#include <cstring>
#include <cassert>

struct search_context_t {
	raptorhw_candidate_impl** buffer;
	int capacity;
	int index;
};

void resize_search_buffer(search_context_t* ctx, int capacity) {
	//Create the new buffer
	raptorhw_candidate_impl** buffer = (raptorhw_candidate_impl**)malloc(sizeof(raptorhw_candidate_impl*) * (capacity + 1));
	assert(buffer != 0);

	//Copy from old buffer
	if (ctx->buffer != 0 && capacity > ctx->capacity) {
		memcpy(buffer, ctx->buffer, sizeof(raptorhw_candidate_impl*) * ctx->capacity);
		free(ctx->buffer);
	}	

	//Apply
	ctx->buffer = buffer;
	ctx->capacity = capacity;
}

void search_callback(void* ctx, raptorhw_candidate_impl* candidate) {
	//Get the "real" context
	search_context_t* search = (search_context_t*)ctx;

	//Make sure there's space
	while (search->index >= search->capacity)
		resize_search_buffer(search, search->capacity * 2);

	//Add
	search->buffer[search->index++] = candidate;
}

raptorhw_candidate_impl** raptorhw_context_impl::search() {
	//Create and initialize search context
	search_context_t search;
	memset(&search, 0, sizeof(search_context_t));
	resize_search_buffer(&search, 8);

	//Enumerate modules and search
	for (int i = 0; i < modules_count; i++)
		modules[i]->search(search_callback, &search);

	//Add a null terminator to the end of the list
	search.buffer[search.index] = 0;

	return search.buffer;
}

void raptorhw_context_impl::search_destroy(raptorhw_candidate_impl** candidates) {
	//Dispose of all candidates
	for (int i = 0; candidates[i] != 0; i++)
		delete candidates[i];

	//Free the list
	free(candidates);
}