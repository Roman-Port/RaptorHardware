#include "context.h"
#ifdef RAPTORHW_DEVICE_AIRSPY
#include <raptorhw_device_airspy.h>
#endif
#ifdef RAPTORHW_DEVICE_AIRSPYHF
#include <raptorhw_device_airspyhf.h>
#endif
#ifdef RAPTORHW_DEVICE_RTLSDR
#include <raptorhw_device_rtlsdr.h>
#endif

void raptorhw_context_impl::initialize_modules() {
#ifdef RAPTORHW_DEVICE_AIRSPY
	add_module(raptorhw_device_airspy_create());
#endif
#ifdef RAPTORHW_DEVICE_AIRSPYHF
	add_module(raptorhw_device_airspyhf_create());
#endif
#ifdef RAPTORHW_DEVICE_RTLSDR
	add_module(raptorhw_device_rtlsdr_create());
#endif
}