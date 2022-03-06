#include "internal.h"
#include "candidate.h"
#include <raptorhw_module.h>
#include <raptorhw_util.h>
#include <raptorhw_device_airspyhf.h>

#define AIRSPY_MAX_DEVICES 16

class airspyhf_module : public raptorhw_module_impl {

	virtual size_t get_name(char* result, size_t resultSize) override
	{
		return raptorhw_strcpy(result, "AirSpy HF+", resultSize);
	}

	virtual void search(raptorhw_module_impl_search_cb callback, void* callbackCtx) override
	{
		//Request serials
		uint64_t serials[AIRSPY_MAX_DEVICES];
		int count = airspyhf_list_devices(serials, AIRSPY_MAX_DEVICES);

		//Create devices
		for (int i = 0; i < count; i++)
			callback(callbackCtx, new airspyhf_candidate(serials[i]));
	}

};

raptorhw_module_impl* raptorhw_device_airspyhf_create() {
	return new airspyhf_module();
}