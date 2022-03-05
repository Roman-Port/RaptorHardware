#include <raptorhw_gain.h>
#include <cassert>

raptorhw_gain_option_impl::~raptorhw_gain_option_impl() {

}

raptorhw_gain_group_impl::raptorhw_gain_group_impl() {
	//Clear gains
	gains_count = 0;
	for (int i = 0; i < RAPTORHW_MAX_GAIN_OPTIONS; i++)
		gains[i] = 0;
}

raptorhw_gain_group_impl::~raptorhw_gain_group_impl() {
	//Free gains
	for (int i = 0; i < gains_count; i++) {
		delete gains[i];
		gains[i] = 0;
	}
	gains_count = 0;
}

int raptorhw_gain_group_impl::get_option_count() {
	return gains_count;
}

void raptorhw_gain_group_impl::put_option(raptorhw_gain_option_impl* option) {
	assert(gains_count < RAPTORHW_MAX_GAIN_OPTIONS);
	gains[gains_count++] = option;
}

raptorhw_gain_option_impl* raptorhw_gain_group_impl::get_option(int index) {
	if (index < 0 || index >= gains_count)
		return 0;
	else
		return gains[index];
}

bool raptorhw_gain_group_impl::select() {
	//Reapply all options
	bool success = true;
	for (int i = 0; i < gains_count; i++) {
		if (gains[i]->get_agc())
			success = gains[i]->set_agc(true) && success;
		else
			success = gains[i]->set_value(gains[i]->get_value()) && success;
	}
	return success;
}