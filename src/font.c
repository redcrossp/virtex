#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "virtex.h"
#include "font.h"
#include "../fonts/font0.c"

char* const LITERAL_STRINGS[] = {
  "ZERO0ZE",
  "ONE1ONE",
  "TWO2TWO",
  "THREE3T",
  "FOUR4FO",
  "FIVE5FI",
  "SIX6SIX",
  "SEVEN7S",
  "EIGHT8E",
  "NINE9NI"
};

static char* characters[3][10];

void font_init() {
  for (int size = 0; size < 3; size++) {
    char* needle = fontMonoPremier[size];
    for (int number = 0; number < 9; number++) {
      characters[size][number] = needle;
      while (*needle++) /* do nothing */;
    }
    // get needle for character 9
    characters[size][9] = needle;
  }
}

static unsigned int* get_char_widths(
    char* text, unsigned int length, unsigned int fontDepth) {
  unsigned int* charWidths = malloc(length * sizeof(unsigned int));
  for (unsigned int i = 0; i < length; i++) {
    unsigned int charNumber = text[i] - '0';
    unsigned int charStrLength = strlen(characters[fontDepth][charNumber]);
    charWidths[i] = charStrLength / (FONT_MAX_SIZE - fontDepth);
  }
  return charWidths;
}

static format* set_size_literal(
    unsigned int* charWidths, unsigned int count, unsigned int fontDepth) {
  unsigned int height = FONT_MAX_SIZE - fontDepth;
  unsigned int width = count - 1;
  unsigned int baseline = (height - 1) / 2;
  for (unsigned int i = 0; i < count; i++) {
    width += charWidths[i];
  }
  return fmt_create(height, width, baseline);
}

format* font_format(char* text, unsigned int fontDepth) {
  // printf("Printing text %s at size %d", text, c.sizedDepth);
  unsigned int length = strlen(text);
  unsigned int* charWidths = get_char_widths(text, length, fontDepth);
  format* f = set_size_literal(charWidths, length, fontDepth);
  char** fontOfSize = characters[fontDepth];

  for (unsigned int i = 0; i < f->height; i++) {
    // write format string row
    for (unsigned int j = 0; j < length; j++) {
      // write character cross section
      int number = text[j] - '0';
      char* charLineStart = fontOfSize[number] + i * charWidths[j];
      strncat(f->string[i], charLineStart, charWidths[j]);
      if (j != length - 1) strpad(f->string[i], 1);
    }
  }
  free(charWidths);
  return f;
}

