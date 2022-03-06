#include <raptorhw_custom_property.h>
#include <raptorhw_util.h>

raptorhw_custom_property_impl::raptorhw_custom_property_impl(const char* name, int flags) :
	name(name),
	flags(flags)
{

}

raptorhw_custom_property_impl::~raptorhw_custom_property_impl() {

}

size_t raptorhw_custom_property_impl::get_name(char* result, size_t resultMax) {
	return raptorhw_strcpy(result, name, resultMax);
}

int raptorhw_custom_property_impl::get_flags() {
	return flags;
}

bool raptorhw_custom_property_impl::read_string(char* result, size_t resultMax, size_t* written) {
	(*written) = 0;
	return false;
}

bool raptorhw_custom_property_impl::write_string(char* input, size_t len) {
	return false;
}

bool raptorhw_custom_property_impl::read_int(int* result) {
	(*result) = 0;
	return false;
}

bool raptorhw_custom_property_impl::write_int(int value) {
	return false;
}