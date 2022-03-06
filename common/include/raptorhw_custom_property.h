#pragma once

#include <stdint.h>
#include <stdio.h>

class raptorhw_custom_property_impl {

public:
	raptorhw_custom_property_impl(const char* name, int flags);
	virtual ~raptorhw_custom_property_impl();

	size_t get_name(char* result, size_t resultMax);
	int get_flags();

	virtual bool read_string(char* result, size_t resultMax, size_t* written);
	virtual bool write_string(char* input, size_t len);

	virtual bool read_int(int* result);
	virtual bool write_int(int value);

private:
	const char* name;
	int flags;

};