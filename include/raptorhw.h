#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* CAPABILITIES */

#define RAPTORHW_CAPABILITY_BIAS_T (1 << 0)
#define RAPTORHW_CAPABILITY_OFFSET_TUNING (1 << 1)
#define RAPTORHW_CAPABILITY_DIRECT_SAMPLING (1 << 2)

/* UNITS */

#define RAPTORHW_UNITS_UNSPECIFIED 0
#define RAPTORHW_UNITS_DB 1

/* CUSTOM PROPERTY FLAGS */

#define RAPTORHW_CUSTOMPROP_WRITABLE (1 << 0)
#define RAPTORHW_CUSTOMPROP_STRING (1 << 1)
#define RAPTORHW_CUSTOMPROP_INT (1 << 2)
#define RAPTORHW_CUSTOMPROP_BOOL (1 << 3) /* MUST ALSO HAVE RAPTORHW_CUSTOMPROP_INT */

/* TYPES */

typedef struct { void* placeholder; } raptorhw_gain_t;
typedef struct { void* placeholder; } raptorhw_gain_group_t;
typedef struct { void* placeholder; } raptorhw_custom_property_t;
typedef struct { void* placeholder; } raptorhw_instance_t;
typedef struct { void* placeholder; } raptorhw_candidate_t;
typedef struct { void* placeholder; } raptorhw_context_t;

struct raptorhw_complex_t {
	float real;
	float imag;
};

typedef void (*raptorhw_instance_streaming_cb)(raptorhw_instance_t* device, void* ctx, raptorhw_complex_t* samples, int sampleCount);

#if defined(_WIN32)
#define EXPORT_API extern "C" __declspec( dllexport )
#elif defined(__GNUC__)
#define EXPORT_API extern "C" __attribute__((visibility("default")))
#else
#define EXPORT_API extern "C"
#endif

#define RAPTORHW_RESULT int

/* CONTEXT */

EXPORT_API raptorhw_context_t* raptorhw_context_create();
EXPORT_API int raptorhw_context_get_module_count(raptorhw_context_t* context);
EXPORT_API size_t raptorhw_context_get_module_info(raptorhw_context_t* context, int index, char* nameBuffer, size_t nameBufferSize);
EXPORT_API raptorhw_candidate_t** raptorhw_context_search(raptorhw_context_t* context);
EXPORT_API void raptorhw_context_search_destroy(raptorhw_context_t* context, raptorhw_candidate_t** candidates);
EXPORT_API void raptorhw_context_destroy(raptorhw_context_t* context);

/* CANDIDATE METHODS */

/// <summary>
/// Gets the pointer to the name of this device. Valid as long as the candidate list is.
/// </summary>
/// <param name="candidate">The candidate handle.</param>
/// <returns></returns>
EXPORT_API size_t raptorhw_candidate_get_name(raptorhw_candidate_t* candidate, char* result, size_t resultLen);

/// <summary>
/// Gets the pointer to the serial number of this device. Valid as long as the candidate list is.
/// </summary>
/// <param name="candidate">The candidate handle.</param>
/// <returns></returns>
EXPORT_API size_t raptorhw_candidate_get_serial(raptorhw_candidate_t* candidate, char* result, size_t resultLen);

/// <summary>
/// Gets the capabilities of this device. Use the RAPTORHW_CAPABILITY_* macros to compare the bit flags to.
/// </summary>
/// <param name="candidate">The candidate handle.</param>
/// <returns>Bit flags corresponding to the capabilities of this device.</returns>
EXPORT_API int raptorhw_candidate_get_capabilities(raptorhw_candidate_t* candidate);

/// <summary>
/// Opens the device and returns a handle for controlling it. It is safe to destroy the candidate list and continue to use the returned handle.
/// </summary>
/// <param name="candidate">The candidate handle.</param>
/// <returns>The handle for controlling the new instance. Safe to use even after the candidate list is destroyed.</returns>
EXPORT_API raptorhw_instance_t* raptorhw_candidate_open(raptorhw_candidate_t* candidate);

/* INSTANCE METHODS */

/// <summary>
/// Gets the pointer to the name of this device. Valid as long as device is valid.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>The name of the device.</returns>
EXPORT_API size_t raptorhw_instance_get_name(raptorhw_instance_t* device, char* result, size_t resultLen);

/// <summary>
/// Gets the pointer to the serial number of this device. Valid as long as device is valid.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>The serial number of the device.</returns>
EXPORT_API size_t raptorhw_instance_get_serial(raptorhw_instance_t* device, char* result, size_t resultLen);

/// <summary>
/// Gets the capabilities of this device. Use the RAPTORHW_CAPABILITY_* macros to compare the bit flags to.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>Bit flags corresponding to the capabilities of this device.</returns>
EXPORT_API int raptorhw_instance_get_capabilities(raptorhw_instance_t* device);

/// <summary>
/// Gets the estimated minimum frequency of this device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>The estimated minimum frequency of the device.</returns>
EXPORT_API int64_t raptorhw_instance_get_min_freq(raptorhw_instance_t* device);

/// <summary>
/// Gets the estimated maximum frequency of this device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>The estimated maximum frequency of the device.</returns>
EXPORT_API int64_t raptorhw_instance_get_max_freq(raptorhw_instance_t* device);

/// <summary>
/// Gets the current center frequency of the device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>The current center frequency of the device.</returns>
EXPORT_API int64_t raptorhw_instance_get_freq(raptorhw_instance_t* device);

/// <summary>
/// Sets the current center frequency of the device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>0 if unsuccessful, 1 if successful.</returns>
EXPORT_API RAPTORHW_RESULT raptorhw_instance_set_freq(raptorhw_instance_t* device, int64_t value);

/// <summary>
/// Gets if Bias T is enabled on the device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>1 if it is enabled, otherwise 0.</returns>
EXPORT_API RAPTORHW_RESULT raptorhw_instance_get_bias_t(raptorhw_instance_t* device);

/// <summary>
/// Sets if Bias T is enabled on the device. Only usable if RAPTORHW_CAPABILITY_BIAS_T is set in the device capabilities.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <param name="enabled">0 or 1, 1 being enabled and 0 being disabled.</param>
/// <returns>0 or 1. 1 is returned if the operation was successful, otherwise 0.</returns>
EXPORT_API RAPTORHW_RESULT raptorhw_instance_set_bias_t(raptorhw_instance_t* device, RAPTORHW_RESULT enabled);

/// <summary>
/// Gets if offset tuning is enabled on the device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>0 or 1. 1 is returned if offset tuning is on, otherwise 0.</returns>
EXPORT_API RAPTORHW_RESULT raptorhw_instance_get_offset_tuning(raptorhw_instance_t* device);

/// <summary>
/// Sets if offset tuning is enabled on the device. Only usable if RAPTORHW_CAPABILITY_OFFSET_TUNING is set in the device capabilities.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <param name="enabled">0 or 1, 1 being enabled and 0 being disabled.</param>
/// <returns>0 or 1. 1 is returned if the operation was successful, otherwise 0.</returns>
EXPORT_API RAPTORHW_RESULT raptorhw_instance_set_offset_tuning(raptorhw_instance_t* device, RAPTORHW_RESULT enabled);

/// <summary>
/// Gets if direct sampling is enabled on the device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>0 or 1. 1 is returned if direct sampling is on, otherwise 0.</returns>
EXPORT_API RAPTORHW_RESULT raptorhw_instance_get_direct_sampling(raptorhw_instance_t* device);

/// <summary>
/// Sets if direct sampling is enabled on the device. Only usable if RAPTORHW_CAPABILITY_DIRECT_SAMPLING is set in the device capabilities.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <param name="enabled">0 or 1, 1 being enabled and 0 being disabled.</param>
/// <returns>0 or 1. 1 is returned if the operation was successful, otherwise 0.</returns>
EXPORT_API RAPTORHW_RESULT raptorhw_instance_set_direct_sampling(raptorhw_instance_t* device, RAPTORHW_RESULT enabled);

/// <summary>
/// Requests the device's supported sample rates. Result is an array you must preallocate. Calling this function with max==0 will return the number of items to preallocate.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <param name="result">A preallocated array, length max, of ints that the result will be written to.</param>
/// <param name="max">The maximum number of items to write to result. Calling with 0 will return the number of items present.</param>
/// <returns>The number of values written to result if max != 0. If max == 0, returns the total number of items.</returns>
EXPORT_API int raptorhw_instance_get_supported_samplerates(raptorhw_instance_t* device, int* result, int max);

/// <summary>
/// Gets the current sample rate of the device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>The current device sample rate.</returns>
EXPORT_API int raptorhw_instance_get_samplerate(raptorhw_instance_t* device);

/// <summary>
/// Sets the current sample rate to the value specified.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <param name="index">The index of the sample rate in the list of rates.</param>
/// <returns>0 or 1. 1 is returned if the operation was successful, otherwise 0.</returns>
EXPORT_API RAPTORHW_RESULT raptorhw_instance_set_samplerate(raptorhw_instance_t* device, int index);

/// <summary>
/// Get the number of gain settings available.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>The number of gains accessible with raptorhw_instance_get_gains_at.</returns>
EXPORT_API int raptorhw_instance_get_gain_group_count(raptorhw_instance_t* device);

/// <summary>
/// Gets a gain setting at the specified index. The pointer is valid until the device handle is destroyed.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns>If index is valid, a pointer to the gain setting, valid until the device handle is destroyed with raptorhw_instance_close. Otherwise, NULL.</returns>
EXPORT_API raptorhw_gain_group_t* raptorhw_instance_get_gain_group_at(raptorhw_instance_t* device, int index);

/// <summary>
/// Gets the number of custom properties on the device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <returns></returns>
EXPORT_API int raptorhw_instance_get_custom_property_count(raptorhw_instance_t* device);

/// <summary>
/// Gets a pointer to the custom property at the specified index.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <param name="index">The index of the property, starting at 0.</param>
/// <returns></returns>
EXPORT_API raptorhw_custom_property_t* raptorhw_instance_get_custom_property_at(raptorhw_instance_t* device, int index);

/// <summary>
/// Starts receiving samples asynchronously. For each sample buffer, the callback will be called. Ran until raptorhw_instance_stop_rx.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
/// <param name="callback">The callback fired for each block of samples.</param>
/// <param name="ctx">The context object passed into the callback when it's fired.</param>
/// <returns>0 or 1. 1 is returned if the operation was successful, otherwise 0.</returns>
EXPORT_API RAPTORHW_RESULT raptorhw_instance_start_rx(raptorhw_instance_t* device, raptorhw_instance_streaming_cb callback, void* ctx);

/// <summary>
/// Stops the receiving samples process started by raptorhw_instance_start_rx.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
EXPORT_API void raptorhw_instance_stop_rx(raptorhw_instance_t* device);

/// <summary>
/// Stops streaming, destroys the device handle, and frees all resources used by the device.
/// </summary>
/// <param name="device">The device instance handled obtained from raptorhw_candidate_create_instance.</param>
EXPORT_API void raptorhw_instance_close(raptorhw_instance_t* device);

/* CUSTOM PROPERTY METHODS */

EXPORT_API size_t raptorhw_customprop_get_name(raptorhw_custom_property_t* prop, char* result, size_t resultLen);
EXPORT_API int raptorhw_customprop_get_flags(raptorhw_custom_property_t* prop);
EXPORT_API int raptorhw_customprop_read_string(raptorhw_custom_property_t* prop, char* result, size_t resultLen, size_t* resultWritten);
EXPORT_API int raptorhw_customprop_write_string(raptorhw_custom_property_t* prop, char* input, size_t inputLen);
EXPORT_API int raptorhw_customprop_read_int(raptorhw_custom_property_t* prop, int* result);
EXPORT_API int raptorhw_customprop_write_int(raptorhw_custom_property_t* prop, int value);

/* GAIN GROUP METHODS */

EXPORT_API size_t raptorhw_gain_group_get_name(raptorhw_gain_group_t* group, char* result, size_t resultLen);
EXPORT_API int raptorhw_gain_group_get_option_count(raptorhw_gain_group_t* group);
EXPORT_API raptorhw_gain_t* raptorhw_gain_group_get_option(raptorhw_gain_group_t* group, int index);
EXPORT_API int raptorhw_gain_group_select(raptorhw_gain_group_t* group);

EXPORT_API size_t raptorhw_gain_item_get_name(raptorhw_gain_t* item, char* result, size_t resultLen);
EXPORT_API int raptorhw_gain_item_get_units(raptorhw_gain_t* item);
EXPORT_API int raptorhw_gain_item_get_agc_supported(raptorhw_gain_t* item);
EXPORT_API int raptorhw_gain_item_get_agc(raptorhw_gain_t* item);
EXPORT_API int raptorhw_gain_item_set_agc(raptorhw_gain_t* item, int enabled);
EXPORT_API int raptorhw_gain_item_get_option_count(raptorhw_gain_t* item);
EXPORT_API float raptorhw_gain_item_get_option_label(raptorhw_gain_t* item, int index);
EXPORT_API int raptorhw_gain_item_get_value(raptorhw_gain_t* item);
EXPORT_API int raptorhw_gain_item_set_value(raptorhw_gain_t* item, int index);