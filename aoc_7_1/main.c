#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

#define ASSERT(expression, label, err_code, error_msg) do { if (!(expression)) { printf(error_msg); err = err_code; goto label; } } while(0)
#define ABS_DIFF(a, b) (a > b ? a - b : b - a)

extern int errno;

int split(char* str, const char* delim, char** values, unsigned int* values_counter, unsigned int token_max_size);
int split2int(char* str, const char* delim, unsigned long int* values, unsigned int* values_counter, unsigned long int *max, unsigned long int *min);

int split(char* str, const char* delim, char** values, unsigned int* values_counter, unsigned int token_max_size) {
        int err = 0;
        unsigned int i = 0;
        char *token = 0;
        char *end = 0;

        while ((token = strsep(&str, delim))) {
                // skip empty tokens
                int token_len = strlen(token);
                if (token_len == 0 ) {
                        continue;
                }

                ASSERT(i < *values_counter, err0, -1, "Buffer oveflow\n");
                strcpy((char*) values+(i*token_max_size), token);
                i++;
        }

        *values_counter = i;

err0:
        return err;
}

/**
 * @brief Split the str into an array of integers.
 */
int split2int(char* str, const char* delim, unsigned long int* values, unsigned int* values_counter, unsigned long int *max, unsigned long int *min) {
	int err = 0;
	unsigned int i = 0;
	char *token = 0;
	char *end = 0;
	
	unsigned long int max_value = 0;
	unsigned long int min_value = ULONG_MAX;
	while ((token = strsep(&str, delim))) {
		// skip empty tokens
		int token_len = strlen(token);
		if (token_len == 0 ) {
			continue;
		}

		ASSERT(i < *values_counter, err0, -1, "Buffer oveflow\n");

		values[i] = (unsigned long int) strtol(token, &end, 10);
		ASSERT(token != end && errno == 0, err0, -2, "Invalid input (non integer value).\n");
		max_value = (values[i] > max_value ? values[i] : max_value);
		min_value = (values[i] < min_value ? values[i] : min_value);

		i++;
	}

	
	*values_counter = i;

	if (min != 0) {
		*min = min_value;
	}

	if (max != 0) {
		*max = max_value;
	}
err0:
	return err;
}

int main() {
	int err = 0;
	
	// Open the input file
	FILE *input_file = NULL;
	input_file = fopen("input.txt", "r");
	ASSERT(input_file != NULL, err0, -1, "Failed to open the input file.\n");

	size_t len = 0;
        ssize_t read = 0;
        char *line = NULL;
        char *end = NULL;
	unsigned long int value = 0;
	unsigned long int values[4096]; // 32kb? pff!
	unsigned int values_counter = 4096;
	unsigned long int max = 0;

	read = getline(&line, &len, input_file);
	ASSERT(read != -1, err1, -5, "Invalid input (empty)\n");
	err = split2int(line, ",", values, &values_counter, &max, 0);
	ASSERT(err == 0 && values_counter > 0, err1, err, "Invalid input.\n");

	unsigned long int current_values[2048];
	unsigned int i = 0;
	unsigned int j = 0;

	memset(current_values, 0, 2048*sizeof(unsigned long int));
	for (i = 0; i < values_counter; i++) {
		current_values[values[i]]++;
	}


	unsigned long int result = ULONG_MAX;
	unsigned long int fuel = 0;
	// fuel for position 0
	for (i = 0; i <= max; i++) {
		fuel += (current_values[i]*(ABS_DIFF(i, 0)));
	}

	// check all other positions step-by-step
	result = fuel;
	unsigned left = 0;
	unsigned right = values_counter;
	for (i = 1; i <= max; i++) {
		left += current_values[i-1];
		right -= current_values[i-1];
		unsigned long int new_fuel = fuel - right + left;
		result = (new_fuel < result ? new_fuel : result);
		fuel = new_fuel;
	}
	printf("Result: %lu\n", result);

err1:
	if (line != NULL) {
		free(line);
	}

	fclose(input_file);

err0:
	return err;
}

