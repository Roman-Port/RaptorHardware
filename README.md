# Raptor Hardware

Raptor Hardware is a convenience library that will automatically abstract away libraries for various software defined radios (SDRs), allowing you to interact with them without worrying about their specific implementations.

This will allow your programs to easily interact with a variety of SDRs just by using a single interface.

Since all radio-specific libraries are built internally by this one, any user won't need the respective libraries installed either. It's all included in this library.

## Usage

Once the library is installed, you can simply include it with CMake using ``find_package(raptorhw REQUIRED)`` and linking with ``target_link_libraries(<target> raptorhw)``. Include the header ``#include <raptorhw.h>``.

The usage of the library follows these principles:

* Get a context object with ``raptorhw_context_create`` to hold onto for the lifetime of your application.
* Request a null-terminated list of connected devices with ``raptorhw_context_search``.
* Enumerate through devices to find the one you want to use with the capabilities you want. Open the one you'd like to use with ``raptorhw_candidate_open``. After this call, you can free the list of devices safely.
* Set properties of the device you just opened, like the sample rate (``raptorhw_instance_set_samplerate``) or center frequency (``raptorhw_instance_set_freq``).
* Start asynchronously receiving samples to a callback function on another thread with ``raptorhw_instance_start_rx``.
* Use the device and set settings as usual.
* When you're done with the device, stop rx with ``raptorhw_instance_stop_rx``. The worker thread will end and this function will return.
* Close the device with ``raptorhw_instance_close``.
* Finally, after you're done with the library, close it with ``raptorhw_context_destroy``.

## Android

Android, while **not supported just yet**, is expected to be implemented soon. This'll allow you to provide a callback in which you'll need to show the device access dialog in Android. More details will come soon!

## Building

Since all device libraries are built statically when building this library, builds can take a little bit. Most building follows the normal procedure, but importantly you'll **need to clone this repo with submodules**.

When cloning, run the following command:

```bash
git clone --recursive https://github.com/Roman-Port/RaptorHardware.git
```

If you already cloned the repo but forgot to include submodules, it won't build. Fear not! Run the following command to download them:

```bash
git submodule update --init --recursive
```

From here, it should be standard procedure:

```bash
mkdir build
cd build
cmake ..
make
sudo make install
```

## Reference

### raptorhw_context_t

This type is a global object that should exist for the lifetime of your application.

* ``raptorhw_context_create`` - Creates the context.
* ``raptorhw_context_get_module_count`` - Gets the number of modules installed.
* ``raptorhw_context_get_module_info`` - Requests the details about an installed module using it's index.
* ``raptorhw_context_search`` - Returns a pointer to a null-terminated list of pointers to ``raptorhw_candidate_t`` objects.
* ``raptorhw_context_search_destroy`` - Destroys the list of candidates from ``raptorhw_context_search``, **including the candidates themselves**.
* ``raptorhw_context_destroy`` - Destroys the context object.

### raptorhw_candidate_t

This type represents a potential device that's connected but not yet opened.

* ``raptorhw_candidate_get_name`` - Gets the name of the device.
* ``raptorhw_candidate_get_serial`` - Gets the serial number of the device.
* ``raptorhw_candidate_get_capabilities`` - Gets the capabilities of the device. Use the RAPTORHW_CAPABILITY_* macros to compare bit flags against.
* ``raptorhw_candidate_open`` - Opens the device fully and returns a ``raptorhw_instance_t`` pointer on succcess. After this call, it's safe to free the candidate list.

### raptorhw_instance_t

This type is the core device that you'll interact with. It's created by calling ``raptorhw_candidate_open`` on a candidate.


* ``raptorhw_instance_get_name`` - Gets the name of the device.
* ``raptorhw_instance_get_serial`` - Gets the serial number of the device.
* ``raptorhw_instance_get_capabilities`` - Gets the capabilities of the device. Use the RAPTORHW_CAPABILITY_* macros to compare bit flags against.
* ``raptorhw_instance_get_min_freq`` - Gets the **estimated** minimum tunable frequency, in Hz, the device supports. It may be inaccurate.
* ``raptorhw_instance_get_max_freq`` - Gets the **estimated** maximum tunable frequency, in Hz, the device supports. It may be inaccurate.
* ``raptorhw_instance_get_freq`` - Gets the current center frequency.
* ``raptorhw_instance_set_freq`` - Sets the current center frequency. Returns 1 on success, otherwise 0.
* ``raptorhw_instance_get_bias_t`` - Gets if Bias-T is enabled (1 if on, 0 if off).
* ``raptorhw_instance_set_bias_t`` - Sets if Bias-T is enabled (1 if on, 0 if off). May not be supported, check the capabilities. Returns 1 on success, otherwise 0.
* ``raptorhw_instance_get_offset_tuning`` - Gets if offset tuning is enabled (1 if on, 0 if off).
* ``raptorhw_instance_set_offset_tuning`` - Sets if offset tuning is enabled (1 if on, 0 if off). May not be supported, check the capabilities. Returns 1 on success, otherwise 0.
* ``raptorhw_instance_get_direct_sampling`` - Gets if direct sampling is enabled (1 if on, 0 if off).
* ``raptorhw_instance_set_direct_sampling`` - Sets if direct sampling is enabled (1 if on, 0 if off). May not be supported, check the capabilities. Returns 1 on success, otherwise 0.
* ``raptorhw_instance_get_supported_samplerates`` - Gets the supported sample rates by filling an array. Returns the number of sample rates. Call with 0 length to get the number of rates without writing anything, then call again after allocating the array.
* ``raptorhw_instance_get_samplerate`` - Gets the device's current sample rate.
* ``raptorhw_instance_set_samplerate`` - Sets the device's current sample rate to the value **at the index you pass**. Returns 1 on success, otherwise 0.
* ``raptorhw_instance_get_gain_group_count`` - Gets the number of gain groups the device has.
* ``raptorhw_instance_get_gain_group_at`` - Gets the ``raptorhw_gain_group_t`` gain group at the specified index, valid until the device is closed.
* ``raptorhw_instance_get_custom_property_count`` - Gets the number of custom properties the device has.
* ``raptorhw_instance_get_custom_property_at`` - Gets the ``raptorhw_custom_property_t`` custom property at the specified index, valid until the device is closed.
* ``raptorhw_instance_start_rx`` - Starts asynchronously receiving samples on a different thread. Pass in a callback to be called for each block of samples.
* ``raptorhw_instance_stop_rx`` - Stops receiving samples and the worker thread associated, blocking until the worker is done.
* ``raptorhw_instance_close`` - Destroys the device, stopping rx if it is active.

### raptorhw_custom_property_t

Custom properties represent device-specific information or settings that don't fit into the normal interface. They're entirely optional. You can get a pointer to them from ``raptorhw_instance_get_custom_property_at``.

Properties may be a string or an integer and the flags will specifiy which. They can also be read-write or just read-only.

* ``raptorhw_customprop_get_name`` - Gets the display name of the custom property.
* ``raptorhw_customprop_get_flags`` - Gets the flags specifying info about the property. Use the RAPTORHW_CUSTOMPROP_* macros to compare bitflags.
* ``raptorhw_customprop_read_string`` - Reads the value as a string if the property supports it. Returns 1 if successful, 0 if not (or not supported). Use the flags to check if this type is used.
* ``raptorhw_customprop_write_string`` - Writes to the value as a string if the property supports it. Returns 1 if successful, 0 if not (or not supported). Use the flags to check if this type is used.
* ``raptorhw_customprop_read_int`` - Reads the value as an int if the property supports it. Returns 1 if successful, 0 if not (or not supported). Use the flags to check if this type is used.
* ``raptorhw_customprop_write_int`` - Writes to the value as an int if the property supports it. Returns 1 if successful, 0 if not (or not supported). Use the flags to check if this type is used.

### raptorhw_gain_group_t

Gain groups contain a list of gain settings that fall into a "mode" of the device. Most devices only have a single entry. The exception is the AirSpy device, which supports three different modes of gain control.

If displaying in a user interface, it's recommended to not even show the name of the group or that options are even grouped unless there is more than one group on the device.

* ``raptorhw_gain_group_get_name`` - Gets the display name of the group.
* ``raptorhw_gain_group_get_option_count`` - Gets the number of options in the group.
* ``raptorhw_gain_group_get_option`` - Gets a pointer to a ``raptorhw_gain_t`` object at the specified index. Valid as long as the device is.
* ``raptorhw_gain_group_select`` - Called whenever the selected group is changed. Returns 1 on success, otherwise 0.

### raptorhw_gain_t

This type is a gain setting that the user can manipulate. Gain settings can also (optionally) allow either automatic (AGC) or manual control. A function exists to check if this is supported.

Since the meaning of the gain varies from device to device, gain settings are addressed with an index. To display options, a label can be retrieved for each of the indices. Additionally, a unit associated can also be retrieved for display purposes.

* ``raptorhw_gain_item_get_name`` - Gets the display name of the setting.
* ``raptorhw_gain_item_get_units`` - Gets the unit of the item for display purposes. Compare against the RAPTORHW_UNITS_* macros.
* ``raptorhw_gain_item_get_agc_supported`` - Checks if AGC is supported on this device. Returns 1 if it is, or 0 if it is manual control only.
* ``raptorhw_gain_item_get_agc`` - If supported, returns if AGC mode is on. (1 is on, 0 is off)
* ``raptorhw_gain_item_set_agc`` - If supported, sets if AGC mode is on. (1 is on, 0 is off) Returns 1 if successful, otherwise 0.
* ``raptorhw_gain_item_get_option_count`` - Gets the number of available options to pick from.
* ``raptorhw_gain_item_get_option_label`` - Gets the label of an option at the specified index in units specified by ``raptorhw_gain_item_get_units``.
* ``raptorhw_gain_item_get_value`` - Gets the currently selected manual index.
* ``raptorhw_gain_item_set_value`` - Sets the currently selected manual index.