#include "defines.h"

const char* raptorhw_gain_group_get_name(raptorhw_gain_group_t* group) {
	return group->get_name();
}

int raptorhw_gain_group_get_count(raptorhw_gain_group_t* group) {
	return group->get_setting_count();
}

raptorhw_gain_t* raptorhw_gain_group_get_item(raptorhw_gain_group_t* group, int index) {
	return group->get_setting(index);
}

int raptorhw_gain_group_select(raptorhw_gain_group_t* group) {
	return group->select();
}

const char* raptorhw_gain_item_get_name(raptorhw_gain_t* item) {
	return item->get_name();
}

int raptorhw_gain_item_get_agc_supported(raptorhw_gain_t* item) {
	return item->get_agc_supported();
}

int raptorhw_gain_item_get_agc(raptorhw_gain_t* item) {
	return item->get_agc();
}

int raptorhw_gain_item_set_agc(raptorhw_gain_t* item, int enabled) {
	return item->set_agc(enabled);
}

int raptorhw_gain_item_get_value_count(raptorhw_gain_t* item) {
	return item->get_value_count();
}

void raptorhw_gain_item_get_value_label(raptorhw_gain_t* item, int index, char* result, size_t resultLen) {
	item->get_value_label(index, result, resultLen);
}

int raptorhw_gain_item_get_value(raptorhw_gain_t* item) {
	return item->get_value();
}

int raptorhw_gain_item_set_value(raptorhw_gain_t* item, int index) {
	return item->set_value(index);
}