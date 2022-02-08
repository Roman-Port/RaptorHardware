#include "include.h"
#include <airspy.h>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>

#define AIRSPY_SN_LENGTH 32
#define AIRSPY_CAPABILITIES RAPTORHW_CAPABILITY_BIAS_T

/* GAIN ITEMS */

typedef int (*airspy_gain_cb)(airspy_device* device, uint8_t value);
class airspy_gain : public raptorhw_gain_t {

public:
	airspy_gain(airspy_device* device, const char* name, airspy_gain_cb func, int maxValue) :
		device(device),
		name(name),
		func(func),
		max_value(maxValue),
		value(0)
	{

	}

	virtual const char* get_name() override {
		return name;
	}
	virtual bool get_agc_supported() override {
		return false;
	}

	virtual bool get_agc() override {
		return false;
	}
	virtual bool set_agc(bool enabled) override {
		return false;
	}

	virtual int get_value_count() override {
		return max_value + 1;
	}
	virtual void get_value_label(int index, char* result, size_t resultLen) override {
		sprintf_s(result, resultLen, "%i", index);
	}

	virtual int get_value() override {
		return value;
	}
	virtual bool set_value(int index) override {
		if (func(device, index) != 0)
			return false;
		value = index;
		return true;
	}

protected:
	airspy_device* device;

private:
	const char* name;
	airspy_gain_cb func;
	int max_value;
	int value;

};

class airspy_gain_agc : public airspy_gain {
public:
	airspy_gain_agc(airspy_device* device, const char* name, airspy_gain_cb func, int maxValue, airspy_gain_cb agcFunc) :
		airspy_gain(device, name, func, maxValue),
		agc_func(agcFunc),
		agc_enabled(false)
	{

	}

	virtual bool get_agc() override {
		return agc_enabled;
	}
	virtual bool set_agc(bool enabled) override {
		if (agc_func(device, enabled) != 0)
			return false;
		agc_enabled = enabled;
		return true;
	}

	virtual bool set_value(int index) override {
		if (agc_enabled)
			set_agc(false);
		return airspy_gain::set_value(index);
	}

private:
	airspy_gain_cb agc_func;
	bool agc_enabled;

};

/* GAIN GROUPS */

class airspy_gain_group_sensitive : public raptorhw_gain_group_t {

public:
	airspy_gain_group_sensitive(airspy_device* device) :
		item(device, "Sensitivity", airspy_set_sensitivity_gain, 21)
	{

	}
	virtual const char* get_name() override {
		return "Sensitive";
	}
	virtual int get_setting_count() override {
		return 1;
	}
	virtual raptorhw_gain_t* get_setting(int count) override {
		switch (count) {
		case 0: return &item;
		}
		return 0;
	}
	virtual bool select() override {
		return true;
	}

private:
	airspy_gain item;

};

class airspy_gain_group_linear : public raptorhw_gain_group_t {

public:
	airspy_gain_group_linear(airspy_device* device) :
		item(device, "Linearity", airspy_set_linearity_gain, 21)
	{

	}
	virtual const char* get_name() override {
		return "Linear";
	}
	virtual int get_setting_count() override {
		return 1;
	}
	virtual raptorhw_gain_t* get_setting(int count) override {
		switch (count) {
		case 0: return &item;
		}
		return 0;
	}
	virtual bool select() override {
		return true;
	}

private:
	airspy_gain item;

};

class airspy_gain_group_free : public raptorhw_gain_group_t {

public:
	airspy_gain_group_free(airspy_device* device) :
		lna(device, "LNA", airspy_set_lna_gain, 15, airspy_set_lna_agc),
		mixer(device, "Mixer", airspy_set_mixer_gain, 15, airspy_set_mixer_agc),
		vga(device, "VGA", airspy_set_vga_gain, 15)
	{

	}
	virtual const char* get_name() override {
		return "Free";
	}
	virtual int get_setting_count() override {
		return 3;
	}
	virtual raptorhw_gain_t* get_setting(int count) override {
		switch (count) {
		case 0: return &lna;
		case 1: return &mixer;
		case 2: return &vga;
		}
		return 0;
	}
	virtual bool select() override {
		return true;
	}

private:
	airspy_gain_agc lna;
	airspy_gain_agc mixer;
	airspy_gain vga;

};

/* INSTANCE */

class airspy_instance_t : public raptorhw_instance_t {

public:
	airspy_instance_t(uint64_t serial, airspy_device* device) :
		serial(serial),
		device(device),
		center_freq(0),
		bias_t_enabled(false),
		sample_rate(0),
		gain_linear(device),
		gain_sensitive(device),
		gain_free(device),
		rx_callback(NULL),
		rx_ctx(NULL)
	{
		//Format serial number as string
		sprintf_s(serial_string, sizeof(serial_string), "%llx", serial);
	}

	~airspy_instance_t() {
		//Stop streaming if we are
		stop_rx();

		//Close the device handle
		airspy_close(device);

		//Set the device handle to NULL
		device = NULL;
	}

	virtual const char* get_name() override {
		return "AirSpy";
	}

	virtual const char* get_serial() override {
		return serial_string;
	}

	virtual int get_capabilities() override {
		return AIRSPY_CAPABILITIES;
	}

	virtual int64_t get_min_freq() override {
		return 24000000;
	}
	virtual int64_t get_max_freq() override {
		return 1750000000;
	}

	virtual int64_t get_freq() override {
		return center_freq;
	}
	virtual bool set_freq(int64_t value) override {
		if (airspy_set_freq(device, value) != 0)
			return false;
		center_freq = value;
		return true;
	}

	virtual bool get_bias_t() override {
		return bias_t_enabled;
	}
	virtual bool set_bias_t(bool enabled) override {
		if (airspy_set_rf_bias(device, enabled) != 0)
			return false;
		bias_t_enabled = enabled;
		return true;
	}

	virtual int get_supported_samplerates(int* result, int max) override {
		//If count is 0, return the number of items
		if (max == 0 || result == 0) {
			uint32_t temp;
			airspy_get_samplerates(device, &temp, 0);
			return temp;
		}

		//Query
		airspy_get_samplerates(device, (unsigned int*)result, max);

		return max;
	}
	virtual int get_samplerate() override {
		return sample_rate;
	}
	virtual bool set_samplerate(int rate) override {
		if (airspy_set_samplerate(device, rate) != 0)
			return false;
		sample_rate = rate;
		return true;
	}

	virtual int get_gain_group_count() override {
		return 3;
	}
	virtual raptorhw_gain_group_t* get_gain_group_at(int index) override {
		switch (index) {
		case 0: return &gain_linear;
		case 1: return &gain_sensitive;
		case 2: return &gain_free;
		}
		return 0;
	}

	virtual bool start_rx(raptorhw_instance_streaming_cb callback, void* ctx) override {
		//Make sure there isn't already an active session
		if (rx_callback != NULL)
			return false;

		//Sanity check
		if (callback == NULL)
			return false;

		//Set the arguments locally
		rx_callback = callback;
		rx_ctx = ctx;

		//Configure the device to use the correct type of samples
		if (airspy_set_sample_type(device, airspy_sample_type::AIRSPY_SAMPLE_FLOAT32_IQ) != 0)
			return false;

		//Begin streaming
		return airspy_start_rx(device, read_callback, this) == 0;
	}
	virtual void stop_rx() override {
		//Make sure that there is an active session
		if (rx_callback == NULL)
			return;

		//Stop the active thread
		if (airspy_stop_rx(device) != 0)
			return;

		//Clear out the internal arguments
		rx_callback = NULL;
		rx_ctx = NULL;
	}

private:
	airspy_device* device;
	uint64_t serial;
	char serial_string[AIRSPY_SN_LENGTH];

	int64_t sample_rate;
	int64_t center_freq;
	bool bias_t_enabled;

	airspy_gain_group_linear gain_linear;
	airspy_gain_group_sensitive gain_sensitive;
	airspy_gain_group_free gain_free;

	raptorhw_instance_streaming_cb rx_callback;
	void* rx_ctx;

	static int read_callback(airspy_transfer* transfer) {
		//Get the true context
		airspy_instance_t* ctx = (airspy_instance_t*)transfer->ctx;

		//Send
		ctx->rx_callback(ctx, ctx->rx_ctx, (raptorhw_complex_t*)transfer->samples, transfer->sample_count);

		return 0;
	}

};

class airspy_candidate_t : public raptorhw_candidate_t {

public:
	airspy_candidate_t(uint64_t serial) : serial(serial)
	{
		//Format serial number as string
		sprintf_s(serial_string, sizeof(serial_string), "%llx", serial);
	}

	virtual const char* get_name() override {
		return "AirSpy";
	}

	virtual const char* get_serial() override {
		return serial_string;
	}

	virtual int get_capabilities() override {
		return AIRSPY_CAPABILITIES;
	}

	virtual raptorhw_instance_t* create_instance() override {
		//Open the AirSpy device
		airspy_device* device;
		if (airspy_open_sn(&device, serial) != 0)
			return NULL;

		//Wrap the device
		return new airspy_instance_t(serial, device);
	}

private:
	uint64_t serial;
	char serial_string[AIRSPY_SN_LENGTH];

};

int raptorhw_airspy_search(raptorhw_candidate_t** candidates, int max) {
	//If there's no space left, abort
	if (max == 0)
		return 0;

	//Allocate array to store serials
	uint64_t* serials = (uint64_t*)malloc(sizeof(uint64_t) * max);
	assert(serials != 0);

	//Request list
	int count = airspy_list_devices(serials, max);

	//Enumerate and create devices
	int i;
	for (i = 0; i < count && i < max; i++)
		candidates[i] = new airspy_candidate_t(serials[i]);

	return i;
}