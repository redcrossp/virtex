#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "virtex.h"

const unsigned int vtx_init_itemcap = 4;

vtx* vtx_create(vtx_type type) {
  vtx* v = malloc(sizeof(vtx));
  v->type = type;
  v->itemc = 0;
  v->itemcap = vtx_init_itemcap;
  v->itemv = malloc(sizeof(node) * vtx_init_itemcap);
  return v;
}

void vtx_destroy(vtx* v) {
  if (v->type == et_lit) {
    free(v->itemv[0].text);
  } else {
    for (unsigned int i = 0; i < v->itemc; i++)
      vtx_destroy(v->itemv[i].item);
  }
  free(v->itemv);
}

int vtx_insert(vtx* v, vtx* insert) {
  if (v->type == et_lit)
    return -1;
  if (v->itemc >= v->itemcap) {
    v->itemcap *= 2;
    v->itemv = realloc(v->itemv, sizeof(node) * v->itemcap);
  }
  v->itemv[v->itemc++].item = insert;
  return 0;
}

int vtx_text(vtx* v, char* str) {
  if (v->type != et_lit)
    return -1;
  char* str_copy = malloc(sizeof(char) * (strlen(str) + 1));
  strcpy(str_copy, str);
  v->itemv[0].text = str_copy;
  return 0;
}

void vtx_print(vtx* v) {
  char** str = vtx_format(v);
  for (unsigned int i = 0; i < v->height; i++)
    printf("%s\n", str[i]);
}




