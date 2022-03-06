#include <raptorhw_device.h>
#include <cassert>

raptorhw_device_impl::raptorhw_device_impl() {
	//Clear gains
	gains_count = 0;
	for (int i = 0; i < RAPTORHW_MAX_GAIN_GROUPS; i++)
		gains[i] = 0;

	//Clear properties
	properties_count = 0;
	for (int i = 0; i < RAPTORHW_MAX_CUSTOM_PROPERTIES; i++)
		properties[i] = 0;
}

raptorhw_device_impl::~raptorhw_device_impl() {
	//Free gains
	for (int i = 0; i < gains_count; i++) {
		delete gains[i];
		gains[i] = 0;
	}
	gains_count = 0;

	//Free properties
	for (int i = 0; i < properties_count; i++) {
		delete properties[i];
		properties[i] = 0;
	}
	properties_count = 0;
}

// GAINS

int raptorhw_device_impl::get_gains_count() {
	return gains_count;
}

void raptorhw_device_impl::put_gains(raptorhw_gain_group_impl* option) {
	assert(gains_count < RAPTORHW_MAX_GAIN_GROUPS);
	gains[gains_count++] = option;
}

raptorhw_gain_group_impl* raptorhw_device_impl::get_gains_at(int index) {
	if (index < 0 || index >= gains_count)
		return 0;
	else
		return gains[index];
}

// PROPERTIES

int raptorhw_device_impl::get_property_count() {
	return properties_count;
}

void raptorhw_device_impl::put_property(raptorhw_custom_property_impl* item) {
	assert(properties_count < RAPTORHW_MAX_CUSTOM_PROPERTIES);
	properties[properties_count++] = item;
}

raptorhw_custom_property_impl* raptorhw_device_impl::get_property_at(int index) {
	if (index < 0 || index >= properties_count)
		return 0;
	else
		return properties[index];
}