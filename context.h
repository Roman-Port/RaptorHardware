#pragma once

#include <raptorhw.h>
#include <raptorhw_module.h>

#define RAPTORHW_MAX_MODULES 16

class raptorhw_context_impl : public raptorhw_context_t {

public:
	raptorhw_context_impl();
	void initialize_modules();
	void add_module(raptorhw_module_impl* mod);

	raptorhw_candidate_impl** search();
	void search_destroy(raptorhw_candidate_impl** candidates);

	raptorhw_module_impl* modules[RAPTORHW_MAX_MODULES];
	int modules_count;

};