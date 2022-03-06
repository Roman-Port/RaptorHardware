#include "custom_properties.h"
#include <raptorhw_device.h>
#include <cstring>

// VERSION

prop_version::prop_version(airspyhf_device_t* device) : raptorhw_custom_property_impl("Firmware Version", RAPTORHW_CUSTOMPROP_STRING), device(device) {

}

bool prop_version::read_string(char* result, size_t resultMax, size_t* written) {
	//Limit the value size to 255 since it's going to be as a uint8...for some reason
	uint8_t resultLen = (resultMax > 255 ? 255 : resultMax);

	//Read
	if (airspyhf_version_string_read(device, result, resultLen) == AIRSPYHF_SUCCESS) {
		(*written) = strlen(result);
		return true;
	}
	return false;
}

//HF PREAMP

prop_preamp::prop_preamp(airspyhf_device_t* device) : raptorhw_custom_property_impl("HF Preamp", RAPTORHW_CUSTOMPROP_WRITABLE | RAPTORHW_CUSTOMPROP_BOOL | RAPTORHW_CUSTOMPROP_INT), device(device), enabled(true) {

}

bool prop_preamp::write_int(int value) {
	if (airspyhf_set_hf_lna(device, value) != AIRSPYHF_SUCCESS)
		return false;
	enabled = value;
	return true;
}

bool prop_preamp::read_int(int* result) {
	(*result) = enabled;
	return true;
}

//HF AGC THRESHOLD

prop_agc_threshold::prop_agc_threshold(airspyhf_device_t* device) : raptorhw_custom_property_impl("HF AGC Threshold", RAPTORHW_CUSTOMPROP_WRITABLE | RAPTORHW_CUSTOMPROP_BOOL | RAPTORHW_CUSTOMPROP_INT), device(device), enabled(true) {

}

bool prop_agc_threshold::write_int(int value) {
	if (airspyhf_set_hf_agc_threshold(device, value) != AIRSPYHF_SUCCESS)
		return false;
	enabled = value;
	return true;
}

bool prop_agc_threshold::read_int(int* result) {
	(*result) = enabled;
	return true;
}