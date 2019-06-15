#ifndef _VIRTEX_H_
#define _VIRTEX_H_

typedef struct vtx vtx;

union node {
  char* text;
  vtx* item;
} node;

enum vtx_type {

  et_lit,
  et_sum,
  // et_diff,
  // et_prod,
  // et_quot,
  et_frac,
  // et_exp,
  // et_bigsum,
  // et_bigprod,
  // et_sqrt,
  // et_func
};

struct vtx {
  enum vtx_type type;
  unsigned int itemc;
  union node* itemv;
  unsigned int width;
  unsigned int baseln;
  unsigned int height;
};

vtx* vtx_create(void);
void vtx_insert(vtx* v);
void vtx_remove(vtx* v);
void vtx_destroy(vtx* v);

#endif
