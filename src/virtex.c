#include <stdlib.h>
#include "virtex.h"

vtx* vtx_create(void) {
  vtx* v = malloc(sizeof(vtx));
  return v;
}

void vtx_destroy(vtx* v) {
  if (v->type == et_lit) {
    free(v->itemv[0].text);
  } else {
    for (unsigned int i = 0; i < v->itemc; i++)
      vtx_destroy(v->itemv[i].item);
    free(v->itemv);
  }
  free(v->itemv);
}
