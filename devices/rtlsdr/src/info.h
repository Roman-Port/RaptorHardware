#pragma once

#include <stdint.h>

#define RTLSDR_INFO_BUFFER_SIZE 257

struct raptor_rtlsdr_info_t {

	uint32_t index;
	char manufacturer[RTLSDR_INFO_BUFFER_SIZE];
	char product[RTLSDR_INFO_BUFFER_SIZE];
	char serial[RTLSDR_INFO_BUFFER_SIZE];

};

bool query_rtlsdr_info(uint32_t index, raptor_rtlsdr_info_t* result);