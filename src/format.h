#ifndef _FORMAT_H_
#define _FORMAT_H_

#include <stdlib.h>
#include "virtex.h"

char* strpad(char* str, size_t n);

typedef char** (*formatter)(virtex*);
char*** fmt_children(virtex* v);
char** fmt_alloc(virtex* v);
void fmt_free(virtex* v, char** fmt_string);
void fmt_free_children(virtex* v, char*** childrenStrings);

char** fmt_literal(virtex* v);
char** fmt_sum(virtex* v);
char** fmt_difference(virtex* v);
char** fmt_product(virtex* v);
char** fmt_fraction(virtex* v);

#endif // _FORMAT_H_

