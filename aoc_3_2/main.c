#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

#define ASSERT(expression, label, err_code, error_msg) do { if (!(expression)) { printf(error_msg); err = err_code; goto label; } } while(0)

extern int errno;

int main() {
	int err = 0;
	
	// Open the input file
	FILE *input_file = NULL;
	input_file = fopen("input.txt", "r");
	ASSERT(input_file != NULL, err0, -1, "Failed to open the input file.\n");

	// Read all values from the input file
	size_t len = 0;
        ssize_t read = 0;
        char *line = NULL;
        char *end = NULL;
	unsigned long int value = 0;
	unsigned int all_values_counter = 0;
	unsigned int one_counter = 0;
	unsigned int zero_counter = 0;
	unsigned long int ones_values[1000];
        unsigned long int zeros_values[1000];
	unsigned long int all_values[1000];
	while ((read = getline(&line, &len, input_file)) != -1) {
		value = (unsigned long int) strtol(line, &end, 2);
		ASSERT(line != end && errno == 0, err1, -2, "Invalid input (non integer value).\n");
		
		all_values[all_values_counter] = value;
		
		// next check avoids integer overflow
		all_values_counter++;

		ASSERT(all_values_counter <= 1000, err1, -5, "Buffer overflow.\n");
	}

	// FIND MOST COMMON
	int i = 0;
	int j = 0;
	unsigned long int values[1000];
	// values and all_values have compatible sizes
	memcpy(values, all_values, all_values_counter*sizeof(unsigned long int));
	unsigned int values_counter = all_values_counter;
	for (i = 11; i >= 0; i--) {
		one_counter = 0;
		zero_counter = 0;
		for (j = 0; j < values_counter; j++) {
			if ( (values[j] & (0x1 << i)) >> i ) {
				ones_values[one_counter] = values[j];
				// values_counter value avoids integer overflow
				one_counter++;
			} else {
				zeros_values[zero_counter] = values[j];
				// vaues_counter value avoids integer overflow
				zero_counter++;
			}
		}
		
		if (one_counter >= zero_counter) {
			// values and ones_values have compatibles sizes
			memcpy(values, ones_values, one_counter*sizeof(unsigned long int));
			values_counter = one_counter;
		} else {
			// values and zero_values have compatible sizes
			memcpy(values, zeros_values, zero_counter*sizeof(unsigned long int));
			values_counter = zero_counter;
		}

		// we found it
		if (values_counter == 1) {
			break;
		}
	}

	unsigned long int A = values[0];
	printf("Found A: %lu\n", A);

	// FIND LEAST COMMON
	// values and all values have compatibles sizes
	memcpy(values, all_values, all_values_counter*sizeof(unsigned long int));
	values_counter = all_values_counter;
        for (i = 11; i >= 0; i--) {
                one_counter = 0;
                zero_counter = 0;
                for (j = 0; j < values_counter; j++) {
                        if ( (values[j] & (0x1 << i)) >> i ) {
                                ones_values[one_counter] = values[j];
				// values_counter value avoids integer overflow
                                one_counter++;
                        } else {
                                zeros_values[zero_counter] = values[j];
				// values_counter value avoids integer overflow
                                zero_counter++;
                        }
                }

                if (one_counter >= zero_counter) {
			// values and zeros_values have compatible sizes
			memcpy(values, zeros_values, zero_counter*sizeof(unsigned long int));
                        values_counter = zero_counter;
                } else {
			// values and ones_values have compatibles sizes
			memcpy(values, ones_values, one_counter*sizeof(unsigned long int));
                     	values_counter = one_counter;
                }

                // we found it
                if (values_counter == 1) {
                        break;
                }
        }

	unsigned long int B = values[0];
	printf("Found B: %lu\n", B);

	unsigned long int result = A * B;

	printf("Result: %lu\n", result);

err1:
	if (line != NULL) {
		free(line);
	}

	fclose(input_file);

err0:
	return err;
}

