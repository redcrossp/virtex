#ifndef _FONT_H_
#define _FONT_H_

#include "format.h"

void font_init();
format* font_format(char* text, unsigned int fontDepth);

#define FONT_MAX_SIZE (5)
#define FONT_MAX_DEPTH (2)

/*
char** fmt_literal(virtex* v, font f);
font font_create(unsigned int size);
font font_increase(font f);
font font_decrease(font f);
*/

#endif // _FONT_H_

