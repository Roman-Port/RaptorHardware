#include "gain_macros.h"
#include "../lib/libairspy/src/airspy.h"
#include <raptorhw_module.h>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <raptorhw_util.h>

#define AIRSPY_SN_LENGTH 32
#define AIRSPY_CAPABILITIES RAPTORHW_CAPABILITY_BIAS_T

/* GAINS */

CREATE_GAIN(airspy_gain_sensitivity, "Sensitivity", airspy_set_sensitivity_gain, 21);

CREATE_GAINGROUP_START(airspy_gain_group_sensitive, "Sensitive")
CREATE_GAINGROUP_ADD(airspy_gain_sensitivity)
CREATE_GAINGROUP_END()

//

CREATE_GAIN(airspy_gain_linearity, "Linearity", airspy_set_linearity_gain, 21);

CREATE_GAINGROUP_START(airspy_gain_group_linear, "Linear")
CREATE_GAINGROUP_ADD(airspy_gain_linearity)
CREATE_GAINGROUP_END()

//

CREATE_GAIN_AGC(airspy_gain_lna, "LNA", airspy_set_lna_gain, airspy_set_lna_agc, 15);
CREATE_GAIN_AGC(airspy_gain_mixer, "Mixer", airspy_set_mixer_gain, airspy_set_mixer_agc, 15);
CREATE_GAIN(airspy_gain_vga, "VGA", airspy_set_vga_gain, 15);

CREATE_GAINGROUP_START(airspy_gain_group_free, "Free")
CREATE_GAINGROUP_ADD(airspy_gain_lna)
CREATE_GAINGROUP_ADD(airspy_gain_mixer)
CREATE_GAINGROUP_ADD(airspy_gain_vga)
CREATE_GAINGROUP_END()

/* INSTANCE */

class airspy_instance_t : public raptorhw_device_impl {

public:
	airspy_instance_t(uint64_t serial, airspy_device* device) :
		serial(serial),
		device(device),
		center_freq(0),
		bias_t_enabled(false),
		sample_rate(0),
		rx_callback(NULL),
		rx_ctx(NULL)
	{
		put_gains(new airspy_gain_group_free(device));
		put_gains(new airspy_gain_group_linear(device));
		put_gains(new airspy_gain_group_sensitive(device));
	}

	~airspy_instance_t() {
		//Stop streaming if we are
		stop_rx();

		//Close the device handle
		airspy_close(device);

		//Set the device handle to NULL
		device = NULL;
	}

	virtual size_t get_name(char* result, size_t resultMax) override
	{
		return raptorhw_strcpy(result, "AirSpy", resultMax);
	}

	virtual size_t get_serial(char* result, size_t resultMax) override
	{
		return snprintf(result, resultMax, "%llx", serial);
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

	virtual int64_t get_current_freq() override {
		return center_freq;
	}
	virtual bool set_current_freq(int64_t value) override {
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

	virtual bool get_direct_sampling() override {
		return false;
	}
	virtual bool set_direct_sampling(bool enabled) override {
		return false;
	}

	virtual int get_supported_samplerates_count() override {
		uint32_t temp;
		airspy_get_samplerates(device, &temp, 0);
		return temp;
	}
	virtual int get_supported_samplerates(int* result, int max) override {
		return airspy_get_samplerates(device, (unsigned int*)result, max);
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

	int64_t sample_rate;
	int64_t center_freq;
	bool bias_t_enabled;

	raptorhw_instance_streaming_cb rx_callback;
	void* rx_ctx;

	static int read_callback(airspy_transfer* transfer) {
		//Get the true context
		airspy_instance_t* ctx = (airspy_instance_t*)transfer->ctx;

		//Send
		ctx->rx_callback((raptorhw_instance_t*)ctx, ctx->rx_ctx, (raptorhw_complex_t*)transfer->samples, transfer->sample_count);

		return 0;
	}

};

class airspy_candidate_t : public raptorhw_candidate_impl {

public:
	airspy_candidate_t(uint64_t serial) : serial(serial)
	{
	}

	virtual size_t get_name(char* result, size_t resultMax) override
	{
		return raptorhw_strcpy(result, "AirSpy", resultMax);
	}

	virtual size_t get_serial(char* result, size_t resultMax) override
	{
		return snprintf(result, resultMax, "%llx", serial);
	}

	virtual int get_capabilities() override {
		return AIRSPY_CAPABILITIES;
	}

	virtual raptorhw_device_impl* open() override {
		//Open the AirSpy device
		airspy_device* device;
		if (airspy_open_sn(&device, serial) != 0)
			return NULL;

		//Wrap the device
		return new airspy_instance_t(serial, device);
	}

private:
	uint64_t serial;

};

#define AIRSPY_MAX_DEVICES 16

class airspy_module : public raptorhw_module_impl {

	virtual size_t get_name(char* result, size_t resultSize) override
	{
		return raptorhw_strcpy(result, "AirSpy", resultSize);
	}

	virtual void search(raptorhw_module_impl_search_cb callback, void* callbackCtx) override
	{
		//Request serials
		uint64_t serials[AIRSPY_MAX_DEVICES];
		int count = airspy_list_devices(serials, AIRSPY_MAX_DEVICES);

		//Create devices
		for (int i = 0; i < count; i++)
			callback(callbackCtx, new airspy_candidate_t(serials[i]));
	}

};

raptorhw_module_impl* raptorhw_device_airspy_create() {
	return new airspy_module();
}