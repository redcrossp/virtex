#ifndef _FORMAT_H_
#define _FORMAT_H

#include "virtex.h"

char* strpad(char* str, size_t n);

typedef char** (*formatter)(vtx*);
char*** fmt_children(vtx* v);

char** fmt_alloc(vtx* v);
void fmt_free(vtx* v, char*** fmt_str);

char** fmt_lit(vtx* v);
char** fmt_sum(vtx* v);
char** fmt_diff(vtx* v);
char** fmt_prod(vtx* v);
char** fmt_frac(vtx* v);

void set_size_sum(vtx* v);

#endif // _FORMAT_H_

