#include <string.h>
#include <stdio.h>

#define FORMAT_LITERAL_EXPECT "euler"
TEST("Format Literal") {
  virtex* v = vtx_create(VT_LITERAL);
  vtx_text(v, "euler");

  format* f = fmt_dispatch(v);
  EXPECT(f->width == strlen(FORMAT_LITERAL_EXPECT));
  EXPECT(f->height == 1);
  EXPECT(f->baseline == 0);
  EXPECT(strcmp(f->string[0], FORMAT_LITERAL_EXPECT) == 0);

  fmt_destroy(f);
  vtx_destroy(v);
  PASS();
}

#define FORMAT_SUM_EXPECT "3.14 + 2.718"
TEST("Format Sum") {
  virtex* v = vtx_create(VT_SUM);
  virtex* lit_pi = vtx_create(VT_LITERAL);
  virtex* lit_e = vtx_create(VT_LITERAL);

  vtx_text(lit_pi, "3.14");
  vtx_text(lit_e, "2.718");

  vtx_insert(v, lit_pi);
  vtx_insert(v, lit_e);

  format* f = fmt_dispatch(v);
  EXPECT(f->width == strlen(FORMAT_SUM_EXPECT));
  EXPECT(f->height == 1);
  EXPECT(f->baseline == 0);
  EXPECT(strcmp(f->string[0], FORMAT_SUM_EXPECT) == 0);

  fmt_destroy(f);
  vtx_destroy(v);
  PASS();
}
  
