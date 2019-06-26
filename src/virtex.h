#ifndef _VIRTEX_H_
#define _VIRTEX_H_

#define VTX_INIT_CHILDCAPACITY (4)

typedef union node node;
typedef struct vtx vtx;

union node {
  char* text;
  vtx* item;
};

enum vtxType {
  VT_LITERAL,
  VT_SUM,
  VT_DIFFERENCE,
  VT_PRODUCT,
  VT_QUOTIENT,
  VT_FRACTION,
  VT_EXPONENT,
  VT_BIGSUM,
  VT_BIGPROD,
  VT_SQRT,
  VT_FUNC
};
typedef enum vtxType vtxType;

struct vtx {
  enum vtxType type;
  unsigned int childCount;   // item count of child vertecies
  unsigned int childCapacity; // item capacity of child vertex array
  union node* childNodes;    // item values of child vertecies
  unsigned int width;
  unsigned int baseline;
  unsigned int height;
};

vtx* vtx_create(vtxType type);
void vtx_destroy(vtx* v);
int vtx_insert(vtx* v, vtx* insert);
// int vtx_remove(vtx* v);
int vtx_text(vtx* v, char* str);
// defined in format.c
char** vtx_format(vtx* v);
void vtx_print(vtx* v);

#endif
