#include "device.h"
#include "gain.h"
#include "custom_properties.h"
#include <raptorhw_util.h>
#include <cassert>
#include <stdlib.h>

raptor_airspyhf_device::raptor_airspyhf_device(uint64_t serial, airspyhf_device_t* device) :
	serial(serial),
	device(device),
	current_center_freq(0),
	current_sample_rate(0),
	rx_callback(0),
	rx_ctx(0)
{
	//Query the number of sample rates
	airspyhf_get_samplerates(device, &sample_rates_count, 0);

	//Allocate sample rates
	sample_rates = (uint32_t*)malloc(sizeof(uint32_t) * sample_rates_count);
	assert(sample_rates != 0);

	//Request sample rates
	airspyhf_get_samplerates(device, sample_rates, sample_rates_count);

	//Add custom properties
	put_gains(new raptor_airspyhf_gain(device));
	put_property(new prop_agc_threshold(device));
	put_property(new prop_preamp(device));
	put_property(new prop_version(device));
}

raptor_airspyhf_device::~raptor_airspyhf_device() {
	//Free the sample rate array
	free(sample_rates);
	sample_rates = 0;
}

size_t raptor_airspyhf_device::get_name(char* result, size_t resultMax)
{
	return raptorhw_strcpy(result, "AirSpy HF+", resultMax);
}

size_t raptor_airspyhf_device::get_serial(char* result, size_t resultMax)
{
	return snprintf(result, resultMax, "%llx", serial);
}

int raptor_airspyhf_device::get_capabilities()
{
	return AIRSPYHF_CAPABILITIES;
}

int64_t raptor_airspyhf_device::get_min_freq()
{
	return 9000; //https://airspy.com/airspy-hf-plus/
}

int64_t raptor_airspyhf_device::get_max_freq()
{
	return 260000000; //https://airspy.com/airspy-hf-plus/
}

int64_t raptor_airspyhf_device::get_current_freq()
{
	return current_center_freq;
}

bool raptor_airspyhf_device::set_current_freq(int64_t value)
{
	if (airspyhf_set_freq(device, value) != 0)
		return false;
	current_center_freq = value;
	return true;
}

int raptor_airspyhf_device::get_supported_samplerates_count()
{
	return sample_rates_count;
}

int raptor_airspyhf_device::get_supported_samplerates(int* result, int max)
{
	bool success = airspyhf_get_samplerates(device, (uint32_t*)result, max) == AIRSPYHF_SUCCESS;
	return success ? max : 0;
}

int raptor_airspyhf_device::get_samplerate()
{
	return current_sample_rate;
}

bool raptor_airspyhf_device::set_samplerate(int index)
{
	//Convert the index into the actual samplerate
	if (index < 0 || index >= sample_rates_count)
		return false;
	uint32_t sampleRate = sample_rates[index];

	//Apply
	if (airspyhf_set_samplerate(device, sampleRate) != 0)
		return false;

	//Set
	current_sample_rate = sampleRate;
	return true;
}

bool raptor_airspyhf_device::start_rx(raptorhw_instance_streaming_cb callback, void* ctx)
{
	//Make sure there isn't already an active session
	if (rx_callback != NULL)
		return false;

	//Sanity check
	if (callback == NULL)
		return false;

	//Set the arguments locally
	rx_callback = callback;
	rx_ctx = ctx;

	//Begin streaming
	return airspyhf_start(device, read_callback, this) == 0;
}

void raptor_airspyhf_device::stop_rx()
{
	//Make sure that there is an active session
	if (rx_callback == NULL)
		return;

	//Stop the active thread
	if (airspyhf_stop(device) != 0)
		return;

	//Clear out the internal arguments
	rx_callback = NULL;
	rx_ctx = NULL;
}

int raptor_airspyhf_device::read_callback(airspyhf_transfer_t* data) {
	//Get the context
	raptor_airspyhf_device* context = (raptor_airspyhf_device*)data->ctx;

	//Immediately pass the buffer out
	context->rx_callback((raptorhw_instance_t*)context, context->rx_ctx, (raptorhw_complex_t*)data->samples, data->sample_count);

	//TODO: Notify of dropped samples

	return 0;
}