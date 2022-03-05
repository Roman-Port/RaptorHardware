#include <cassert>
#include "context.h"

raptorhw_context_impl::raptorhw_context_impl() {
	//Clear modules list
	modules_count = 0;
	for (int i = 0; i < RAPTORHW_MAX_MODULES; i++)
		modules[i] = 0;

	//Initialize modules
	initialize_modules();
}

void raptorhw_context_impl::add_module(raptorhw_module_impl* mod) {
	assert(modules_count < RAPTORHW_MAX_MODULES);
	modules[modules_count++] = mod;
}