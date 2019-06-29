#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "virtex.h"

vtx* vtx_create(vtxType type) {
  vtx* v = malloc(sizeof(vtx));
  v->type = type;
  v->childCount = 0;
  v->childCapacity = VTX_INIT_CHILDCAPACITY;
  v->childNodes = malloc(sizeof(node) * VTX_INIT_CHILDCAPACITY);
  return v;
}

// destroys vertex AND children
void vtx_destroy(vtx* v) {
  if (v->type == VT_LITERAL) {
    free(v->childNodes[0].text);
  } else {
    for (unsigned int i = 0; i < v->childCount; i++)
      vtx_destroy(v->childNodes[i].item);
  }
  free(v->childNodes);
  free(v);
}

// add child vertex, doubling size of children array if necessary
// fails with return value -1 if literal vertex
int vtx_insert(vtx* v, vtx* insert) {
  if (v->type == VT_LITERAL)
    return -1;
  if (v->childCount >= v->childCapacity) {
    v->childCapacity *= 2;
    v->childNodes = realloc(v->childNodes, sizeof(node) * v->childCapacity);
  }
  v->childNodes[v->childCount++].item = insert;
  return 0;
}

// set text of vertex
// fails with return value -1 if non-terminal vertex
int vtx_text(vtx* v, char* str) {
  if (v->type != VT_LITERAL)
    return -1;
  char* str_copy = malloc(sizeof(char) * (strlen(str) + 1));
  strcpy(str_copy, str);
  v->childNodes[0].text = str_copy;
  return 0;
}




