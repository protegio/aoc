#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

#define ASSERT(expression, label, err_code, error_msg) do { if (!(expression)) { printf(error_msg); err = err_code; goto label; } } while(0)
#define ABS_DIFF(a ,b) (a > b ? a - b : b - a)

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

typedef struct {
	unsigned long int x;
	unsigned long int y;
} point;

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

	// read the segments
	unsigned int inputs_size = 10;
	char inputs[1000][10][8];
	unsigned int outputs_size = 4;
	char outputs[1000][4][8];
	unsigned int entry_counter = 0;
	unsigned int i = 0;
	unsigned int size_counters[8];

	memset(inputs, 0, 1000*10*8*sizeof(char));
	memset(inputs, 0, 1000*4*8*sizeof(char));
	memset(size_counters, 0, 7*sizeof(unsigned int));

	while ((read = getline(&line, &len, input_file)) != -1) {
		unsigned int tokens_size = 2;
		char tokens[2][10*8];
		if (line[read-1] == '\n') {
			line[read-1] = '\0';
		}
		err = split(line, "|", (char**) &tokens, &tokens_size, 10*8);
	        ASSERT(err == 0 && tokens_size == 2, err1, err, "Invalid input.\n");

		err = split(tokens[0], " ", (char**) &inputs[entry_counter], &inputs_size, 8);
		ASSERT(err == 0 && inputs_size == 10, err1, err, "Invalid input.\n");

                err = split(tokens[1], " ", (char**) &outputs[entry_counter], &outputs_size, 8);
                ASSERT(err == 0 && outputs_size == 4, err1, err, "Invalid input.\n");

		for (i = 0; i < inputs_size; i++) {
			printf("%s ", inputs[entry_counter][i]);
		}

		printf("| ");

		for (i = 0; i < outputs_size; i ++) {
			unsigned int len = strlen(outputs[entry_counter][i]);
			size_counters[len]++;
			printf("%s(%u) ", outputs[entry_counter][i], strlen(outputs[entry_counter][i]));
		}

		printf("\n");

		entry_counter++;
	}

	printf("Entry counter: %u\n", entry_counter);

	//           result = (1 counter)      + (7 counter)     + (4 counter)     + (8 counter)
	unsigned int result = size_counters[2] + size_counters[3] + size_counters[4] + size_counters[7];
	printf("1 (2): %u\n", size_counters[2]);
	printf("4 (5): %u\n", size_counters[4]);
	printf("7 (3): %u\n", size_counters[3]);
	printf("8 (7): %u\n", size_counters[7]);
	printf("Result: %u\n", result);

err1:
	if (line != NULL) {
		free(line);
	}

	fclose(input_file);

err0:
	return err;
}

