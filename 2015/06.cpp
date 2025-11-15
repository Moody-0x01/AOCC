/*
--- Day 6: Probably a Fire Hazard ---
Because your neighbors keep defeating you in the holiday house decorating contest year after year, you've decided to deploy one million lights in a 1000x1000 grid.

Furthermore, because you've been especially nice this year, Santa has mailed you instructions on how to display the ideal lighting configuration.

Lights in your grid are numbered from 0 to 999 in each direction; the lights at each corner are at 0,0, 0,999, 999,999, and 999,0. The instructions include whether to turn on, turn off, or toggle various inclusive ranges given as coordinate pairs. Each coordinate pair represents opposite corners of a rectangle, inclusive; a coordinate pair like 0,0 through 2,2 therefore refers to 9 lights in a 3x3 square. The lights all start turned off.

To defeat your neighbors this year, all you have to do is set up your lights by doing the instructions Santa sent you in order.

For example:

turn on 0,0 through 999,999 would turn on (or leave on) every light.
toggle 0,0 through 999,0 would toggle the first line of 1000 lights, turning off the ones that were on, and turning on the ones that were off.
turn off 499,499 through 500,500 would turn off (or leave off) the middle four lights.
After following the instructions, how many lights are lit?
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
// #include <vector>
// #include <set>
#define WIDTH 1000
#define HEIGHT 1000
#define LIGHT_COUNT WIDTH * HEIGHT

typedef enum kind_e {
  TOGGLE = 0,
  TURN_ON,
  TURN_OFF
} Kind;

// const static char *action_strs[] = {
//   "turn",
//   "turn on",
//   "turn off",
// };
typedef struct vec2_s {
  int x, y;
} vec2;

typedef struct instruction_s {
  Kind kind;
  vec2 start;
  vec2 end;
} instruction_t;
#define MAX_TOKENS 12

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
   
  instruction_t ins;
  ssize_t nread = 1;
  size_t n = 0;
  char **tok;
  char **token_vec;
  char **chord_vec;
  char *Line = NULL;  
  // std::set<int> lit_lights_set;
  static bool lights[LIGHT_COUNT] = {0}; 
  while (1) {
    nread = getline(&Line, &n, fp);
    if (nread <= 0)
      break;
    if (nread-1 == 0) {
      free(Line);
      continue;
    }
    Line[nread-1] = 0;
    // TODO: Parse the line into an instruction thingy
    token_vec = split(Line, " ");
    tok = token_vec;
    // TODO: Parse the instruction kind
    if (strcmp(*tok, "turn") == 0) {
      tok++;
      if (strcmp(*tok, "on") == 0)
        ins.kind = TURN_ON;
      else if (strcmp(*tok, "off") == 0)
        ins.kind = TURN_OFF;
      else {
        fprintf(stderr, "Expected `on` or `off` but found %s\n", *tok);
        exit(1);
      }
    } else if (strcmp(*tok, "toggle") == 0) {
       ins.kind = TOGGLE;        
    } else {
      fprintf(stderr, "Expected `toggle` or `turn` but found %s\n", *tok);
      exit(1);
    }
    tok++;
    chord_vec = split(*tok, ",");
    ins.start.x = atoi(chord_vec[0]);
    ins.start.y = atoi(chord_vec[1]);
    tok++;
    assert(strcmp(*tok, "through") == 0 && "Expected a thro here!!");
    tok++;
    free_split_result(chord_vec);

    chord_vec = split(*tok, ",");
    ins.end.x = atoi(chord_vec[0]);
    ins.end.y = atoi(chord_vec[1]);
    // printf("The parsed action tell me to %s all bulbs from loc (%i, %i) thro (%i, %i)\n", action_strs[ins.kind], 
    //     ins.start.x, ins.start.y, ins.end.x, ins.end.y);
    // TODO: Cleanup ur messss..
    switch (ins.kind) {
    case TURN_OFF: {
      // Getting a light using chords
      // lights[(LIGHT_COUNT * y) + x];
      // Range: ((LIGHT_COUNT * sy) + sx) -> ((LIGHT_COUNT * ey) + ex)
      // int start = ((HEIGHT * ins.start.y) + ins.start.x);
      // int end   = ((HEIGHT * ins.end.y) + ins.end.x);
      // for (int cell = start; cell <= end; ++cell)
      //   lights[cell] = 0;
      for (int x = ins.start.x; x <= ins.end.x; ++x) {
        for (int y = ins.start.y; y <= ins.end.y; ++y) {
           int cell = (x * WIDTH) + y;
           lights[cell] = 0;
        }
      }
    } break;
    case TURN_ON: {
      for (int x = ins.start.x; x <= ins.end.x; ++x) {
        for (int y = ins.start.y; y <= ins.end.y; ++y) {
           int cell = (x * WIDTH) + y;
           lights[cell] = 1;
        }
      }      
    } break;
    case TOGGLE: {
      for (int x = ins.start.x; x <= ins.end.x; ++x) {
        for (int y = ins.start.y; y <= ins.end.y; ++y) {
           int cell = (x * WIDTH) + y;
           lights[cell] = !lights[cell];
        }
      }
    } break;
    default: {} break;
    }
    free_split_result(chord_vec);
    free_split_result(token_vec);
    free(Line);
    Line = NULL;
  }

  size_t turned_on = 0, turned_off = 0; 
  for (int y = 0; y < HEIGHT; ++y) {
    for (int cell = 0; cell < WIDTH; cell++) {
      int idx = (HEIGHT * y) + cell;
      turned_on += lights[idx];
      turned_off += !lights[idx];
    }
  }
  printf("Turned off: %zu Turned on: %zu\n", turned_off, turned_on);
  return (0);
}
