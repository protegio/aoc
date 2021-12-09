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
	unsigned int segments_size = 0;
	unsigned long int segments[1000][2][2];
	while ((read = getline(&line, &len, input_file)) != -1) {
		unsigned int tokens_size = 2;
		char tokens[2][20];
		err = split(line, " -> ", (char**) &tokens, &tokens_size, 20);
	        ASSERT(err == 0 && tokens_size == 2, err1, err, "Invalid input.\n");

		unsigned int pointASize = 2;
		err = split2int(tokens[0], ",", segments[segments_size][0], &pointASize);
		ASSERT(err == 0 && pointASize == 2, err1, err, "Invalid input.\n");
		//printf("%lu,%lu -> ", segments[segments_size][0][0], segments[segments_size][0][1]);

                unsigned int pointBSize = 2;
                err = split2int(tokens[1], ",", segments[segments_size][1], &pointBSize);
                ASSERT(err == 0 && pointBSize == 2, err1, err, "Invalid input.\n");
		//printf("%lu,%lu\n", segments[segments_size][1][0], segments[segments_size][1][1]);

		segments_size++;
	}

	unsigned int i = 0;
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int map[1000][1000];
	memset(map, 0, 1000*1000*sizeof(unsigned int));
	for (i = 0; i < segments_size; i++) {
		// skip diagonal segments
		
		unsigned long int x1 = segments[i][0][0];
		unsigned long int y1 = segments[i][0][1];
		unsigned long int x2 = segments[i][1][0];
		unsigned long int y2 = segments[i][1][1];

		if (x1 != x2 && y1 != y2 && ABS_DIFF(x1, x2) != ABS_DIFF(y1, y2)) {
			continue;
		}
	
		// XXX: I could use a min/max function to avoid
		// so many branches, but I'm ok with that.	
		if (y1 == y2 && x1 > x2) {
			for (x = x2; x <= x1; x++) {
				map[x][y1]++;	
			}
		} else if (y1 == y2 && x1 < x2) {
			for (x = x1; x <= x2; x++) {
                                map[x][y1]++;
                        }
		} else if (x1 == x2 && y1 > y2) {
			for (x = y2; x <= y1; x++) {
                                map[x1][x]++;
                        }
		} else if (x1 == x2 && y1 < y2) {
			for (x = y1; x <= y2; x++) {
                                map[x1][x]++;
                        }
		} else if (x1 > x2 && y1 > y2) {
			y = y2;
			for (x = x2; x <= x1; x++) {
				map[x][y]++;
				y++;
			}
		} else if (x1 > x2 && y1 < y2) {
			y = y2;
			for (x = x2; x <= x1; x++) {
                        	map[x][y]++;
				y--;
                        }
		} else if (x1 < x2 && y1 > y2) {
			y = y1;
			for (x = x1; x <= x2; x++) {
                        	map[x][y]++;
				y--;
                        }
		} else if (x1 < x2 && y1 < y2) {
			y = y1;
			for (x = x1; x <= x2; x++) {
                        	map[x][y]++;
				y++;
                        }
		}
	}

	int j = 0;
	unsigned int result = 0;
	for (i = 0; i < 1000; i++) {
		for (j = 0; j < 1000; j++) {
			if (map[i][j] >= 2) {
				result++;
			}
		}
	}

	printf("Result: %u\n", result);

err1:
	if (line != NULL) {
		free(line);
	}

	fclose(input_file);

err0:
	return err;
}

