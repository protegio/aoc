#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#define ASSERT(expression, label, err_code, error_msg) while(0){ if (!(expression)) { printf(error_msg); err = err_code; goto label; } }

extern int errno;

long int sum_three(long int a, long int b, long int c);

inline long int sum_three(long int a, long int b, long int c) {
	int err;

	ASSERT(a >= 0 && b <= LONG_MAX - a || a < 0 && b >= LONG_MIN - a, err0, -1, "Integer overflow!\n");
	long int d = a + b;

	ASSERT(d >= 0 && c <= LONG_MAX - d || d < 0 && c >= LONG_MIN - d, err0, -1, "Integer overflow!\n" );
	return d + c;

err0:
	errno = ERANGE;
	return 0;
}


int main() {
	int err = 0;
	unsigned int counter = 0;
	size_t len = 0;
	ssize_t read = 0;
	char *line = NULL;
	char *end = NULL;
	FILE *input_file = NULL;

	input_file = fopen("input.txt", "r");
	ASSERT(input_file != NULL, err1, -1, "Failed to open the input file.\n");

	read = getline(&line, &len, input_file);
	ASSERT(read != -1, err2, -2, "Invalid input.\n");
	long int a = strtol(line, &end, 10);
	ASSERT(line != end && errno == 0, err3, -3, "Invalid input.\n");

	read = getline(&line, &len, input_file);
        ASSERT(read != -1, err3, -2, "Invalid input.\n");
        long int b = strtol(line, &end, 10);
        ASSERT(line != end && errno == 0, err3, -3, "Invalid input.\n");

	read = getline(&line, &len, input_file);
        ASSERT(read != -1, err3, -2, "Invalid input.\n");
        long int c = strtol(line, &end, 10);
        ASSERT(line != end && errno == 0, err3, -3, "Invalid input.\n");

	while ((read = getline(&line, &len, input_file)) != -1) {
		long int d = strtol(line, &end, 10);
		ASSERT(line != end && errno == 0, err3, -3, "Invalid input.\n");

		long int x = sum_three(b, c, d);
		ASSERT(errno == 0, err3, -4, "Integer overflow!\n");

		long int y = sum_three(a, b, c);
		ASSERT(errno == 0, err3, -4, "Integer overflow!\n");

		if (x > y) {
			ASSERT(UINT_MAX-counter >= 1, err3, -4, "Integer Overflow!\n");
			counter++;
		}

		a=b;
		b=c;
		c=d;
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

