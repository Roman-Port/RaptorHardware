#define CREATE_GAIN(name, displayName, func, maxValue) class name : public raptorhw_gain_option_impl { \
public:\
name(airspy_device* device) : device(device), value(0) {}\
virtual size_t get_name(char* result, size_t resultSize) override { return raptorhw_strcpy(result, displayName, resultSize); }\
virtual bool get_agc_supported() override { return false; }\
virtual int get_unit() override { return 0; }\
virtual bool get_agc() override { return false; }\
virtual bool set_agc(bool enabled) override { return false; }\
virtual int get_option_count() override { return maxValue + 1; }\
virtual float get_option_value(int index) override { return index; }\
virtual int get_value() override { return value; }\
virtual bool set_value(int index) override { if (func(device, index) != 0) { return false; } value = index; return true; }\
private:\
airspy_device* device;\
int value;\
}

#define CREATE_GAIN_AGC(name, displayName, func, agcFunc, maxValue) class name : public raptorhw_gain_option_impl { \
public:\
name(airspy_device* device) : device(device), value(0), agc_enabled(false) {}\
virtual size_t get_name(char* result, size_t resultSize) override { return raptorhw_strcpy(result, displayName, resultSize); }\
virtual bool get_agc_supported() override { return true; }\
virtual int get_unit() override { return 0; }\
virtual bool get_agc() override { return agc_enabled; }\
virtual bool set_agc(bool enabled) override { if (agcFunc(device, enabled) != 0) { return false; } agc_enabled = enabled; return true; }\
virtual int get_option_count() override { return maxValue + 1; }\
virtual float get_option_value(int index) override { return index; }\
virtual int get_value() override { return value; }\
virtual bool set_value(int index) override { if (agc_enabled) { set_agc(false); } if (func(device, index) != 0) { return false; } value = index; return true; }\
private:\
airspy_device* device;\
bool agc_enabled;\
int value;\
}

#define CREATE_GAINGROUP_START(name, displayName) class name : public raptorhw_gain_group_impl {\
public:\
virtual size_t get_name(char* result, size_t resultSize) override {\
return raptorhw_strcpy(result, displayName, resultSize);\
}\
name(airspy_device* device) {

#define CREATE_GAINGROUP_ADD(name) put_option(new name(device));
#define CREATE_GAINGROUP_END() }};