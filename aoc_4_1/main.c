#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

#define ASSERT(expression, label, err_code, error_msg) do { if (!(expression)) { printf(error_msg); err = err_code; goto label; } } while(0)

extern int errno;

int split2int(char* str, const char* delim, unsigned long int* values, unsigned int* values_counter);

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
        unsigned long int draw_numbers[1000];
        unsigned int draw_numbers_counter = sizeof(draw_numbers);

	// read draw numbers
	read = getline(&line, &len, input_file);
	ASSERT(read != -1, err1, -2, "Invalid input (empty).\n");
	err = split2int(line, ",", draw_numbers, &draw_numbers_counter);
	ASSERT(err == 0, err1, err, "Invalid input (draw numbers format).\n");

	// read boards
	unsigned int i = 0;
	unsigned long int boards[100][5][5];
	unsigned int boards_counter = 0;
	while ((read = getline(&line, &len, input_file)) != -1) {
		// skip line 0, 6, 12, etc...
		if ((i % 6) == 0) {
			boards_counter++;
			i++;
			continue;
		}

		unsigned int line_size = 5;
		err = split2int(line, " ", (boards[boards_counter-1][i%6-1]), &line_size);
	        ASSERT(err == 0 && line_size == 5, err1, err, "Invalid input (board format).\n");
		i++;
	}

	// find first winning game (dummy)
	unsigned int j = 0;
	int win = 0;
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int winning_board = 0;
	long unsigned int last_draw_number = 0;
	for (i = 0; i < draw_numbers_counter; i++) {
		for (j = 0; j < boards_counter; j++) {
			unsigned int line_counter[] = { 0, 0, 0, 0, 0 };
			unsigned int colum_counter[] = { 0, 0, 0, 0, 0 };

			// summ all lines and colums
			for (x = 0; x < 5; x++) {
				for (y = 0; y < 5; y++) {
					line_counter[x] += boards[j][x][y];
                                        colum_counter[y] += boards[j][x][y];
				}
			}

			// mark number and check win
			for (x = 0; x < 5; x++) {
				for (y = 0; y < 5; y++) {
					if (boards[j][x][y] == draw_numbers[i]) {
						boards[j][x][y] = 0;
						line_counter[x] -= draw_numbers[i];
                                        	colum_counter[y] -= draw_numbers[i];

						// check if this is the winning number
						if (line_counter[x] == 0 || colum_counter[y] == 0) {
                                       			win = 1;
                                        		winning_board = j;
                                       	 		last_draw_number = draw_numbers[i];
                                        		break;
                                		}
					}
				}
				if (win == 1) break;
			}
			if (win == 1) break;
		}
		if (win == 1) break;
	}

	if (win == 1) {
		printf("BINGO!\n");
		unsigned long int score = 0;
		for ( x = 0; x < 5; x++ ) {
			for ( y = 0; y < 5; y++ ) {
				printf("%02lu ", boards[winning_board][x][y]);
				score += boards[winning_board][x][y];
			}
			printf("\n");
		}
		unsigned long int result = score * last_draw_number;
		printf("Winning board: %u\n", winning_board);
		printf("Score: %lu\n", score);
		printf("Last draw number (%u): %lu\n", i, last_draw_number);
		printf("Result: %lu\n", result);
	}

err1:
	if (line != NULL) {
		free(line);
	}

	fclose(input_file);

err0:
	return err;
}

