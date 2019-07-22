#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "virtex.h"
#include "format.h"

virtex* vtx_create(vtxType type) {
  virtex* v = malloc(sizeof(virtex));
  v->type = type;
  v->childrenCount = 0;
  v->childrenCapacity = VTX_INIT_CHILDCAPACITY;
  v->childrenNodes = malloc(sizeof(node) * VTX_INIT_CHILDCAPACITY);
  if (type == VT_LITERAL) {
    v->childrenNodes[0].text = malloc(sizeof(char));
  }
  return v;
}

// destroys virtex AND children
void vtx_destroy(virtex* v) {
  if (v->type == VT_LITERAL) {
    free(v->childrenNodes[0].text);
  } else {
    for (unsigned int i = 0; i < v->childrenCount; i++)
      vtx_destroy(v->childrenNodes[i].item);
  }
  free(v->childrenNodes);
  free(v);
}

// add children virtex, doubling size of children array if necessary
// fails with return value -1 if literal virtex
int vtx_insert(virtex* v, virtex* insert) {
  if (v->type == VT_LITERAL)
    return -1;
  if (v->childrenCount >= v->childrenCapacity) {
    v->childrenCapacity *= 2;
    v->childrenNodes = realloc(v->childrenNodes, sizeof(node) * v->childrenCapacity);
  }
  v->childrenNodes[v->childrenCount++].item = insert;
  return 0;
}

// remove children virtex at specified index
// fails with return value NULL if literal virtex or index out of range
virtex* vtx_remove(virtex* v, unsigned int index) {
  if (v->type == VT_LITERAL || index >= v->childrenCount)
    return NULL;
  virtex* poppedVirtex = v->childrenNodes[index].item;
  for (; index < v->childrenCount - 1; index++) {
    v->childrenNodes[index] = v->childrenNodes[index + 1];
  }
  v->childrenCount--;
  return poppedVirtex;
}

// set text of virtex
// fails with return value -1 if non-terminal virtex
int vtx_text(virtex* v, char* str) {
  if (v->type != VT_LITERAL)
    return -1;
  char* str_copy = malloc(sizeof(char) * (strlen(str) + 1));
  strcpy(str_copy, str);
  free(v->childrenNodes[0].text);
  v->childrenNodes[0].text = str_copy;
  return 0;
}

void vtx_print(virtex* v) {
  format* f = fmt_dispatch(v);
  fmt_print(f);
  fmt_destroy(f);
}
