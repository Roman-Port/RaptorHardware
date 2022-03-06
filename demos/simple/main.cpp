#include <stdio.h>
#include <raptorhw.h>

void print_device_list(raptorhw_candidate_t** devices) {
	char name[256];
	char serial[256];
	printf("=== CONNECTED DEVICES\n");
	for (int i = 0; devices[i] != 0; i++) {
		//Query device name and serial
		raptorhw_candidate_get_name(devices[i], name, sizeof(name));
		raptorhw_candidate_get_serial(devices[i], serial, sizeof(serial));

		//Get capabilities
		int capabilities = raptorhw_candidate_get_capabilities(devices[i]);

		//Print info
		printf("    %i: %s (%s) - %i\n", i, name, serial, capabilities);
	}
}

void stream_handler(raptorhw_instance_t* device, void* ctx, raptorhw_complex_t* samples, int sampleCount) {
	//Get the file
	FILE* file = (FILE*)ctx;

	//Write
	fwrite(samples, sizeof(raptorhw_complex_t), sampleCount, file);
}

int main() {
	//Get a context
	raptorhw_context_t* ctx = raptorhw_context_create();

	//Search for devices (which are valid until we destroy them)
	raptorhw_candidate_t** devices = raptorhw_context_search(ctx);

	//Print out devices
	print_device_list(devices);

	//Only continue if there's more than one device
	if (devices[0] != 0) {
		//Open the first device
		raptorhw_instance_t* device = raptorhw_candidate_open(devices[0]);

		//Query device name and serial to print header
		char name[256];
		char serial[256];
		raptorhw_instance_get_name(device, name, sizeof(name));
		raptorhw_instance_get_serial(device, serial, sizeof(serial));
		printf("=== OPENED %s (%s)\n", name, serial);

		//Set the sample rate to the first index and tune
		raptorhw_instance_set_samplerate(device, 0);
		raptorhw_instance_set_freq(device, 107900000);

		//Open a file to write to
		FILE* result = fopen("output.bin", "wb");

		//Begin streaming asyncronously and pass the file as the context object
		raptorhw_instance_start_rx(device, stream_handler, result);

		//Print info
		printf("=== STARTED STREAMING AT %i...\n    (press enter to stop)\n", raptorhw_instance_get_samplerate(device));

		//Wait for an enter key to be pressed to stop
		getchar();

		//Stop streaming (this'll hang until it's done, which won't be long)
		raptorhw_instance_stop_rx(device);

		//Close the file
		fclose(result);
	}
	else {
		//Log warning
		printf("NO DEVICES ARE CONNECTED\n");
	}

	//Destroy the search query
	raptorhw_context_search_destroy(ctx, devices);

	//Destroy the device context
	raptorhw_context_destroy(ctx);

	return 0;
}