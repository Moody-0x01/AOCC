/*
--- Day 5: Doesn't He Have Intern-Elves For This? ---
Santa needs help figuring out which strings in his text file are naughty or nice.

A nice string is one with all of the following properties:

It contains at least three vowels (aeiou only), like aei, xazegov, or aeiouaeiouaeiou.
It contains at least one letter that appears twice in a row, like xx, abcdde (dd), or aabbccdd (aa, bb, cc, or dd).
It does not contain the strings ab, cd, pq, or xy, even if they are part of one of the other requirements.
For example:

ugknbfddgicrmopn is nice because it has at least three vowels (u...i...o...), a double letter (...dd...), and none of the disallowed substrings.
aaa is nice because it has at least three vowels and a double letter, even though the letters used by different rules overlap.
jchzalrnumimnmhp is naughty because it has no double letter.
haegwjzuvuyypxyu is naughty because it contains the string xy.
dvszwmarrgswjxmb is naughty because it contains only one vowel.
How many strings are nice?
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
// NOTE: Nice words contains vowels
#define BAD_SUB_COUNT 4
#define VOLWELS "aeiou\0"

static char bad_subs[BAD_SUB_COUNT][3] = {
	"ab", "cd", "pq", "xy"
};
bool is_vowel(char c)
{
	for (char *v = VOLWELS; *v; ++v) {
		if (*v == c)
			return true;
	}
	return false;
}
static unsigned int vowel_count = 0, repeating_chars = 0, len = 0;
static bool has_bad_word = false;
bool is_nice(char *word)
{
	// TODO: Check if the word has atleast 3 vowels. 
	vowel_count = 0, repeating_chars = 0, len = 0;
	has_bad_word = false;
	// TODO: Check if the word does not have ab, cd, pq, or xy 

	for (int i = 0; i < BAD_SUB_COUNT; ++i) {
		if (strstr(word, bad_subs[i])) {
			has_bad_word = true;
			return false;
		}
	}
	for (; word[len]; ++len) vowel_count += (unsigned int)(is_vowel(word[len]));
	if (vowel_count < 3)
		return false;
	// TODO: Check if the word has atleast one char that repeats once atleast in a row,
	for (size_t i = 0; i < (len - 1); ++i) if (word[i] == word[i+1]) repeating_chars++;
	return repeating_chars;
}

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
	unsigned long nice = 0, bad = 0;
	size_t n = 0;
	char *Line = NULL;

	while (1) {
		nread = getline(&Line, &n, fp);
		if (nread <= 0)
			break;
		Line[nread-1] = 0;
		if (is_nice(Line)) {
			nice++; 
		} else {
			bad++;
			// printf("\n");
			// printf("@Word Stats: %s\n", Line);
			// if (has_bad_word)
			//   printf("@Word %s is bad lol\n", Line);
			// if (vowel_count < 3)
			//   printf("@Word requirements: 3 vowels but found %u\n", vowel_count);
			// if (!repeating_chars)
			//   printf("No repeating_chars in %s\n", Line);
			// printf("\n");
		}

		free(Line);
		Line = NULL;
	}
	printf("Nice words: %ld Bad words: %ld\n", nice, bad);
	return (0);
}
