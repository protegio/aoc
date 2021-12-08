#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#define ASSERT(expression, label, err_code, error_msg) while(0){ if (!(expression)) { printf(error_msg); err = err_code; goto label; } }

extern int errno;

int main() {
	int err = 0;
	unsigned int counters[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	size_t len = 0;
	ssize_t read = 0;
	char *line = NULL;
	char *end = NULL;
	FILE *input_file = NULL;

	input_file = fopen("input.txt", "r");
	ASSERT(input_file != NULL, err0, -1, "Failed to open the input file.\n");

	unsigned long int gama_rate = 0;
	unsigned long int epsilon_rate = 0;
	unsigned int counter = 0;
	unsigned int i = 0;
	long int value = 0;

	while ((read = getline(&line, &len, input_file)) != -1) {
		value = strtol(line, &end, 2);
		ASSERT(line != end && errno == 0, err1, -2, "Invalid input (non integer value).\n");

		for (i = 0; i < 12; i++) {
			ASSERT(UINT_MAX-counters[i] >= 1, err1, -4, "Integer overflow!\n");
			counters[i] += ((0x1 << i) & value) >> i;
		}

		ASSERT(UINT_MAX-counter >=1, err1, -4, "Integer overflow!\n");
		counter++;
	}

	for (i = 0; i < 12; i++) {
		if (counters[i] > counter/2) {
			gama_rate = gama_rate | (0x1 << i);
		}
	}

	epsilon_rate = (~gama_rate) & 0x0fff;

	ASSERT(gama_rate == 0 || epsilon_rate <= ULONG_MAX / gama_rate, err1, -4, "Integer overflow!\n"); 
	unsigned long int result = epsilon_rate * gama_rate;

	printf("Result: %lu\n", result);

err1:
	if (line != NULL) {
		free(line);
	}

	fclose(input_file);

err0:
	return err;
}

