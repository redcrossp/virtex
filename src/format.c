#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "virtex.h"
#include "format.h"

// A [format string] is an array of strings giving an ASCII representation of
// a [virtex]. A [format string] has virtex.height strings of exactly virtex.width
// length.

// Pads str on end with spaces (ASCII 32).
// str must have enough space for the result.
char* strpad(char* str, size_t n) {
  size_t len = strlen(str);
  size_t end = len + n;
  for (size_t i = len; i < end; i++)
    str[i] = ' ';
  str[end] = '\0';
  return str;
}

// format virtex and print to screen
void vtx_print(virtex* v) {
  char** fmt_string = vtx_format(v);
  for (unsigned int i = 0; i < v->height; i++)
    printf("%s\n", fmt_string[i]);
  fmt_free(v, fmt_string);
}

char** vtx_format(virtex* v) {
  char** fmt_string;
  switch (v->type) {
    case VT_LITERAL:
      fmt_string = fmt_literal(v);
      break;
    case VT_SUM:
      fmt_string = fmt_sum(v);
      break;
    case VT_DIFFERENCE:
      fmt_string = fmt_difference(v);
      break;
    case VT_PRODUCT:
      fmt_string = fmt_product(v);
      break;
    case VT_FRACTION:
      fmt_string = fmt_fraction(v);
      break;
    case VT_EXPONENT:
      fmt_string = fmt_exponent(v);
      break;
    case VT_BIGSUM:
    case VT_BIGPROD:
    case VT_SQRT:
    case VT_FUNCTION:
      break;
  }
  return fmt_string;
}

// Returns [format string] array of virtex children.
char*** fmt_children(virtex* v) {
  char*** childrenStrings = malloc(sizeof(char**) * v->childCount);
  for (unsigned int i = 0; i < v->childCount; i++)
    childrenStrings[i] = vtx_format(v->childNodes[i].item);
  return childrenStrings;
}

// Allocates empty format string with appropriate size for virtex.
// Must be called after format info for node is determined.
char** fmt_alloc(virtex* v) {
  char** out = malloc(sizeof(char*) * v->height);
  for (unsigned int i = 0; i < v->height; i++) {
    out[i] = malloc(sizeof(char) * (v->width + 1));
    out[i][0] = '\0';
  }
  return out;
}

void fmt_free(virtex* v, char** fmt_string) {
  for (unsigned int i = 0; i < v->height; i++) {
    free(fmt_string[i]);
  }
  free(fmt_string);
}

// Frees return value of fmt_children.
void fmt_free_children(virtex* v, char*** childrenStrings) {
  for (unsigned int i = 0; i < v->childCount; i++) {
    fmt_free(v->childNodes[i].item, childrenStrings[i]);
  }
  free(childrenStrings);
}



// =========================================================
//                  VTX-SPECIFIC FORMATTERS
// =========================================================

char** fmt_literal(virtex* v) {
  v->height = 1;
  v->baseline = 0;
  v->width = strlen(v->childNodes[0].text);
  char** formatStr = fmt_alloc(v);
  strcpy(formatStr[0], v->childNodes[0].text);
  return formatStr;
}

void set_size_list(virtex* v, const unsigned int delimLen) {
  unsigned int aboveln = 0;
  unsigned int belowln = 0;
  unsigned int width = 0;
  for (unsigned int i = 0; i < v->childCount; i++) {
    unsigned int itemHeight = v->childNodes[i].item->height;
    unsigned int itemBaseln = v->childNodes[i].item->baseline;
    if (itemBaseln > aboveln)
      aboveln = itemBaseln;
    if (itemHeight - itemBaseln > belowln)
      belowln = itemHeight - itemBaseln;
    width += v->childNodes[i].item->width + delimLen;
  }
  v->baseline = aboveln;
  v->height = aboveln + belowln;
  v->width = width - delimLen;
}

char** fmt_list(virtex* v, const char* delimiter, const unsigned int delimLen) {
  // each output row has text
  char*** childrenStrings = fmt_children(v);
  set_size_list(v, delimLen);

  char** formatStr = fmt_alloc(v);
  for (unsigned int x = 0; x < v->childCount; x++) {
    // addition symbols
    if (x != 0) {
      for (unsigned int y = 0; y < v->height; y++) {
        if (y == v->baseline)
          strcat(formatStr[y], delimiter);
        else
          strpad(formatStr[y], delimLen);
      }
    }

    // items
    virtex* item = v->childNodes[x].item;
    char** itemStr = childrenStrings[x];
    for (unsigned int y = 0, itemY = item->baseline - v->baseline;
        y < v->height; y++, itemY++) {
      if (0 <= itemY && itemY < item->height) {
        // line is in subitem bounds
        strcat(formatStr[y], itemStr[itemY]);
      } else {
        strpad(formatStr[y], item->width);
      }
    }
  }
  fmt_free_children(v, childrenStrings);
  return formatStr;
}

char** fmt_sum(virtex* v) {
  const char* DELIMITER_SUM = " + ";
  const unsigned int DELIMITER_SUM_LENGTH = 3;
  return fmt_list(v, DELIMITER_SUM, DELIMITER_SUM_LENGTH);
}

char** fmt_difference(virtex* v) {
  const char* DELIMITER_DIFF = " - ";
  const unsigned int DELIMITER_DIFF_LENGTH = 3;
  return fmt_list(v, DELIMITER_DIFF, DELIMITER_DIFF_LENGTH);
}

char** fmt_product(virtex* v) {
  const char* DELIMITER_PROD = " * ";
  const unsigned int DELIMITER_PROD_LENGTH = 3;
  return fmt_list(v, DELIMITER_PROD, DELIMITER_PROD_LENGTH);
}

char** fmt_fraction(virtex* v) {
  char*** childrenStrings = fmt_children(v);
  
  virtex* item0 = v->childNodes[0].item;
  virtex* item1 = v->childNodes[1].item;

  v->baseline = item0->height;
  v->height = item0->height + item1->height + 1;
  v->width = (item0->width >= item1->width ? item0->width : item1->width) + 2;

  unsigned int item0Start = (v->width - item0->width) / 2;
  unsigned int item1Start = (v->width - item1->width) / 2;

  char** formatStr = fmt_alloc(v);
  for (unsigned int y = 0; y < v->height; y++) {
    if (y < item0->height) {
      strpad(formatStr[y], item0Start);
      strcat(formatStr[y], childrenStrings[0][y]);
      strpad(formatStr[y], v->width - item0->width - item0Start);
    } else if (y == item0->height) {
      for (unsigned int x = 0; x < v->width; x++)
        strcat(formatStr[y], "-");
    } else {
      strpad(formatStr[y], item1Start);
      strcat(formatStr[y], childrenStrings[1][y - item0->height - 1]);
      strpad(formatStr[y], v->width - item1->width - item1Start);
    }
  }
  fmt_free_children(v, childrenStrings);
  return formatStr;
}

char** fmt_exponent(virtex* v) {
  char*** childrenStrings = fmt_children(v);

  virtex* item0 = v->childNodes[0].item;
  virtex* item1 = v->childNodes[1].item;

  v->height = item0->height + item1->baseline;
  v->baseline = item0->baseline + item1->baseline;
  v->width = item0->width + item1->width + 1;

  char** formatStr = fmt_alloc(v);
  for (unsigned int y = 0; y < v->height; y++) {
    if (y < item1->baseline) {
      strpad(formatStr[y], item0->width + 1);
    } else {
      strcat(formatStr[y], childrenStrings[0][y - item1->baseline]);
      strpad(formatStr[y], 1);
    }
    if (y < item1->height) {
      strcat(formatStr[y], childrenStrings[1][y]);
    } else {
      strpad(formatStr[y], item1->width);
    }
  }
  fmt_free_children(v, childrenStrings);
  return formatStr;
}

