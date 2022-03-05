#pragma once

#include "../../include/raptorhw.h"

#define RAPTORHW_MAX_GAIN_OPTIONS 16

class raptorhw_gain_option_impl {

public:
	virtual ~raptorhw_gain_option_impl() = 0;

	virtual size_t get_name(char* result, size_t resultSize) = 0;
	virtual int get_unit() = 0;

	virtual bool get_agc_supported() = 0;

	virtual bool get_agc() = 0;
	virtual bool set_agc(bool enabled) = 0;

	virtual int get_option_count() = 0;
	virtual float get_option_value(int index) = 0;

	virtual int get_value() = 0;
	virtual bool set_value(int index) = 0;

};

class raptorhw_gain_group_impl {

public:
	raptorhw_gain_group_impl();
	virtual ~raptorhw_gain_group_impl();

	virtual size_t get_name(char* result, size_t resultSize) = 0;
	int get_option_count();
	raptorhw_gain_option_impl* get_option(int index);
	bool select();

protected:
	void put_option(raptorhw_gain_option_impl* option);

private:
	raptorhw_gain_option_impl* gains[RAPTORHW_MAX_GAIN_OPTIONS];
	int gains_count;

};