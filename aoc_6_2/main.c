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
int split2int(char* str, const char* delim, unsigned long int* values, unsigned int* values_counter);

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
int split2int(char* str, const char* delim, unsigned long int* values, unsigned int* values_counter) {
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

		values[i] = (unsigned long int) strtol(token, &end, 10);
                ASSERT(token != end && errno == 0, err0, -2, "Invalid input (non integer value).\n");

		i++;
	}

	*values_counter = i;

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
	unsigned long int values[1000];
	unsigned int values_counter = 1000;

	read = getline(&line, &len, input_file);
	ASSERT(read != -1, err1, -5, "Invalid input (empty)\n");
	err = split2int(line, ",", values, &values_counter);
	ASSERT(err == 0 && values_counter > 0, err1, err, "Invalid input.\n");


	unsigned long int current_values[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned long int new_values[] =	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int TIME = 256;

	for (i = 0; i < values_counter; i++) {
		current_values[values[i]]++;
	}

	// Progress over time
	for (i = 0; i < TIME; i++) {
		new_values[0] = current_values[1];
		new_values[1] = current_values[2];
		new_values[2] = current_values[3];
		new_values[3] = current_values[4];
		new_values[4] = current_values[5];
		new_values[5] = current_values[6];
		new_values[6] = current_values[7] + current_values[0];
		new_values[7] = current_values[8];
		new_values[8] = current_values[0];
		memcpy(current_values, new_values, 9*sizeof(unsigned long int));
	}

	// Summ all
	unsigned long int result = 0;
	for(i = 0; i < 9; i++) {
		result += current_values[i];
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

