#include <raptorhw.h>
#include <stdio.h>
#include <stdlib.h>

int prompt_number() {
	char line[32];
	int index = 0;
	int temp = 0;
	while (index < 31) {
		//Read
		temp = getchar();

		//If it's a new line and we have text, accept
		if (temp == '\n' && index > 0)
			break;

		//Skip anything except for 0-9
		if (temp != '0' && temp != '1' && temp != '2' && temp != '3' && temp != '4' && temp != '5' && temp != '6' && temp != '7' && temp != '8' && temp != '9')
			continue;

		//Add
		line[index++] = (char)temp;
	}

	line[index++] = 0;
	return atoi(line);
}

int prompt_index(int count) {
	int index;
	do {
		index = prompt_number() - 1;
	} while (index < 0 || index >= count);
	return index;
}

void print_capabilities(int capabilities) {
	if (capabilities & RAPTORHW_CAPABILITY_BIAS_T)
		printf("        RAPTORHW_CAPABILITY_BIAS_T\n");
	if (capabilities & RAPTORHW_CAPABILITY_OFFSET_TUNING)
		printf("        RAPTORHW_CAPABILITY_OFFSET_TUNING\n");
	if (capabilities & RAPTORHW_CAPABILITY_DIRECT_SAMPLING)
		printf("        RAPTORHW_CAPABILITY_DIRECT_SAMPLING\n");
}

void print_candidate(int number, raptorhw_candidate_t* data) {
	printf("DEVICE #%i:\n", number);
	printf("    NAME: %s\n", raptorhw_candidate_get_name(data));
	printf("    SERIAL: %s\n", raptorhw_candidate_get_serial(data));
	printf("    CAPABILITIES: %i\n", raptorhw_candidate_get_capabilities(data));
	print_capabilities(raptorhw_candidate_get_capabilities(data));
}

void print_gain_group(int number, raptorhw_gain_group_t* group) {
	printf("        #%i - %s:\n", number, raptorhw_gain_group_get_name(group));
	raptorhw_gain_t* gain;
	char temp[32];
	for (int i = 0; i < raptorhw_gain_group_get_count(group); i++) {
		gain = raptorhw_gain_group_get_item(group, i);
		printf("            #%i: %s", i + 1, raptorhw_gain_item_get_name(gain));
		if (raptorhw_gain_item_get_agc_supported(gain))
			printf(" (AGC)");
		printf("\n");
		for (int ii = 0; ii < raptorhw_gain_item_get_value_count(gain); ii++) {
			raptorhw_gain_item_get_value_label(gain, ii, temp, sizeof(temp));
			printf("                #%i: %s\n", ii + 1, temp);
		}
	}
}

void print_instance(raptorhw_instance_t* device) {
	printf("DEVICE INFO:\n");
	printf("    NAME: %s\n", raptorhw_instance_get_name(device));
	printf("    SERIAL: %s\n", raptorhw_instance_get_serial(device));
	printf("    CAPABILITIES: %i\n", raptorhw_instance_get_capabilities(device));
	print_capabilities(raptorhw_instance_get_capabilities(device));
	printf("    MIN FREQUENCY: %lli\n", raptorhw_instance_get_min_freq(device));
	printf("    MAX FREQUENCY: %lli\n", raptorhw_instance_get_max_freq(device));
	printf("    GAIN GROUPS: %i\n", raptorhw_instance_get_gain_group_count(device));
	for (int i = 0; i < raptorhw_instance_get_gain_group_count(device); i++)
		print_gain_group(i + 1, raptorhw_instance_get_gain_group_at(device, i));
}

int prompt_sample_rate(raptorhw_instance_t* device) {
	//Query sample rates
	int sampleRateCount = raptorhw_instance_get_supported_samplerates(device, 0, 0);
	int* sampleRates = (int*)malloc(sizeof(int) * sampleRateCount);
	sampleRateCount = raptorhw_instance_get_supported_samplerates(device, sampleRates, sampleRateCount);

	//Print sample rates
	printf("DEVICE SAMPLE RATES:\n");
	for (int i = 0; i < sampleRateCount; i++)
		printf("    #%i: %i\n", i + 1, sampleRates[i]);
	printf("(%i results)\n", sampleRateCount);

	//Prompt
	int rate;
	do {
		printf("Type the number of the sample rate to use.\n");
		rate = prompt_number() - 1;
	} while (rate < 0 || rate >= sampleRateCount);

	//Resolve rate from an index to the value
	rate = sampleRates[rate];

	//Clean up
	free(sampleRates);

	return rate;
}

void record_callback(raptorhw_instance_t* device, void* ctx, raptorhw_complex_t* samples, int sampleCount) {
	fwrite(samples, sizeof(raptorhw_complex_t), sampleCount, (FILE*)ctx);
}

void run_menu(raptorhw_instance_t* device) {
	while (true) {
		//Print options
		printf("==== MENU ====\n");
		printf("#1: Set gain");
		printf("#2: Exit");

		//Read option
		switch (prompt_number()) {
		case 1:
		{
			printf("Choose gain group...\n");
			raptorhw_gain_group_t* group = raptorhw_instance_get_gain_group_at(device, prompt_index(raptorhw_instance_get_gain_group_count(device)));
			printf("Choose gain value...\n");
			raptorhw_gain_t* gain = raptorhw_gain_group_get_item(group, prompt_index(raptorhw_gain_group_get_count(group)));
			printf("Choose index to set it to...\n");
			int value = prompt_index(raptorhw_gain_item_get_value_count(gain));
			printf("Setting...");
			if (raptorhw_gain_item_set_value(gain, value))
				printf("OK\n");
			else
				printf("FAILED\n");
			break;
		}
		case 2: return;
		}
	}
}

int main() {
	//Search for potential candidates
	raptorhw_candidate_t* candidates[16];
	int count = raptorhw_search(candidates, 16);

	//Loop through and render each
	for (int i = 0; i < count; i++)
		print_candidate(i + 1, candidates[i]);

	//Show end of list message and prompt
	printf("(%i results)\n", count);
	printf("Type device number to open it.\n");
	int index = prompt_number() - 1;

	//Open device
	raptorhw_instance_t* device = 0;
	if (index >= 0 && index < count) {
		printf("Opening device %s (%s)...", raptorhw_candidate_get_name(candidates[index]), raptorhw_candidate_get_serial(candidates[index]));
		device = raptorhw_candidate_create_instance(candidates[index]);
		if (device == 0)
			printf("FAILED!\n");
		else
			printf("OK!\n");
	}

	//Clean up
	raptorhw_search_delete(candidates);
	if (device == 0)
		return 0;

	//Print device info
	print_instance(device);

	//Set sample rate
	if (!raptorhw_instance_set_samplerate(device, prompt_sample_rate(device))) {
		printf("FAILED TO SET SAMPLE RATE\n");
		raptorhw_instance_close(device);
		return 0;
	}

	//Set center frequency
	printf("Tuning to 92.5 MHz...");
	if (!raptorhw_instance_set_freq(device, 92500000)) {
		printf("FAILED\n");
		raptorhw_instance_close(device);
		return 0;
	}
	else {
		printf("OK\n");
	}

	//Open output file
	printf("Opening recorded_iq.bin file...");
	FILE* output = fopen("recorded_iq.bin", "wb");
	if (output == 0) {
		printf("FAILED\n");
		raptorhw_instance_close(device);
		return 0;
	}
	else {
		printf("OK\n");
	}

	//Start recording
	printf("Starting RX...");
	if (!raptorhw_instance_start_rx(device, record_callback, output)) {
		printf("FAILED\n");
		raptorhw_instance_close(device);
		return 0;
	}
	else {
		printf("OK\n");
	}

	//Enter menu
	run_menu(device);

	//Stop RX
	printf("Stopping RX...");
	raptorhw_instance_stop_rx(device);
	printf("OK\n");

	//Close file
	printf("Closing file...");
	if (fclose(output))
		printf("OK\n");
	else
		printf("FAILED\n");

	//Close device
	printf("Closing device...");
	raptorhw_instance_close(device);
	printf("OK\n");

	return 0;
}