#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#define ASSERT(expression, label, err_code, error_msg) while(0){ if (!(expression)) { printf(error_msg); err = err_code; goto label; } }

extern int errno;

int main() {
	int err = 0;
	unsigned int counter = 0;
	size_t len = 0;
	ssize_t read = 0;
	char *line = NULL;
	char *end = NULL;
	FILE *input_file = NULL;

	long int map[] = { 
		0, // forward 
		0, // up
		0, // down
	};

	input_file = fopen("input.txt", "r");
	ASSERT(input_file != NULL, err0, -1, "Failed to open the input file.\n");

	while ((read = getline(&line, &len, input_file)) != -1) {
		long int space = 0;
		unsigned int type = 0;
		switch(read) {
			case 10:
				space = strtol(line+7, &end, 10);
				type = 0;
				break;
			case 5:
				space = strtol(line+2, &end, 10);
				type = 1;
				break;
			case 7:
				space = strtol(line+4, &end, 10);
				type = 2;
				break;
		}
		ASSERT(line != end && errno == 0, err1, -2, "Invalid input (non integer value).\n");
		ASSERT(space > 0, err1, -3, "Invalid input (negative value).\n");
		ASSERT(space > 0 && LONG_MAX-map[type] >= space, err1, -4, "Integer overflow!\n");
		map[type] += space;
	}

	long int horizontal = map[0];

	// XXX: subtracting two positive long integers is overflow safe	
	long int depth = map[2] - map[1];

	ASSERT(horizontal == 0 || depth <= LONG_MAX / horizontal, err1, -4, "Integer overflow!\n"); 
	long int result = horizontal * depth;

	// XXX: print %ld for long integer
	printf("Result: %ld\n", result);

err1:
	if (line != NULL) {
		free(line);
	}

	fclose(input_file);

err0:
	return err;
}

