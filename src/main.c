#include <stdlib.h>

#include "virtex.h"
#include "format.h"

int main() {
  vtx* lit1 = vtx_create(et_lit);
  vtx_text(lit1, "1");
  vtx* lit2 = vtx_create(et_lit);
  vtx_text(lit2, "2");
  vtx* lit3 = vtx_create(et_lit);
  vtx_text(lit3, "3");
  vtx* lit4 = vtx_create(et_lit);
  vtx_text(lit4, "4");
  vtx* lit5 = vtx_create(et_lit);
  vtx_text(lit5, "5");
  vtx* lit6 = vtx_create(et_lit);
  vtx_text(lit6, "6");

  vtx* frac0 = vtx_create(et_frac);
  vtx_insert(frac0, lit3);
  vtx_insert(frac0, lit6);

  vtx* sum = vtx_create(et_diff);
  vtx_insert(sum, lit1);
  vtx_insert(sum, frac0);
  vtx_insert(sum, lit4);

  vtx* frac = vtx_create(et_frac);
  vtx_insert(frac, lit2);
  vtx_insert(frac, sum);

  vtx_print(frac);

  vtx_destroy(sum);

  return 0;
}

