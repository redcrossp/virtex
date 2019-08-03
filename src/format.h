#ifndef _FORMAT_H_
#define _FORMAT_H_

#include <stdlib.h>
#include "virtex.h"

typedef struct format format;

struct format {
  unsigned int height;
  unsigned int width;
  unsigned int baseline;
  char** string;
};

char* strpad(char* str, size_t n);

format* fmt_create(unsigned int height, unsigned int width,
    unsigned int baseline);
void fmt_destroy(format* f);
void fmt_destroy_children(format** childrenFormats, unsigned int count);
void fmt_malloc_string(format* f);
void fmt_print(format* f);

format** fmt_children(virtex* v);
format* fmt_dispatch(virtex* v);

format* fmt_literal(virtex* v);
format* fmt_sum(virtex* v);
format* fmt_difference(virtex* v);
format* fmt_product(virtex* v);
format* fmt_fraction(virtex* v);
format* fmt_exponent(virtex* v);

#endif // _FORMAT_H_

