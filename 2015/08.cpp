/*
--- Day 8: Matchsticks ---
Space on the sleigh is limited this year, and so Santa will be bringing his list as a digital copy. He needs to know how much space it will take up when stored.

It is common in many programming languages to provide a way to escape special characters in strings. For example, C, JavaScript, Perl, Python, and even PHP handle special characters in very similar ways.

However, it is important to realize the difference between the number of characters in the code representation of the string literal and the number of characters in the in-memory string itself.

For example:

"" is 2 characters of code (the two double quotes), but the string contains zero characters.
"abc" is 5 characters of code, but 3 characters in the string data.
"aaa\"aaa" is 10 characters of code, but the string itself contains six "a" characters and a single, escaped quote character, for a total of 7 characters in the string data.
"\x27" is 6 characters of code, but the string itself contains just one - an apostrophe ('), escaped using hexadecimal notation.
Santa's list is a file that contains many double-quoted string literals, one on each line. The only escape sequences used are \\ (which represents a single backslash), \" (which represents a lone double-quote character), and \x plus two hexadecimal characters (which represents a single character with that ASCII code).

Disregarding the whitespace in the file, what is the number of characters of code for string literals minus the number of characters in memory for the values of the strings in total for the entire file?

For example, given the four strings above, the total number of characters of string code (2 + 5 + 10 + 6 = 23) minus the total number of characters in memory for string values (0 + 3 + 7 + 1 = 11) is 23 - 11 = 12.
*/
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

#define MAX_TOKENS 12

bool isdigits(char *s) {
  if (!s)
    return false;
  if (!*s)
    return false;
  for (int i = 0; s[i];i++) {
    if (!isdigit(s[i]))
      return false;
  }
  return true;
}

void free_split_result(char **res)
{
  int idx = 0;
  if (res) {
    while (res[idx]) {
      free(res[idx]);
      idx++;
    }
    free(res);
  }
}

char **split(char *s, const char *delim)
{
  char *sc; // string copy
  char **vec;
  char *tok;
  int vec_idx;

  sc  = strdup(s);
  vec = (char **)malloc(sizeof(char *) * MAX_TOKENS);
  vec_idx = 0;
  tok = strtok(sc, delim);
  while (tok) {
    vec[vec_idx++] = strdup(tok);
    tok = strtok(NULL, delim);
  }
  vec[vec_idx] = NULL;
  free(sc);
  return vec;
}
#define MAX_INST 1024 * 60
bool isvalidhexdigit(char a) {
  return (((a >= 'a') && (a <= 'f')) || ((a >= '0') && (a <= '9')));
}
bool is_byte(char a, char b)
{
  return isvalidhexdigit(a) && isvalidhexdigit(b);
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
  size_t n = 0;
  size_t mem_storage = 0;
  size_t code_storage = 0;
  char *Line = NULL;  
  
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
    int i = 1;
    while (i < nread - 2) {
      if (Line[i] == '\\') {
        i++; /* Skip \ */
        switch (Line[i]) {
          case '\'':
          case '"': {
            i++;
            mem_storage++;
          } break;
          case 'x': {
            // escape x - first byte digit - second byte digit
            i+=3;
            mem_storage++;
          } break;
          default: {
            i++;
            mem_storage++;
          }
        }
      } else {
        i++;
        mem_storage++;
      }
    }
    code_storage += nread - 1;
    free(Line);
    Line = NULL;
  }
  printf("answer: %zu\n", code_storage - mem_storage);
  return (0);
}
