#pragma once

#include "internal.h"
#include <raptorhw_gain.h>

class raptor_airspyhf_gain : public raptorhw_gain_group_impl {

public:
	raptor_airspyhf_gain(airspyhf_device_t* device);
	virtual size_t get_name(char* result, size_t resultSize) override;

};