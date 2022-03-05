#pragma once
#include <raptorhw_gain.h>
#include <stdint.h>
#include <stdio.h>

#define RAPTORHW_MAX_GAIN_GROUPS 8

class raptorhw_device_impl {

public:
	raptorhw_device_impl();
	virtual ~raptorhw_device_impl();

	virtual size_t get_name(char* result, size_t resultMax) = 0;
	virtual size_t get_serial(char* result, size_t resultMax) = 0;
	virtual int get_capabilities() = 0;
	virtual int64_t get_min_freq() = 0;
	virtual int64_t get_max_freq() = 0;
	virtual int64_t get_current_freq() = 0;

	virtual bool set_current_freq(int64_t value) = 0;

	virtual bool get_bias_t() {
		return false;
	}
	virtual bool set_bias_t(bool enabled) {
		return false;
	}

	virtual bool get_direct_sampling() {
		return false;
	}
	virtual bool set_direct_sampling(bool enabled) {
		return false;
	}

	virtual bool get_offset_tuning() {
		return false;
	}
	virtual bool set_offset_tuning(bool enabled) {
		return false;
	}

	int get_gains_count();
	raptorhw_gain_group_impl* get_gains_at(int index);

	virtual int get_supported_samplerates_count() = 0;
	virtual int get_supported_samplerates(int* result, int max) = 0;

	virtual int get_samplerate() = 0;
	virtual bool set_samplerate(int index) = 0;

	virtual bool start_rx(raptorhw_instance_streaming_cb callback, void* ctx) = 0;
	virtual void stop_rx() = 0;

protected:
	void put_gains(raptorhw_gain_group_impl* option);

private:
	raptorhw_gain_group_impl* gains[RAPTORHW_MAX_GAIN_GROUPS];
	int gains_count;

};