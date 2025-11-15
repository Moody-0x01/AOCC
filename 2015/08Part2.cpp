/*
--- Part Two ---
Now, let's go the other way. In addition to finding the number of characters of code, you should now encode each code representation as a new string and find the number of characters of the new encoded representation, including the surrounding double quotes.

For example:

"" encodes to "\"\"", an increase from 2 characters to 6.
"abc" encodes to "\"abc\"", an increase from 5 characters to 9.
"aaa\"aaa" encodes to "\"aaa\\\"aaa\"", an increase from 10 characters to 16.
"\x27" encodes to "\"\\x27\"", an increase from 6 characters to 11.
Your task is to find the total number of characters to represent the newly encoded strings minus the number of characters of code in each original string literal. For example, for the strings above, the total encoded length (6 + 9 + 16 + 11 = 42) minus the characters in the original code representation (23, just like in the first part of this puzzle) is 42 - 23 = 19.
*/
#include <cstddef>
#include <cstdint>
#include <ctype.h>

#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "An input file was not given.\n");
		return (1);
	}
	FILE *fp = fopen(argv[1], "r");

	if (!fp) {
		fprintf(stderr, "Could not open `%s` for reading.\n", argv[1]);
		return (1);
	}

	ssize_t nread = 1;
	size_t n = 0;
	char *Line = NULL;  
	size_t code_size = 0; 
	size_t encoded_size = 0; 
	while (1) {
		nread = getline(&Line, &n, fp);
		if (nread <= 0)
			break;
		if (nread-1 == 0) {
			free(Line);
			continue;
		}
		Line[nread-1] = 0;
		printf("%s\n", Line);
		code_size += nread - 1;

		for (int i = 0; i < nread - 1; i++) {
			switch (Line[i]) {
				case '\\':
				case '\'':
				case '"': {
					encoded_size += 2;
				} break;
				default: {
					encoded_size++;
				} break;
			}
		}

		encoded_size+=2; // adding ""
		free(Line);
		Line = NULL;
	}
	printf("answer: %zu\n", encoded_size - code_size);
	return (0);
}
