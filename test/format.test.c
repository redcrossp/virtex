#include <string.h>
#include <stdio.h>

#define FORMAT_LITERAL_EXPECT "euler"
TEST("Format Literal") {
  virtex* v = vtx_create(VT_LITERAL);
  vtx_text(v, "euler");

  char** fmt_string = vtx_format(v);
  EXPECT(v->width == strlen(FORMAT_LITERAL_EXPECT));
  EXPECT(v->height == 1);
  EXPECT(v->baseline == 0);
  EXPECT(strcmp(*fmt_string, FORMAT_LITERAL_EXPECT) == 0);

  fmt_free(v, fmt_string);
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
  
  char** fmt_string = vtx_format(v);
  EXPECT(v->width == strlen(FORMAT_SUM_EXPECT));
  EXPECT(v->height == 1);
  EXPECT(v->baseline == 0);
  EXPECT(strcmp(*fmt_string, FORMAT_SUM_EXPECT) == 0);

  fmt_free(v, fmt_string);
  vtx_destroy(v);
  PASS();
}
  
