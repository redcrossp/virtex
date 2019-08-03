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

format* fmt_create(unsigned int height, unsigned int width,
    unsigned int baseline);
void fmt_destroy(format* f);
void fmt_destroy_children(format** childrenFormats, unsigned int count);
void fmt_malloc_string(format* f);
void fmt_print(format* f);

typedef struct context context;
struct context {
  unsigned int depth;
  unsigned int sizedDepth;
  int display;
};

#define CTX_DISPLAY (1)
#define CTX_INLINE (0)
const context ctx_root(int display);
context ctx_child(context c);
context ctx_smaller(context c);
unsigned int ctx_font_depth(context c);

char* strpad(char* str, size_t n);

format** fmt_children(virtex* v, context c);
format* fmt_dispatch(virtex* v, context c);

format* fmt_literal(virtex* v, context c);
format* fmt_sum(virtex* v, context c);
format* fmt_difference(virtex* v, context c);
format* fmt_product(virtex* v, context c);
format* fmt_fraction(virtex* v, context c);
format* fmt_exponent(virtex* v, context c);

#endif // _FORMAT_H_

