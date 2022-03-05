#pragma once

#include <raptorhw_candidate.h>

typedef void (*raptorhw_module_impl_search_cb)(void* ctx, raptorhw_candidate_impl* candidate);

class raptorhw_module_impl {

public:
	virtual size_t get_name(char* result, size_t resultSize) = 0;
	virtual void search(raptorhw_module_impl_search_cb callback, void* callbackCtx) = 0;

};