#include <raptorhw.h>
#include "context.h"

raptorhw_context_t* raptorhw_context_create() {
    return new raptorhw_context_impl();
}

int raptorhw_context_get_module_count(raptorhw_context_t* context) {
    return ((raptorhw_context_impl*)context)->modules_count;
}

size_t raptorhw_context_get_module_info(raptorhw_context_t* context, int index, char* nameBuffer, size_t nameBufferSize) {
    //Make sure index is within bounds
    if (index < 0 || index >= ((raptorhw_context_impl*)context)->modules_count) {
        nameBuffer[0] = 0;
        return 0;
    }
    else {
        return ((raptorhw_context_impl*)context)->modules[index]->get_name(nameBuffer, nameBufferSize);
    }
}

raptorhw_candidate_t** raptorhw_context_search(raptorhw_context_t* context) {
    return (raptorhw_candidate_t**)((raptorhw_context_impl*)context)->search();
}

void raptorhw_context_search_destroy(raptorhw_context_t* context, raptorhw_candidate_t** candidates) {
    ((raptorhw_context_impl*)context)->search_destroy((raptorhw_candidate_impl**)candidates);
}

void raptorhw_context_destroy(raptorhw_context_t* context) {
    delete (raptorhw_context_impl*)context;
}

/* CANDIDATE METHODS */

/// <summary>
/// Gets the pointer to the name of this device. Valid as long as the candidate list is.
/// </summary>
/// <param name="candidate">The candidate handle.</param>
/// <returns></returns>
size_t raptorhw_candidate_get_name(raptorhw_candidate_t* candidate, char* result, size_t resultLen) {
    return ((raptorhw_candidate_impl*)candidate)->get_name(result, resultLen);
}

/// <summary>
/// Gets the pointer to the serial number of this device. Valid as long as the candidate list is.
/// </summary>
/// <param name="candidate">The candidate handle.</param>
/// <returns></returns>
size_t raptorhw_candidate_get_serial(raptorhw_candidate_t* candidate, char* result, size_t resultLen) {
    return ((raptorhw_candidate_impl*)candidate)->get_serial(result, resultLen);
}

/// <summary>
/// Gets the capabilities of this device. Use the RAPTORHW_CAPABILITY_* macros to compare the bit flags to.
/// </summary>
/// <param name="candidate">The candidate handle.</param>
/// <returns>Bit flags corresponding to the capabilities of this device.</returns>
int raptorhw_candidate_get_capabilities(raptorhw_candidate_t* candidate) {
    return ((raptorhw_candidate_impl*)candidate)->get_capabilities();
}

/// <summary>
/// Opens the device and returns a handle for controlling it. It is safe to destroy the candidate list and continue to use the returned handle.
/// </summary>
/// <param name="candidate">The candidate handle.</param>
/// <returns>The handle for controlling the new instance. Safe to use even after the candidate list is destroyed.</returns>
raptorhw_instance_t* raptorhw_candidate_open(raptorhw_candidate_t* candidate) {
    return (raptorhw_instance_t*)((raptorhw_candidate_impl*)candidate)->open();
}

/* INSTANCE METHODS */

/// <summary>
/// Gets the pointer to the name of this device. Valid as long as device is valid.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>The name of the device.</returns>
size_t raptorhw_instance_get_name(raptorhw_instance_t* device, char* result, size_t resultLen) {
    return ((raptorhw_device_impl*)device)->get_name(result, resultLen);
}

/// <summary>
/// Gets the pointer to the serial number of this device. Valid as long as device is valid.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>The serial number of the device.</returns>
size_t raptorhw_instance_get_serial(raptorhw_instance_t* device, char* result, size_t resultLen) {
    return ((raptorhw_device_impl*)device)->get_serial(result, resultLen);
}

/// <summary>
/// Gets the capabilities of this device. Use the RAPTORHW_CAPABILITY_* macros to compare the bit flags to.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>Bit flags corresponding to the capabilities of this device.</returns>
int raptorhw_instance_get_capabilities(raptorhw_instance_t* device) {
    return ((raptorhw_device_impl*)device)->get_capabilities();
}

/// <summary>
/// Gets the estimated minimum frequency of this device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>The estimated minimum frequency of the device.</returns>
int64_t raptorhw_instance_get_min_freq(raptorhw_instance_t* device) {
    return ((raptorhw_device_impl*)device)->get_min_freq();
}

/// <summary>
/// Gets the estimated maximum frequency of this device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>The estimated maximum frequency of the device.</returns>
int64_t raptorhw_instance_get_max_freq(raptorhw_instance_t* device) {
    return ((raptorhw_device_impl*)device)->get_max_freq();
}

/// <summary>
/// Gets the current center frequency of the device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>The current center frequency of the device.</returns>
int64_t raptorhw_instance_get_freq(raptorhw_instance_t* device) {
    return ((raptorhw_device_impl*)device)->get_current_freq();
}

/// <summary>
/// Sets the current center frequency of the device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>0 if unsuccessful, 1 if successful.</returns>
RAPTORHW_RESULT raptorhw_instance_set_freq(raptorhw_instance_t* device, int64_t value) {
    return ((raptorhw_device_impl*)device)->set_current_freq(value);
}

/// <summary>
/// Gets if Bias T is enabled on the device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>1 if it is enabled, otherwise 0.</returns>
RAPTORHW_RESULT raptorhw_instance_get_bias_t(raptorhw_instance_t* device) {
    return ((raptorhw_device_impl*)device)->get_bias_t();
}

/// <summary>
/// Sets if Bias T is enabled on the device. Only usable if RAPTORHW_CAPABILITY_BIAS_T is set in the device capabilities.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <param name="enabled">0 or 1, 1 being enabled and 0 being disabled.</param>
/// <returns>0 or 1. 1 is returned if the operation was successful, otherwise 0.</returns>
RAPTORHW_RESULT raptorhw_instance_set_bias_t(raptorhw_instance_t* device, RAPTORHW_RESULT enabled) {
    return ((raptorhw_device_impl*)device)->set_bias_t(enabled);
}

/// <summary>
/// Gets if offset tuning is enabled on the device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>0 or 1. 1 is returned if offset tuning is on, otherwise 0.</returns>
RAPTORHW_RESULT raptorhw_instance_get_offset_tuning(raptorhw_instance_t* device) {
    return ((raptorhw_device_impl*)device)->get_offset_tuning();
}

/// <summary>
/// Sets if offset tuning is enabled on the device. Only usable if RAPTORHW_CAPABILITY_OFFSET_TUNING is set in the device capabilities.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <param name="enabled">0 or 1, 1 being enabled and 0 being disabled.</param>
/// <returns>0 or 1. 1 is returned if the operation was successful, otherwise 0.</returns>
RAPTORHW_RESULT raptorhw_instance_set_offset_tuning(raptorhw_instance_t* device, RAPTORHW_RESULT enabled) {
    return ((raptorhw_device_impl*)device)->set_offset_tuning(enabled);
}

/// <summary>
/// Gets if direct sampling is enabled on the device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>0 or 1. 1 is returned if direct sampling is on, otherwise 0.</returns>
RAPTORHW_RESULT raptorhw_instance_get_direct_sampling(raptorhw_instance_t* device) {
    return ((raptorhw_device_impl*)device)->get_direct_sampling();
}

/// <summary>
/// Sets if direct sampling is enabled on the device. Only usable if RAPTORHW_CAPABILITY_DIRECT_SAMPLING is set in the device capabilities.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <param name="enabled">0 or 1, 1 being enabled and 0 being disabled.</param>
/// <returns>0 or 1. 1 is returned if the operation was successful, otherwise 0.</returns>
RAPTORHW_RESULT raptorhw_instance_set_direct_sampling(raptorhw_instance_t* device, RAPTORHW_RESULT enabled) {
    return ((raptorhw_device_impl*)device)->set_direct_sampling(enabled);
}

/// <summary>
/// Requests the device's supported sample rates. Result is an array you must preallocate. Calling this function with max==0 will return the number of items to preallocate.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <param name="result">A preallocated array, length max, of ints that the result will be written to.</param>
/// <param name="max">The maximum number of items to write to result. Calling with 0 will return the number of items present.</param>
/// <returns>The number of values written to result if max != 0. If max == 0, returns the total number of items.</returns>
int raptorhw_instance_get_supported_samplerates(raptorhw_instance_t* device, int* result, int max) {
    if (max == 0)
        return ((raptorhw_device_impl*)device)->get_supported_samplerates_count();
    else
        return ((raptorhw_device_impl*)device)->get_supported_samplerates(result, max);
}

/// <summary>
/// Gets the current sample rate of the device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>The current device sample rate.</returns>
int raptorhw_instance_get_samplerate(raptorhw_instance_t* device) {
    return ((raptorhw_device_impl*)device)->get_samplerate();
}

/// <summary>
/// Sets the current sample rate to the value specified.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>0 or 1. 1 is returned if the operation was successful, otherwise 0.</returns>
RAPTORHW_RESULT raptorhw_instance_set_samplerate(raptorhw_instance_t* device, int index) {
    return ((raptorhw_device_impl*)device)->set_samplerate(index);
}

/// <summary>
/// Get the number of gain settings available.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>The number of gains accessible with raptorhw_instance_get_gains_at.</returns>
int raptorhw_instance_get_gain_group_count(raptorhw_instance_t* device) {
    return ((raptorhw_device_impl*)device)->get_gains_count();
}

/// <summary>
/// Gets a gain setting at the specified index. The pointer is valid until the device handle is destroyed.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>If index is valid, a pointer to the gain setting, valid until the device handle is destroyed with raptorhw_instance_close. Otherwise, NULL.</returns>
raptorhw_gain_group_t* raptorhw_instance_get_gain_group_at(raptorhw_instance_t* device, int index) {
    return (raptorhw_gain_group_t*)((raptorhw_device_impl*)device)->get_gains_at(index);
}

/// <summary>
/// Starts receiving samples asynchronously. For each sample buffer, the callback will be called. Ran until raptorhw_instance_stop_rx.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <param name="callback">The callback fired for each block of samples.</param>
/// <param name="ctx">The context object passed into the callback when it's fired.</param>
/// <returns>0 or 1. 1 is returned if the operation was successful, otherwise 0.</returns>
RAPTORHW_RESULT raptorhw_instance_start_rx(raptorhw_instance_t* device, raptorhw_instance_streaming_cb callback, void* ctx) {
    return ((raptorhw_device_impl*)device)->start_rx(callback, ctx);
}

/// <summary>
/// Stops the receiving samples process started by raptorhw_instance_start_rx.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
void raptorhw_instance_stop_rx(raptorhw_instance_t* device) {
    ((raptorhw_device_impl*)device)->stop_rx();
}

/// <summary>
/// Stops streaming, destroys the device handle, and frees all resources used by the device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
void raptorhw_instance_close(raptorhw_instance_t* device) {
    delete ((raptorhw_device_impl*)device);
}

/* GAIN GROUP METHODS */

size_t raptorhw_gain_group_get_name(raptorhw_gain_group_t* group, char* result, size_t resultLen) {
    return ((raptorhw_gain_group_impl*)group)->get_name(result, resultLen);
}
int raptorhw_gain_group_get_option_count(raptorhw_gain_group_t* group) {
    return ((raptorhw_gain_group_impl*)group)->get_option_count();
}
raptorhw_gain_t* raptorhw_gain_group_get_option(raptorhw_gain_group_t* group, int index) {
    return (raptorhw_gain_t*)((raptorhw_gain_group_impl*)group)->get_option(index);
}
int raptorhw_gain_group_select(raptorhw_gain_group_t* group) {
    return ((raptorhw_gain_group_impl*)group)->select();
}

size_t raptorhw_gain_item_get_name(raptorhw_gain_t* item, char* result, size_t resultLen) {
    return ((raptorhw_gain_option_impl*)item)->get_name(result, resultLen);
}
int raptorhw_gain_item_get_units(raptorhw_gain_t* item) {
    return ((raptorhw_gain_option_impl*)item)->get_unit();
}
int raptorhw_gain_item_get_agc_supported(raptorhw_gain_t* item) {
    return ((raptorhw_gain_option_impl*)item)->get_agc_supported();
}
int raptorhw_gain_item_get_agc(raptorhw_gain_t* item) {
    return ((raptorhw_gain_option_impl*)item)->get_agc();
}
int raptorhw_gain_item_set_agc(raptorhw_gain_t* item, int enabled) {
    return ((raptorhw_gain_option_impl*)item)->set_agc(enabled);
}
int raptorhw_gain_item_get_option_count(raptorhw_gain_t* item) {
    return ((raptorhw_gain_option_impl*)item)->get_option_count();
}
float raptorhw_gain_item_get_option_label(raptorhw_gain_t* item, int index) {
    return ((raptorhw_gain_option_impl*)item)->get_option_value(index);
}
int raptorhw_gain_item_get_value(raptorhw_gain_t* item) {
    return ((raptorhw_gain_option_impl*)item)->get_value();
}
int raptorhw_gain_item_set_value(raptorhw_gain_t* item, int index) {
    return ((raptorhw_gain_option_impl*)item)->set_value(index);
}