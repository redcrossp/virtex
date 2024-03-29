#ifndef _VIRTEX_H_
#define _VIRTEX_H_

#define VTX_INIT_CHILDCAPACITY (4)

typedef union node node;
typedef struct virtex virtex;

union node {
  char* text;
  virtex* item;
};

enum vtxType {
  VT_LITERAL,
  VT_SUM,
  VT_DIFFERENCE,
  VT_PRODUCT,
  VT_FRACTION,
  VT_EXPONENT,
  VT_BIGSUM,
  VT_BIGPROD,
  VT_SQRT,
  VT_FUNCTION
};
typedef enum vtxType vtxType;

struct virtex {
  enum vtxType type;
  unsigned int childrenCount;   // item count of child virtexes
  unsigned int childrenCapacity; // item capacity of child virtex array
  union node* childrenNodes;    // item values of child virtexes
};

virtex* vtx_create(vtxType type);
void vtx_destroy(virtex* v);
int vtx_insert(virtex* v, virtex* insert);
virtex* vtx_remove(virtex* v, unsigned int index);
int vtx_text(virtex* v, char* str);
void vtx_print(virtex* v);

#endif
