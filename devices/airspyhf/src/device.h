#pragma once
#include "internal.h"
#include <raptorhw_device.h>

class raptor_airspyhf_device : public raptorhw_device_impl {
public:
	raptor_airspyhf_device(uint64_t serial, airspyhf_device_t* device);
	~raptor_airspyhf_device();
	virtual size_t get_name(char* result, size_t resultMax) override;
	virtual size_t get_serial(char* result, size_t resultMax) override;
	virtual int get_capabilities() override;
	virtual int64_t get_min_freq() override;
	virtual int64_t get_max_freq() override;
	virtual int64_t get_current_freq() override;
	virtual bool set_current_freq(int64_t value) override;
	virtual int get_supported_samplerates_count() override;
	virtual int get_supported_samplerates(int* result, int max) override;
	virtual int get_samplerate() override;
	virtual bool set_samplerate(int index) override;
	virtual bool start_rx(raptorhw_instance_streaming_cb callback, void* ctx) override;
	virtual void stop_rx() override;

private:
	static int read_callback(airspyhf_transfer_t* transfer_fn);

	uint64_t serial;
	airspyhf_device_t* device;

	uint32_t current_center_freq;
	uint32_t current_sample_rate;

	uint32_t* sample_rates;
	uint32_t sample_rates_count;

	raptorhw_instance_streaming_cb rx_callback;
	void* rx_ctx;
};