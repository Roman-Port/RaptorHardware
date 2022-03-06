#include "candidate.h"
#include "device.h"
#include <raptorhw_util.h>

airspyhf_candidate::airspyhf_candidate(uint64_t serial) : serial(serial) {

}

size_t airspyhf_candidate::get_name(char* result, size_t resultMax) {
	return raptorhw_strcpy(result, "AirSpy HF+", resultMax);
}

size_t airspyhf_candidate::get_serial(char* result, size_t resultMax) {
	return snprintf(result, resultMax, "%llx", serial);
}

int airspyhf_candidate::get_capabilities() {
	return AIRSPYHF_CAPABILITIES;
}

raptorhw_device_impl* airspyhf_candidate::open() {
	//Open the device
	airspyhf_device_t* device;
	if (airspyhf_open_sn(&device, serial) != 0)
		return 0;

	//Wrap
	return new raptor_airspyhf_device(serial, device);
}