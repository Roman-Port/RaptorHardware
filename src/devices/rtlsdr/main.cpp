#include "include.h"
#include "conversion_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <rtl-sdr.h>
#include <cassert>
#include <cstring>

#define RTLSDR_INFO_BUFFER_SIZE 257
#define RTLSDR_BUFFER_SIZE 16384
#define RTLSDR_CAPABILITIES (RAPTORHW_CAPABILITY_BIAS_T | RAPTORHW_CAPABILITY_OFFSET_TUNING | RAPTORHW_CAPABILITY_DIRECT_SAMPLING)

const int SUPPORTED_SAMPLE_RATES[] = {
	3200000,
	2800000,
	2560000,
	2400000,
	2048000,
	1920000,
	1800000,
	1400000,
	1024000,
	900001,
	250000
};

class rtlsdr_gain_tuner_t : public raptorhw_gain_t {

public:
	rtlsdr_gain_tuner_t(rtlsdr_dev_t* device) : device(device), current_gain_index(0), current_agc(false) {
		//Request the number of tuner gains
		gain_count = rtlsdr_get_tuner_gains(device, NULL);

		//Allocate the gains array
		gains = (int*)malloc(gain_count * sizeof(int));
		assert(gains != NULL);

		//Request tuner gains (which are in 1/10ths of a dB)
		rtlsdr_get_tuner_gains(device, gains);
	}

	virtual const char* get_name() override {
		return "Gain";
	}
	virtual bool get_agc_supported() override {
		return true;
	}

	virtual bool get_agc() override {
		return current_agc;
	}
	virtual bool set_agc(bool enabled) override {
		//Set on device
		if (rtlsdr_set_tuner_gain_mode(device, enabled ? 0 : 1) != 0)
			return false;

		//Set in cache
		current_agc = enabled;

		return true;
	}

	virtual int get_value_count() override {
		return gain_count;
	}
	virtual void get_value_label(int index, char* result, size_t resultLen) override {
		//Sanity check
		if (resultLen < 1)
			return;
		if (index < 0 || index >= gain_count) {
			result[0] = 0x00;
			return;
		}

		//Format
		sprintf_s(result, resultLen, "%i.%i dB", gains[index] / 10, gains[index] % 10);
	}

	virtual int get_value() override {
		return current_gain_index;
	}
	virtual bool set_value(int index) override {
		//Sanity check
		if (index < 0 || index >= gain_count)
			return false;

		//Make sure AGC is off
		if (current_agc)
			set_agc(false);

		//Apply to device
		if (rtlsdr_set_tuner_gain(device, gains[index]) != 0)
			return false;

		//Set in cache
		current_gain_index = index;

		return true;
	}

private:
	rtlsdr_dev_t* device;
	bool current_agc;
	int current_gain_index;
	int gain_count;
	int* gains;

};

class rtlsdr_gain_group_default_t : public raptorhw_gain_group_t {

public:
	rtlsdr_gain_group_default_t(rtlsdr_dev_t* device) : tuner(device) {

	}

	virtual const char* get_name() override {
		return "Default";
	}
	virtual int get_setting_count() override {
		return 1;
	}
	virtual raptorhw_gain_t* get_setting(int index) override {
		switch (index) {
		case 0: return &tuner;
		}
		return 0;
	}
	virtual bool select() override {
		return true;
	}

private:
	rtlsdr_gain_tuner_t tuner;

};

class rtlsdr_instance_t : public raptorhw_instance_t {

public:
	rtlsdr_instance_t(char* inProduct, char* inSerial, rtlsdr_dev_t* device) :
		device(device),
		gain_group(device),
		bias_t_enabled(false),
		rx_callback(NULL),
		rx_ctx(NULL)
	{
		memcpy(product, inProduct, sizeof(product));
		memcpy(serial, inSerial, sizeof(serial));
	}

	~rtlsdr_instance_t() {
		//Stop streaming if we are
		stop_rx();

		//Close the device handle
		rtlsdr_close(device);

		//Set the device handle to NULL
		device = NULL;
	}

	virtual const char* get_name() override {
		return product;
	}

	virtual const char* get_serial() override {
		return serial;
	}

	virtual int get_capabilities() override {
		return RTLSDR_CAPABILITIES;
	}

	virtual int64_t get_min_freq() override {
		//Check if direct sampling is enabled. If it is, return the 0-28.8 MHz tuning range
		if (get_direct_sampling())
			return 0;

		//Get the tuner type and use that to determine the estimated min frequency
		rtlsdr_tuner tuner = rtlsdr_get_tuner_type(device);

		//Compare to the table at https://osmocom.org/projects/rtl-sdr/wiki/Rtl-sdr
		switch (tuner) {
		case rtlsdr_tuner::RTLSDR_TUNER_E4000: return 52000000;
		case rtlsdr_tuner::RTLSDR_TUNER_FC0012: return 22000000;
		case rtlsdr_tuner::RTLSDR_TUNER_FC0013: return 22000000;
		case rtlsdr_tuner::RTLSDR_TUNER_FC2580: return 146000000;
		case rtlsdr_tuner::RTLSDR_TUNER_R820T: return 24000000;
		case rtlsdr_tuner::RTLSDR_TUNER_R828D: return 24000000;
		}

		//Unknown type. Return some default value...
		return 0;
	}
	virtual int64_t get_max_freq() override {
		//Check if direct sampling is enabled. If it is, return the 0-28.8 MHz tuning range
		if (get_direct_sampling())
			return 28800000;

		//Get the tuner type and use that to determine the estimated min frequency
		rtlsdr_tuner tuner = rtlsdr_get_tuner_type(device);

		//Compare to the table at https://osmocom.org/projects/rtl-sdr/wiki/Rtl-sdr
		switch (tuner) {
		case rtlsdr_tuner::RTLSDR_TUNER_E4000: return 2200000000;
		case rtlsdr_tuner::RTLSDR_TUNER_FC0012: return 948600000;
		case rtlsdr_tuner::RTLSDR_TUNER_FC0013: return 1100000000;
		case rtlsdr_tuner::RTLSDR_TUNER_FC2580: return 924000000;
		case rtlsdr_tuner::RTLSDR_TUNER_R820T: return 1766000000;
		case rtlsdr_tuner::RTLSDR_TUNER_R828D: return 1766000000;
		}

		//Unknown type. Return some default value...
		return 2000000000;
	}

	virtual int64_t get_freq() override {
		return rtlsdr_get_center_freq(device);
	}
	virtual bool set_freq(int64_t value) override {
		return rtlsdr_set_center_freq(device, value) != 0;
	}

	virtual bool get_bias_t() override {
		return bias_t_enabled;
	}
	virtual bool set_bias_t(bool enabled) override {
		bias_t_enabled = enabled;
		return rtlsdr_set_bias_tee(device, enabled) == 0;
	}

	virtual bool get_offset_tuning() override {
		return rtlsdr_get_offset_tuning(device);
	}
	virtual bool set_offset_tuning(bool enabled) override {
		return rtlsdr_set_offset_tuning(device, enabled) == 0;
	}

	virtual bool get_direct_sampling() override {
		return rtlsdr_get_direct_sampling(device);
	}
	virtual bool set_direct_sampling(bool enabled) override {
		return rtlsdr_set_direct_sampling(device, enabled) == 0;
	}

	virtual int get_supported_samplerates(int* result, int max) override {
		//Calculate total count
		int totalCount = sizeof(SUPPORTED_SAMPLE_RATES) / sizeof(int);

		//If count is 0, return the number of items
		if (max == 0 || result == 0)
			return totalCount;

		//Copy
		int i;
		for (i = 0; i < max && i < totalCount; i++)
			result[i] = SUPPORTED_SAMPLE_RATES[i];
		return i;
	}
	virtual int get_samplerate() override {
		return rtlsdr_get_sample_rate(device);
	}
	virtual bool set_samplerate(int rate) override {
		return rtlsdr_set_sample_rate(device, rate) == 0;
	}

	virtual int get_gain_group_count() override {
		return 1;
	}
	virtual raptorhw_gain_group_t* get_gain_group_at(int index) override {
		switch (index) {
		case 0: return &gain_group;
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

		//Reset the device buffer. Not quite sure what this does but it seems required
		rtlsdr_reset_buffer(device);

		//Begin streaming
		return rtlsdr_read_async(device, read_callback, this, 0, RTLSDR_BUFFER_SIZE) == 0;
	}
	virtual void stop_rx() override {
		//Make sure that there is an active session
		if (rx_callback == NULL)
			return;

		//Stop the active thread
		if (rtlsdr_cancel_async(device) != 0)
			return;

		//Clear out the internal arguments
		rx_callback = NULL;
		rx_ctx = NULL;
	}

private:
	char product[RTLSDR_INFO_BUFFER_SIZE];
	char serial[RTLSDR_INFO_BUFFER_SIZE];

	rtlsdr_dev_t* device;
	bool bias_t_enabled;
	rtlsdr_gain_group_default_t gain_group;
	float converted_buffer[RTLSDR_BUFFER_SIZE];

	raptorhw_instance_streaming_cb rx_callback;
	void* rx_ctx;

	static void read_callback(unsigned char* buf, uint32_t len, void* ctxBase) {
		//Get the true context
		rtlsdr_instance_t* ctx = (rtlsdr_instance_t*)ctxBase;

		//Sanity check
		assert(len < RTLSDR_BUFFER_SIZE);
		assert(len % 2 == 0);

		//Convert samples using the lookup table
		for (int i = 0; i < len; i++)
			ctx->converted_buffer[i] = SAMPLE_CONVERSION_TABLE[buf[i]];

		//Send callback. We cut the length in half to convert to complex samples
		ctx->rx_callback(ctx, ctx->rx_ctx, (raptorhw_complex_t*)ctx->converted_buffer, len / 2);
	}

};

class rtlsdr_candidate_t : public raptorhw_candidate_t {

public:
	rtlsdr_candidate_t(uint32_t index, char* inProduct, char* inSerial) : index(index)
	{
		memcpy(product, inProduct, sizeof(product));
		memcpy(serial, inSerial, sizeof(serial));
	}

	virtual const char* get_name() override {
		return product;
	}

	virtual const char* get_serial() override {
		return serial;
	}

	virtual int get_capabilities() override {
		return RTLSDR_CAPABILITIES;
	}

	virtual raptorhw_instance_t* create_instance() override {
		//Open the RTL-SDR device
		rtlsdr_dev_t* device;
		if (rtlsdr_open(&device, index) != 0)
			return NULL;

		//Wrap the device
		return new rtlsdr_instance_t(product, serial, device);
	}

private:
	uint32_t index;
	char product[RTLSDR_INFO_BUFFER_SIZE];
	char serial[RTLSDR_INFO_BUFFER_SIZE];

};

int raptorhw_rtlsdr_search(raptorhw_candidate_t** candidates, int max) {
	//Request the device count
	uint32_t count = rtlsdr_get_device_count();

	//Enumerate and create devices
	uint32_t i = 0;
	char manufacturer[RTLSDR_INFO_BUFFER_SIZE];
	char product[RTLSDR_INFO_BUFFER_SIZE];
	char serial[RTLSDR_INFO_BUFFER_SIZE];
	for (i = 0; i < max && i < count && rtlsdr_get_device_usb_strings(i, manufacturer, product, serial) == 0; i++) {
		candidates[i] = new rtlsdr_candidate_t(i, product, serial);
	}

	return i;
}