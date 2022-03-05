#include "info.h"
#include "../lib/include/rtl-sdr.h"

bool query_rtlsdr_info(uint32_t index, raptor_rtlsdr_info_t* result) {
	result->index = index;
	return rtlsdr_get_device_usb_strings(index, result->manufacturer, result->product, result->serial) == 0;
}