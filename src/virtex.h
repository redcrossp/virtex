#ifndef _VIRTEX_H_
#define _VIRTEX_H_

typedef union node node;
typedef struct vtx vtx;

union node {
  char* text;
  vtx* item;
};

enum vtx_type {
  et_lit,
  et_sum,
  et_diff,
  et_prod,
  // et_quot,
  et_frac,
  // et_exp,
  // et_bigsum,
  // et_bigprod,
  // et_sqrt,
  // et_func
};
typedef enum vtx_type vtx_type;

struct vtx {
  enum vtx_type type;
  unsigned int itemc;
  unsigned int itemcap;
  union node* itemv;
  unsigned int width;
  unsigned int baseln;
  unsigned int height;
};

vtx* vtx_create(vtx_type type);
void vtx_destroy(vtx* v);
int vtx_insert(vtx* v, vtx* insert);
// int vtx_remove(vtx* v);
int vtx_text(vtx* v, char* str);
// defined in format.c
char** vtx_format(vtx* v);
void vtx_print(vtx* v);

#endif
