#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#define ASSERT(expression, label, err_code, error_msg) while(0){ if (!(expression)) { printf(error_msg); err = err_code; goto label; } }

extern int errno;


int main() {
	int err = 0;
	ssize_t read = 0;
	int last_value = -1;
	int current_value = 0;
	int counter = 0;
	char *line = NULL;
	size_t len = 0;
	char *end = NULL;
	FILE *input_file = NULL;

	input_file = fopen("input.txt", "r");
	ASSERT(input_file != NULL, err1, -1, "Failed to open the input file.\n");

	read = getline(&line, &len, input_file);
	ASSERT(read != -1, err2, -2, "The input is empty.\n");

	last_value = strtol(line, &end, 10);
	ASSERT(line != end && errno == 0, err3, -3, "The input is invalid.\n");

	while ((read = getline(&line, &len, input_file)) != -1) {
		current_value = strtol(line, &end, 10);
		ASSERT(line != end && errno == 0, err3, -3, "The input is invalid.\n");

		if (current_value > last_value) {
			ASSERT(INT_MAX-counter >= 1, err3, -4, "Int overflow\n");
			counter++;
		}
		last_value = current_value;
	}

	printf("Result: %d\n", counter);

err3:
	if (line != NULL) {
		free(line);
	}

err2:
	fclose(input_file);

err1:
	return err;
}

