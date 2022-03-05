#pragma once
#include <raptorhw_device.h>
#include <stdio.h>

class raptorhw_candidate_impl {

public:
	virtual ~raptorhw_candidate_impl();
	virtual size_t get_name(char* result, size_t resultMax) = 0;
	virtual size_t get_serial(char* result, size_t resultMax) = 0;
	virtual int get_capabilities() = 0;
	virtual raptorhw_device_impl* open() = 0;

};