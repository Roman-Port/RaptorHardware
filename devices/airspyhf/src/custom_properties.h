#pragma once

#include "internal.h"
#include <raptorhw_custom_property.h>

class prop_version : public raptorhw_custom_property_impl {

public:
	prop_version(airspyhf_device_t* device);
	virtual bool read_string(char* result, size_t resultMax, size_t* written) override;

private:
	airspyhf_device_t* device;

};

class prop_preamp : public raptorhw_custom_property_impl {

public:
	prop_preamp(airspyhf_device_t* device);
	virtual bool write_int(int value) override;
	virtual bool read_int(int* result) override;

private:
	airspyhf_device_t* device;
	bool enabled;

};

class prop_agc_threshold : public raptorhw_custom_property_impl {

public:
	prop_agc_threshold(airspyhf_device_t* device);
	virtual bool write_int(int value) override;
	virtual bool read_int(int* result) override;

private:
	airspyhf_device_t* device;
	bool enabled;

};