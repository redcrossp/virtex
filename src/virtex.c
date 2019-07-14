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
  if (type == VT_LITERAL){
	  v->childNodes[0].text = malloc(sizeof(char));
  }
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

// remove child vtx at specified index
// fails with return value NULL if literal vtx or index out of range
vtx* vtx_remove(vtx* v, unsigned int index) {
  if (v->type == VT_LITERAL || index >= v->childCount)
    return NULL;
  vtx* poppedVirtex = v->childNodes[index].item;
  for (; index < v->childCount - 1; index++) {
    v->childNodes[index] = v->childNodes[index + 1];
  }
  v->childCount--;
  return poppedVirtex;
}

// set text of vertex
// fails with return value -1 if non-terminal vertex
int vtx_text(vtx* v, char* str) {
  if (v->type != VT_LITERAL)
    return -1;
  char* str_copy = malloc(sizeof(char) * (strlen(str) + 1));
  strcpy(str_copy, str);
  free(v->childNodes[0].text);
  v->childNodes[0].text = str_copy;
  return 0;
}

