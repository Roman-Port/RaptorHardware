#pragma once

#include "../include/raptorhw.h"

class raptorhw_gain_t {

public:
	virtual const char* get_name() = 0;
	virtual bool get_agc_supported() = 0;

	virtual bool get_agc() = 0;
	virtual bool set_agc(bool enabled) = 0;

	virtual int get_value_count() = 0;
	virtual void get_value_label(int index, char* result, size_t resultLen) = 0;

	virtual int get_value() = 0;
	virtual bool set_value(int index) = 0;

};

class raptorhw_gain_group_t {

public:
	virtual const char* get_name() = 0;
	virtual int get_setting_count() = 0;
	virtual raptorhw_gain_t* get_setting(int count) = 0;
	virtual bool select() = 0;

};

class raptorhw_instance_t {

public:
	virtual const char* get_name() = 0;
	virtual const char* get_serial() = 0;
	virtual int get_capabilities() = 0;

	virtual int64_t get_min_freq() = 0;
	virtual int64_t get_max_freq() = 0;

	virtual int64_t get_freq() = 0;
	virtual bool set_freq(int64_t value) = 0;

	virtual bool get_bias_t() {
		return false;
	}
	virtual bool set_bias_t(bool enabled) {
		return false;
	}

	virtual bool get_offset_tuning() {
		return false;
	}
	virtual bool set_offset_tuning(bool enabled) {
		return false;
	}

	virtual bool get_direct_sampling() {
		return false;
	}
	virtual bool set_direct_sampling(bool enabled) {
		return false;
	}

	//result* is a buffer of size count. The total number of samplerates is always returned.
	virtual int get_supported_samplerates(int* result, int count) = 0;
	virtual int get_samplerate() = 0;
	virtual bool set_samplerate(int rate) = 0;

	virtual int get_gain_group_count() = 0;
	virtual raptorhw_gain_group_t* get_gain_group_at(int index) = 0;

	virtual bool start_rx(raptorhw_instance_streaming_cb callback, void* ctx) = 0;
	virtual void stop_rx() = 0;

};

class raptorhw_candidate_t {

public:
	virtual const char* get_name() = 0;
	virtual const char* get_serial() = 0;
	virtual int get_capabilities() = 0;
	virtual raptorhw_instance_t* create_instance() = 0;

};