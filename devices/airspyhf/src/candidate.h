#include "internal.h"
#include <raptorhw_candidate.h>

class airspyhf_candidate : public raptorhw_candidate_impl {

public:
	airspyhf_candidate(uint64_t serial);
	virtual size_t get_name(char* result, size_t resultMax) override;
	virtual size_t get_serial(char* result, size_t resultMax) override;
	virtual int get_capabilities() override;
	virtual raptorhw_device_impl* open() override;

private:
	uint64_t serial;

};