#include "defines.h"

const char* raptorhw_instance_get_name(raptorhw_instance_t* device) {
	return device->get_name();
}

const char* raptorhw_instance_get_serial(raptorhw_instance_t* device) {
	return device->get_serial();
}

int raptorhw_instance_get_capabilities(raptorhw_instance_t* device) {
	return device->get_capabilities();
}

int64_t raptorhw_instance_get_min_freq(raptorhw_instance_t* device) {
	return device->get_min_freq();
}

int64_t raptorhw_instance_get_max_freq(raptorhw_instance_t* device) {
	return device->get_max_freq();
}

int64_t raptorhw_instance_get_freq(raptorhw_instance_t* device) {
	return device->get_freq();
}

RAPTORHW_RESULT raptorhw_instance_set_freq(raptorhw_instance_t* device, int64_t value) {
	return device->set_freq(value);
}

RAPTORHW_RESULT raptorhw_instance_get_bias_t(raptorhw_instance_t* device) {
	return device->get_bias_t();
}

RAPTORHW_RESULT raptorhw_instance_set_bias_t(raptorhw_instance_t* device, RAPTORHW_RESULT enabled) {
	return device->set_bias_t(enabled);
}

RAPTORHW_RESULT raptorhw_instance_get_offset_tuning(raptorhw_instance_t* device) {
	return device->get_offset_tuning();
}

RAPTORHW_RESULT raptorhw_instance_set_offset_tuning(raptorhw_instance_t* device, RAPTORHW_RESULT enabled) {
	return device->set_offset_tuning(enabled);
}

RAPTORHW_RESULT raptorhw_instance_get_direct_sampling(raptorhw_instance_t* device) {
	return device->get_direct_sampling();
}

RAPTORHW_RESULT raptorhw_instance_set_direct_sampling(raptorhw_instance_t* device, RAPTORHW_RESULT enabled) {
	return device->set_direct_sampling(enabled);
}

int raptorhw_instance_get_supported_samplerates(raptorhw_instance_t* device, int* result, int count) {
	return device->get_supported_samplerates(result, count);
}

int raptorhw_instance_get_samplerate(raptorhw_instance_t* device) {
	return device->get_samplerate();
}

RAPTORHW_RESULT raptorhw_instance_set_samplerate(raptorhw_instance_t* device, int rate) {
	return device->set_samplerate(rate);
}

int raptorhw_instance_get_gain_group_count(raptorhw_instance_t* device) {
	return device->get_gain_group_count();
}

raptorhw_gain_group_t* raptorhw_instance_get_gain_group_at(raptorhw_instance_t* device, int index) {
	return device->get_gain_group_at(index);
}

RAPTORHW_RESULT raptorhw_instance_start_rx(raptorhw_instance_t* device, raptorhw_instance_streaming_cb callback, void* ctx) {
	return device->start_rx(callback, ctx);
}

void raptorhw_instance_stop_rx(raptorhw_instance_t* device) {
	device->stop_rx();
}

void raptorhw_instance_close(raptorhw_instance_t* device) {
	delete (device);
}