#include "gain.h"
#include <raptorhw_util.h>

class hf_attenuation : public raptorhw_gain_option_impl {

public:
	hf_attenuation(airspyhf_device_t* device) : device(device), index(0), agc_on(true) {

	}

	virtual size_t get_name(char* result, size_t resultSize) override
	{
		return raptorhw_strcpy(result, "HF Attenuation", resultSize);
	}

	virtual int get_unit() override
	{
		return RAPTORHW_UNITS_DB;
	}

	virtual bool get_agc_supported() override
	{
		return true;
	}

	virtual bool get_agc() override
	{
		return agc_on;
	}

	virtual bool set_agc(bool enabled) override
	{
		//Set the value
		if (airspyhf_set_hf_agc(device, enabled) != AIRSPYHF_SUCCESS)
			return false;

		//Set internally
		agc_on = enabled;
		return true;
	}

	virtual int get_option_count() override
	{
		return 9;
	}

	virtual float get_option_value(int index) override
	{
		return 6 * index; //6 dB steps
	}

	virtual int get_value() override
	{
		return index;
	}

	virtual bool set_value(int index) override
	{
		//Turn off AGC if it's on
		if (agc_on && !set_agc(false))
			return false;

		//Set the value
		if (airspyhf_set_hf_att(device, index) != AIRSPYHF_SUCCESS)
			return false;

		//Set internally
		this->index = index;
		return true;
	}

private:
	airspyhf_device_t* device;
	int index;
	bool agc_on;

};

raptor_airspyhf_gain::raptor_airspyhf_gain(airspyhf_device_t* device) {
	put_option(new hf_attenuation(device));
}

size_t raptor_airspyhf_gain::get_name(char* result, size_t resultSize)
{
	return raptorhw_strcpy(result, "Default", resultSize);
}
