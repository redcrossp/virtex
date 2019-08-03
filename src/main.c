#include <stdlib.h>

#include "virtex.h"
#include "format.h"
#include "parser.h"

int main() {
  virtex* lit1 = vtx_create(VT_LITERAL);
  vtx_text(lit1, "3");
  virtex* lit2 = vtx_create(VT_LITERAL);
  vtx_text(lit2, "72");
  virtex* lit3 = vtx_create(VT_LITERAL);
  vtx_text(lit3, "-9");
  virtex* lit4 = vtx_create(VT_LITERAL);
  vtx_text(lit4, "10");
  virtex* lit5 = vtx_create(VT_LITERAL);
  vtx_text(lit5, "5");
  virtex* lit6 = vtx_create(VT_LITERAL);
  vtx_text(lit6, "17");
  virtex* lit7 = vtx_create(VT_LITERAL);
  vtx_text(lit7, "6");
  virtex* litPi = vtx_create(VT_LITERAL);
  vtx_text(litPi, "314");

  virtex* sum1 = vtx_create(VT_SUM);
  vtx_insert(sum1, lit1);
  vtx_insert(sum1, lit2);

  virtex* frac1 = vtx_create(VT_FRACTION);
  vtx_insert(frac1, sum1);
  vtx_insert(frac1, lit3);

  virtex* exp1 = vtx_create(VT_EXPONENT);
  vtx_insert(exp1, lit4);
  vtx_insert(exp1, litPi);

  virtex* diff1 = vtx_create(VT_DIFFERENCE);
  vtx_insert(diff1, frac1);
  vtx_insert(diff1, exp1);

  virtex* prod1 = vtx_create(VT_PRODUCT);
  vtx_insert(prod1, lit5);
  vtx_insert(prod1, lit6);

  virtex* frac2 = vtx_create(VT_FRACTION);
  vtx_insert(frac2, diff1);
  vtx_insert(frac2, prod1);

  virtex* prod2 = vtx_create(VT_PRODUCT);
  vtx_insert(prod2, frac2);
  vtx_insert(prod2, lit7);

  // vtx_print(prod2);

  // only have to destory outside virtex
  vtx_destroy(prod2);

  parse_file("src/parse_test");

  return 0;
}

