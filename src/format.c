#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "virtex.h"
#include "format.h"

// A [format] describes the ASCII representation of a virtex. A [format string]
// has format.height strings of exactly format.width length.

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

format* fmt_create(unsigned int height, unsigned int width,
    unsigned int baseline) {
  format* f = malloc(sizeof(format));
  f->height = height;
  f->width = width;
  f->baseline = baseline;
  fmt_malloc_string(f);
  return f;
}

void fmt_destroy(format* f) {
  for (unsigned int i = 0; i < f->height; i++) {
    free(f->string[i]);
  }
  free(f->string);
  free(f);
}

// Frees return value of fmt_children.
void fmt_destroy_children(format** childrenFormats, unsigned int count) {
  for (unsigned int i = 0; i < count; i++) {
    fmt_destroy(childrenFormats[i]);
  }
  free(childrenFormats);
}

// Allocates empty [format string] with appropriate size for virtex.
void fmt_malloc_string(format* f) {
  f->string = malloc(sizeof(char*) * f->height);
  for (unsigned int i = 0; i < f->height; i++) {
    f->string[i] = malloc(sizeof(char) * (f->width + 1));
    f->string[i][0] = '\0';
  }
}

// format virtex and print to screen
void fmt_print(format* f) {
  for (unsigned int i = 0; i < f->height; i++)
    printf("%s\n", f->string[i]);
}

// Returns [format] array of virtex children.
format** fmt_children(virtex* v) {
  format** childrenFormats = malloc(sizeof(format*) * v->childrenCount);
  for (unsigned int i = 0; i < v->childrenCount; i++) {
    childrenFormats[i] = fmt_dispatch(v->childrenNodes[i].item);
  }
  return childrenFormats;
}

format* fmt_dispatch(virtex* v) {
  format* f;
  switch (v->type) {
    case VT_LITERAL:
      f = fmt_literal(v);
      break;
    case VT_SUM:
      f = fmt_sum(v);
      break;
    case VT_DIFFERENCE:
      f = fmt_difference(v);
      break;
    case VT_PRODUCT:
      f = fmt_product(v);
      break;
    case VT_FRACTION:
      f = fmt_fraction(v);
      break;
    case VT_EXPONENT:
      f = fmt_exponent(v);
      break;
    case VT_BIGSUM:
    case VT_BIGPROD:
    case VT_SQRT:
    case VT_FUNCTION:
      break;
  }
  return f;
}



// =========================================================
//                  VTX-SPECIFIC FORMATTERS
// =========================================================

#define FMT_LITERAL_HEIGHT (1)
#define FMT_LITERAL_BASELINE (0)
format* fmt_literal(virtex* v) {
  unsigned int width = strlen(v->childrenNodes[0].text);
  format* f = fmt_create(
      FMT_LITERAL_HEIGHT,
      width,
      FMT_LITERAL_BASELINE
  );
  strcpy(f->string[0], v->childrenNodes[0].text);
  return f;
}

static format* set_size_list(format** childrenFormats, unsigned int count,
    const unsigned int delimLen) {
  unsigned int baseline = 0;
  unsigned int belowBaseline = 0;
  unsigned int width = (count - 1) * delimLen;
  for (unsigned int i = 0; i < count; i++) {
    unsigned int childHeight = childrenFormats[i]->height;
    unsigned int childBaseline = childrenFormats[i]->baseline;
    if (childBaseline > baseline)
      baseline = childBaseline;
    if (childHeight - childBaseline > belowBaseline)
      belowBaseline = childHeight - childBaseline;
    width += childrenFormats[i]->width;
  }
  return fmt_create(
      baseline + belowBaseline,
      width,
      baseline
  );
}

static format* fmt_list(virtex* v, const char* delimiter, const unsigned int delimLen) {
  format** childrenFormats = fmt_children(v);
  format* f = set_size_list(childrenFormats, v->childrenCount, delimLen);

  for (unsigned int x = 0; x < v->childrenCount; x++) {
    // delimeter
    if (x != 0) {
      for (unsigned int y = 0; y < f->height; y++) {
        if (y == f->baseline)
          strcat(f->string[y], delimiter);
        else
          strpad(f->string[y], delimLen);
      }
    }

    // children
    for (unsigned int y = 0, childY = childrenFormats[x]->baseline - f->baseline;
        y < f->height; y++, childY++) {
      if (0 <= childY && childY < childrenFormats[x]->height) {
        // line is in child bounds
        strcat(f->string[y], childrenFormats[x]->string[childY]);
      } else {
        strpad(f->string[y], childrenFormats[x]->width);
      }
    }
  }
  fmt_destroy_children(childrenFormats, v->childrenCount);
  return f;
}

format* fmt_sum(virtex* v) {
  const char* DELIMITER_SUM = " + ";
  const unsigned int DELIMITER_SUM_LENGTH = 3;
  return fmt_list(v, DELIMITER_SUM, DELIMITER_SUM_LENGTH);
}

format* fmt_difference(virtex* v) {
  const char* DELIMITER_DIFF = " - ";
  const unsigned int DELIMITER_DIFF_LENGTH = 3;
  return fmt_list(v, DELIMITER_DIFF, DELIMITER_DIFF_LENGTH);
}

format* fmt_product(virtex* v) {
  const char* DELIMITER_PROD = " * ";
  const unsigned int DELIMITER_PROD_LENGTH = 3;
  return fmt_list(v, DELIMITER_PROD, DELIMITER_PROD_LENGTH);
}

static format* set_size_fraction(format** childrenFormats) {
  unsigned int height = childrenFormats[0]->height + childrenFormats[1]->height + 1;
  unsigned int width = (childrenFormats[0]->width >= childrenFormats[1]->width
      ? childrenFormats[0]->width : childrenFormats[1]->width) + 2;
  unsigned int baseline = childrenFormats[0]->height;
  return fmt_create(height, width, baseline);
}

format* fmt_fraction(virtex* v) {
  format** childrenFormats = fmt_children(v);
  format* f = set_size_fraction(childrenFormats);

  unsigned int child0Start = (f->width - childrenFormats[0]->width) / 2;
  unsigned int child1Start = (f->width - childrenFormats[1]->width) / 2;

  for (unsigned int y = 0, child1Y = -childrenFormats[0]->height - 1;
      y < f->height; y++, child1Y++) {
    if (y < childrenFormats[0]->height) {
      strpad(f->string[y], child0Start);
      strcat(f->string[y], childrenFormats[0]->string[y]);
      strpad(f->string[y], f->width - childrenFormats[0]->width - child0Start);
    } else if (y == childrenFormats[0]->height) {
      for (unsigned int x = 0; x < f->width; x++)
        strcat(f->string[y], "-");
    } else {
      strpad(f->string[y], child1Start);
      strcat(f->string[y], childrenFormats[1]->string[child1Y]);
      strpad(f->string[y], f->width - childrenFormats[1]->width - child1Start);
    }
  }
  fmt_destroy_children(childrenFormats, v->childrenCount);
  return f;
}

static format* set_size_exponent(format** childrenFormats) {
  unsigned int height = childrenFormats[0]->height + childrenFormats[1]->baseline;
  unsigned int width = childrenFormats[0]->width + childrenFormats[1]->width + 1;
  unsigned int baseline = childrenFormats[0]->baseline + childrenFormats[1]->baseline;
  return fmt_create(height, width, baseline);
}

format* fmt_exponent(virtex* v) {
  format** childrenFormats = fmt_children(v);
  format* f = set_size_exponent(childrenFormats);

  for (unsigned int y = 0, child0Y = -childrenFormats[1]->baseline;
      y < f->height; y++, child0Y++) {
    if (y < childrenFormats[1]->baseline) {
      strpad(f->string[y], childrenFormats[0]->width + 1);
    } else {
      strcat(f->string[y], childrenFormats[0]->string[child0Y]);
      strpad(f->string[y], 1);
    }
    if (y < childrenFormats[1]->height) {
      strcat(f->string[y], childrenFormats[1]->string[y]);
    } else {
      strpad(f->string[y], childrenFormats[1]->width);
    }
  }
  fmt_destroy_children(childrenFormats, v->childrenCount);
  return f;
}

