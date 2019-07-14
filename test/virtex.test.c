#include <string.h>
TEST("Create Virtex") {
  vtx* v = vtx_create(VT_LITERAL);

  EXPECT(v != NULL);
  EXPECT(v->type == VT_LITERAL);
  EXPECT(v->childCount == 0);
  EXPECT(v->childCapacity == VTX_INIT_CHILDCAPACITY);

  vtx_destroy(v);
  PASS();
}

TEST("Literal Virtex Set Text") {
  vtx* v = vtx_create(VT_LITERAL);
  
  EXPECT(vtx_text(v, "mahler") == 0);
  EXPECT(vtx_text(v, "joe") == 0);
  EXPECT(vtx_text(v, "euler") == 0);
  EXPECT(strcmp(v->childNodes[0].text, "euler") == 0);

  vtx_destroy(v);
  PASS();
}

TEST("Sum Virtex Set Text") {
  vtx* v = vtx_create(VT_SUM);

  EXPECT(vtx_text(v, "euler") == -1);

  vtx_destroy(v);
  PASS();
}

TEST("Literal Virtex Insert") {
  vtx* v = vtx_create(VT_LITERAL);
  vtx* i = vtx_create(VT_LITERAL);

  EXPECT(vtx_insert(v, i) == -1);
  EXPECT(v->childCount == 0);

  vtx_destroy(v);
  vtx_destroy(i);
  PASS();
}

TEST("Literal Virtex Remove") {
  vtx* v = vtx_create(VT_LITERAL);

  vtx_text(v, "text");
  EXPECT(vtx_remove(v, 0) == NULL);

  vtx_destroy(v);
  PASS();
}

TEST("Sum Virtex Insert") {
  vtx* v = vtx_create(VT_SUM);
  vtx* i = vtx_create(VT_LITERAL);

  EXPECT(vtx_insert(v, i) == 0);
  EXPECT(v->childCount == 1);
  EXPECT(v->childNodes[0].item->type == VT_LITERAL);

  vtx_destroy(v);
  PASS();
}

TEST("Sum Virtex Remove") {
  vtx* v = vtx_create(VT_SUM);
  vtx* i = vtx_create(VT_LITERAL);
  vtx* r = vtx_create(VT_LITERAL);

  vtx_insert(v, i);
  vtx_insert(v, r);

  EXPECT(vtx_remove(v, 0) == i);
  EXPECT(v->childCount == 1);
  EXPECT(vtx_remove(v, 0) == r);
  EXPECT(v->childCount == 0);
  EXPECT(vtx_remove(v, 0) == NULL);
  EXPECT(v->childCount == 0);

  vtx_destroy(v);
  vtx_destroy(i);
  vtx_destroy(r);
  PASS();
}

