#include <string.h>

#include <stdlib.h>
#include "virtex.h"
#include "format.h"

char* strpad(char* str, size_t n) {
  size_t len = strlen(str);
  size_t end = len + n;
  for (size_t i = len; i < end; i++)
    str[i] = ' ';
  str[end] = '\0';
  return str;
}

formatter fmt_dispatch[5] = {
  fmt_lit,
  fmt_sum,
  fmt_diff,
  fmt_prod,
  fmt_frac
};

char** vtx_format(vtx* v) {
  return fmt_dispatch[v->type](v);
}

char*** fmt_children(vtx* v) {
  char*** format_str = malloc(sizeof(char**) * v->itemc);
  for (unsigned int i = 0; i < v->itemc; i++)
    format_str[i] = vtx_format(v->itemv[i].item);
  return format_str;
}

char** fmt_alloc(vtx* v) {
  char** out = malloc(sizeof(char*) * v->height);
  for (unsigned int i = 0; i < v->height; i++) {
    out[i] = malloc(sizeof(char) * (v->width + 1));
    out[i][0] = '\0';
  }
  return out;
}

void fmt_free(vtx* v, char*** fmt_str) {
  for (unsigned int i = 0; i < v->itemc; i++) {
    unsigned int item_height = v->itemv[i].item->height;
    for (unsigned int j = 0; j < item_height; j++) {
      free(fmt_str[i][j]);
    }
    free(fmt_str[i]);
  }
  free(fmt_str);
}

char** fmt_lit(vtx* v) {
  v->height = 1;
  v->baseln = 0;
  v->width = strlen(v->itemv[0].text);
  char** str = fmt_alloc(v);
  strcpy(str[0], v->itemv[0].text);
  return str;
}

char** fmt_list(vtx* v, const char* delimiter, const unsigned int d_length) {
  // each output row has text
  char*** fmt_str = fmt_children(v);
  set_size_sum(v);

  char** str = fmt_alloc(v);
  for (unsigned int x = 0; x < v->itemc; x++) {
    // addition symbols
    if (x != 0) {
      for (unsigned int y = 0; y < v->height; y++) {
        if (y == v->baseln)
          strcat(str[y], delimiter);
        else
          strpad(str[y], d_length);
      }
    }

    // items
    vtx* item = v->itemv[x].item;
    char** itemStr = fmt_str[x];
    for (unsigned int y = 0, itemY = item->baseln - v->baseln;
        y < v->height; y++, itemY++) {
      if (0 <= itemY && itemY < item->height) {
        // line is in subitem bounds
        strcat(str[y], itemStr[itemY]);
      } else {
        strpad(str[y], item->width);
      }
    }
  }
  fmt_free(v, fmt_str);
  return str;
}

char** fmt_sum(vtx* v) {
  const char* DELIMITER_SUM = " + ";
  const unsigned int DELIMITER_SUM_LENGTH = 3;
  return fmt_list(v, DELIMITER_SUM, DELIMITER_SUM_LENGTH);
}

char** fmt_diff(vtx* v) {
  const char* DELIMITER_DIFF = " - ";
  const unsigned int DELIMITER_DIFF_LENGTH = 3;
  return fmt_list(v, DELIMITER_DIFF, DELIMITER_DIFF_LENGTH);
}

char** fmt_prod(vtx* v) {
  const char* DELIMITER_PROD = " * ";
  const unsigned int DELIMITER_PROD_LENGTH = 3;
  return fmt_list(v, DELIMITER_PROD, DELIMITER_PROD_LENGTH);
}

// char** fmt_prod(vtx* v) {
//   char*** fmt_str = fmt_children(v);
//   v->height = get_height(v);
//   v->width = get_width(v);
//   char** str = str_alloc(v);
//   
//   for (unsigned int x = 0; x < v->itemc; x++) {
//     vtx* item = vtx->itemv[x].item;
//     char** itemStr = fmt_str[x];
//     for (unsigned int y = 0, itemY = item->baseln - v->baseln;
//         y < v->height; y++, itemY++) {
//       if (0 <= itemY && itemY < item->height) {
//         // line is in subitem bounds
//         strcat(str[y], itemStr[itemY]);
//       } else {
//         strpad(str[y], item->width);
//       }
//     }
//   }
//   fmt_free(v, fmt_str);
//   return str;
// }

char** fmt_frac(vtx* v) {
  char*** fmt_str = fmt_children(v);
  
  vtx* item0 = v->itemv[0].item;
  vtx* item1 = v->itemv[1].item;

  v->baseln = item0->height;
  v->height = item0->height + item1->height + 1;
  v->width = (item0->width >= item1->width ? item0->width : item1->width) + 2;

  unsigned int item0Start = (v->width - item0->width) / 2;
  unsigned int item1Start = (v->width - item1->width) / 2;

  char** str = fmt_alloc(v);
  for (unsigned int y = 0; y < v->height; y++) {
    if (y < item0->height) {
      strpad(str[y], item0Start);
      strcat(str[y], fmt_str[0][y]);
      strpad(str[y], v->width - item0->width - item0Start);
    } else if (y == item0->height) {
      for (unsigned int x = 0; x < v->width; x++)
        strcat(str[y], "-");
    } else {
      strpad(str[y], item1Start);
      strcat(str[y], fmt_str[1][y - item0->height - 1]);
      strpad(str[y], v->width - item1->width - item1Start);
    }
  }
  fmt_free(v, fmt_str);
  return str;
}

void set_size_sum(vtx* v) {
  unsigned int aboveln = 0;
  unsigned int belowln = 0;
  unsigned int width = 0;
  for (unsigned int i = 0; i < v->itemc; i++) {
    unsigned int itemHeight = v->itemv[i].item->height;
    unsigned int itemBaseln = v->itemv[i].item->baseln;
    if (itemBaseln > aboveln)
      aboveln = itemBaseln;
    if (itemHeight - itemBaseln > belowln)
      belowln = itemHeight - itemBaseln;
    width += v->itemv[i].item->width + 3;
  }
  v->baseln = aboveln;
  v->height = aboveln + belowln;
  v->width = width - 3;
}

