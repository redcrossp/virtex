#ifndef _FORMAT_H_
#define _FORMAT_H

#include "virtex.h"

char* strpad(char* str, size_t n);

typedef char** (*formatter)(vtx*);
char*** fmt_children(vtx* v);
char** fmt_alloc(vtx* v);
void fmt_free(vtx* v, char** fmt_string);
void fmt_free_children(vtx* v, char*** childrenStrings);

char** fmt_literal(vtx* v);
char** fmt_sum(vtx* v);
char** fmt_difference(vtx* v);
char** fmt_product(vtx* v);
char** fmt_fraction(vtx* v);

#endif // _FORMAT_H_

