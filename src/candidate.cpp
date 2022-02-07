#include "defines.h"

const char* raptorhw_candidate_get_name(raptorhw_candidate_t* candidate) {
	return candidate->get_name();
}

const char* raptorhw_candidate_get_serial(raptorhw_candidate_t* candidate) {
	return candidate->get_serial();
}

int raptorhw_candidate_get_capabilities(raptorhw_candidate_t* candidate) {
	return candidate->get_capabilities();
}

raptorhw_instance_t* raptorhw_candidate_create_instance(raptorhw_candidate_t* candidate) {
	return candidate->create_instance();
}