#include <stdlib.h>

#include "virtex.h"
#include "format.h"
#include "parser.h"

int main() {
  vtx* lit1 = vtx_create(VT_LITERAL);
  vtx_text(lit1, "3");
  vtx* lit2 = vtx_create(VT_LITERAL);
  vtx_text(lit2, "72");
  vtx* lit3 = vtx_create(VT_LITERAL);
  vtx_text(lit3, "-9");
  vtx* lit4 = vtx_create(VT_LITERAL);
  vtx_text(lit4, "10");
  vtx* lit5 = vtx_create(VT_LITERAL);
  vtx_text(lit5, "5");
  vtx* lit6 = vtx_create(VT_LITERAL);
  vtx_text(lit6, "17");
  vtx* lit7 = vtx_create(VT_LITERAL);
  vtx_text(lit7, "6");

  vtx* sum1 = vtx_create(VT_SUM);
  vtx_insert(sum1, lit1);
  vtx_insert(sum1, lit2);

  vtx* frac1 = vtx_create(VT_FRACTION);
  vtx_insert(frac1, sum1);
  vtx_insert(frac1, lit3);

  vtx* diff1 = vtx_create(VT_DIFFERENCE);
  vtx_insert(diff1, frac1);
  vtx_insert(diff1, lit4);

  vtx* prod1 = vtx_create(VT_PRODUCT);
  vtx_insert(prod1, lit5);
  vtx_insert(prod1, lit6);

  vtx* frac2 = vtx_create(VT_FRACTION);
  vtx_insert(frac2, diff1);
  vtx_insert(frac2, prod1);

  vtx* diff2 = vtx_create(VT_PRODUCT);
  vtx_insert(diff2, frac2);
  vtx_insert(diff2, lit7);

//  vtx_print(diff2);

  // only have to destroy outside virtex
  vtx_destroy(diff2);

  parse_file("src/parse_test");

  return 0;
}

