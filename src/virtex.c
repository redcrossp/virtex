#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "virtex.h"

virtex* vtx_create(vtxType type) {
  virtex* v = malloc(sizeof(virtex));
  v->type = type;
  v->childCount = 0;
  v->childCapacity = VTX_INIT_CHILDCAPACITY;
  v->childNodes = malloc(sizeof(node) * VTX_INIT_CHILDCAPACITY);
  if (type == VT_LITERAL) {
    v->childNodes[0].text = malloc(sizeof(char));
  }
  return v;
}

// destroys virtex AND children
void vtx_destroy(virtex* v) {
  if (v->type == VT_LITERAL) {
    free(v->childNodes[0].text);
  } else {
    for (unsigned int i = 0; i < v->childCount; i++)
      vtx_destroy(v->childNodes[i].item);
  }
  free(v->childNodes);
  free(v);
}

// add child virtex, doubling size of children array if necessary
// fails with return value -1 if literal virtex
int vtx_insert(virtex* v, virtex* insert) {
  if (v->type == VT_LITERAL)
    return -1;
  if (v->childCount >= v->childCapacity) {
    v->childCapacity *= 2;
    v->childNodes = realloc(v->childNodes, sizeof(node) * v->childCapacity);
  }
  v->childNodes[v->childCount++].item = insert;
  return 0;
}

// remove child virtex at specified index
// fails with return value NULL if literal virtex or index out of range
virtex* vtx_remove(virtex* v, unsigned int index) {
  if (v->type == VT_LITERAL || index >= v->childCount)
    return NULL;
  virtex* poppedVirtex = v->childNodes[index].item;
  for (; index < v->childCount - 1; index++) {
    v->childNodes[index] = v->childNodes[index + 1];
  }
  v->childCount--;
  return poppedVirtex;
}

// set text of virtex
// fails with return value -1 if non-terminal virtex
int vtx_text(virtex* v, char* str) {
  if (v->type != VT_LITERAL)
    return -1;
  char* str_copy = malloc(sizeof(char) * (strlen(str) + 1));
  strcpy(str_copy, str);
  free(v->childNodes[0].text);
  v->childNodes[0].text = str_copy;
  return 0;
}

