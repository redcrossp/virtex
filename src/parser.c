#include<stdlib.h>
#include<stdio.h>

#include "parser.h"

token* create_token(){
  token* t = malloc(sizeof(token));
  t->sym = malloc(MAX_TOKEN_SIZE * sizeof(char));
  t->sym[0] = 0;
  t->size = 0;
  return t;
}

void destroy_token(token* t){
  free(t->sym);
  free(t);
}

int append_token(token* t, char c){
  if (t->size == MAX_TOKEN_SIZE)
    return 0;
  t->sym[t->size] = c;
  t->size++;
  t->sym[t->size] = '\0';
  return 1;
}

void print_token(token* t){
  printf("size: %d; token: ", t->size);
  for (int i = 0; i < t->size; i++)
    printf("%c", t->sym[i]);
  printf("\n");
//  printf("|\n");
}

int token_equals(token* t1, token* t2){
  if (t1->size != t2->size)
    return 0;
  for (int i = 0; i < t1->size; i++)
    if (t1->sym[i] != t2->sym[i])
      return 0;
  return 1;
}

stack* create_stack(){
  stack* s = malloc(sizeof(stack));
  s->syms = malloc(MAX_STACK_SIZE * sizeof(token*));
  //TODO: Uninitialized
  s->size = 0;
  return s;
}

void destroy_stack(stack* s){
  for (int i = 0; i < s->size; i++)
    destroy_token(s->syms[i]);
  free(s->syms);
  free(s);
}

int push_stack(stack* s, token* t){
  if (s->size == MAX_STACK_SIZE || t->size == 0)
    return 0;
  s->syms[s->size] = t;
  s->size++;
  return 1;
}

token* pop_stack(stack* s){
  if (s->size == 0)
    return NULL;
  s->size--;
  return s->syms[s->size];
}

//CHECK VALGRIND
token* peek_stack(stack* s){
  if (s->size == 0)
    return NULL;
  return s->syms[s->size - 1];
}

void print_stack(stack* s){
  for (int i = 0; i < s->size; i++)
    print_token(s->syms[i]);
}

vtx_stack* create_vtx_stack(){
  vtx_stack* vs = malloc(sizeof(vtx_stack));
  vs->vtcs = malloc(MAX_STACK_SIZE * sizeof(virtex*));
  //TODO: Uninitialized
  vs->size = 0;
  return vs;
}

void destroy_vtx_stack(vtx_stack* vs){
  for (int i = 0; i < vs->size; i++)
    vtx_destroy(vs->vtcs[i]);
  free(vs->vtcs);
  free(vs);
}

int push_vtx_stack(vtx_stack* vs, virtex* v){
  if (vs->size == MAX_STACK_SIZE)
    return 0;
  vs->vtcs[vs->size] = v;
  vs->size++;
  return 1;
}

virtex* pop_vtx_stack(vtx_stack* vs){
  if (vs->size == 0)
    return NULL;
  vs->size--;
  return vs->vtcs[vs->size];
}

int is_operator(char c){
  for (int i = 0; i < NUM_OPERATORS; i++){
    if (c == OPERATORS[i])
      return (i + 1);
  }
  return 0;
}

int is_escape(char c){
  return (c == ESCAPE);
}

int token_is_operator(token* t){
  if (t == NULL)
    return 0;
  return is_operator(t->sym[0]);
}

int token_is_escaped(token* t){
  if (t == NULL)
    return 0;
  return is_escape(t->sym[0]);
}

int token_escape_identify(token* t){
  if (!token_is_escaped(t))
    return 0;
  if (t->size == 5 && t->sym[1] == 'f' && t->sym[2] == 'r' && t->sym[3] == 'a' && t->sym[4] == 'c')
    return 1;
  return -1;
}

int is_delim_char(char c){
  return (c == ' ' || c == '\n' || c == EOF);
}

int is_brace(char c){
  for (int i = 0; i < NUM_BRACES; i++)
    if (BRACES[i] == c)
      return (i + 1);
  return 0;
}

int token_is_brace(token* t){
  if (t == NULL)
    return 0;
  return is_brace(t->sym[0]);
}

int operator_precedence(token* t){
  int operator = token_is_operator(t);
  switch (operator) {
    case 1: return 3;
        break;
    case 2: return 2;
        break;
    case 3: return 2;
        break;
  }
  return -1;
}

/* -1 Left
 * +1 Right
 *  0 Both */
int operator_associativity(token* t){
  int operator = token_is_operator(t);
  switch (operator){
    case 1: return 0;
        break;
    case 2: return 0;
        break;
    case 3: return -1;
  }
  return -2;
}

virtex* postfix_to_virtex(stack* postfix){
  stack* reverse = create_stack();
  vtx_stack* arg_stack = create_vtx_stack();
  while (postfix->size > 0)
    push_stack(reverse, pop_stack(postfix));
  while (reverse->size > 0){
    token* curr = pop_stack(reverse);
    if (!token_is_operator(curr) && ! token_is_escaped(curr)){
      virtex* temp = vtx_create(VT_LITERAL);
      vtx_text(temp, curr->sym);
      destroy_token(curr);
      push_vtx_stack(arg_stack, temp);
    } else if (token_is_operator(curr)){
      virtex* temp;
      int op = token_is_operator(curr);
      switch (op){
        case 1: temp = vtx_create(VT_PRODUCT);
            break;
        case 2: temp = vtx_create(VT_SUM);
            break;
        case 3: temp = vtx_create(VT_DIFFERENCE);
            break;
      }
      virtex* second = pop_vtx_stack(arg_stack);
      vtx_insert(temp, pop_vtx_stack(arg_stack));
      vtx_insert(temp, second);
      push_vtx_stack(arg_stack, temp);
    } else {
      virtex* temp;
      int es = token_escape_identify(curr);
      switch (es){
        case 1: temp = vtx_create(VT_FRACTION);
            break;
      }
      virtex* second = pop_vtx_stack(arg_stack);
      vtx_insert(temp, pop_vtx_stack(arg_stack));
      vtx_insert(temp, second);
      push_vtx_stack(arg_stack, temp);
    }
  }
  virtex* ans = pop_vtx_stack(arg_stack);
  destroy_vtx_stack(arg_stack);
  destroy_stack(reverse);
  return ans;
}

stack* read_to_postfix(stack* token_stack){
  stack* postfix = create_stack();
  stack* reverse = create_stack();
  stack* op_stack = create_stack();
  while (token_stack->size > 0)
    push_stack(reverse, pop_stack(token_stack));
  //SHUNTING YARD LET'S GO
  while (reverse->size > 0){
    token* curr = pop_stack(reverse);
    if (token_is_escaped(curr)){
      push_stack(op_stack, curr);
    } else if (token_is_operator(curr)){
      while (token_is_escaped(peek_stack(op_stack)) || operator_precedence(peek_stack(op_stack)) > operator_precedence(curr) || (token_is_brace(peek_stack(op_stack)) != 1 && operator_precedence(peek_stack(op_stack)) == operator_precedence(curr) && operator_associativity(peek_stack(op_stack)) < 1))
        push_stack(postfix, pop_stack(op_stack));
      push_stack(op_stack, curr);
    } else if (token_is_brace(curr) == 1){
      push_stack(op_stack, curr);
    } else if (token_is_brace(curr) == 2){
      while (token_is_brace(peek_stack(op_stack)) != 1)
        push_stack(postfix, pop_stack(op_stack));
      pop_stack(op_stack);
    } else 
      push_stack(postfix, curr);
  }
  while (op_stack->size > 0)
    push_stack(postfix, pop_stack(op_stack));
  destroy_stack(op_stack);
  destroy_stack(reverse);
  return postfix;
}

stack* parse_to_tokens(FILE* input_file){
  stack* token_stack = create_stack();
  int c = -1;
  token* t = NULL;
  do {
    c = fgetc(input_file);
    if (t == NULL){
      if (!is_delim_char(c)){
        t = create_token();
        append_token(t, c);
        if (is_brace(c) || (is_operator(c))){
          push_stack(token_stack, t);
          t = NULL;
        }
      }
    } else if (is_delim_char(c)){
      if (token_is_operator(t)){
        push_stack(token_stack, t);
        t = NULL;
      }
    } else if (is_operator(c)){
      push_stack(token_stack, t);
      t = create_token();
      append_token(t, c);
      push_stack(token_stack, t);
      t = NULL;
    } else if (is_brace(c)){
      push_stack(token_stack, t);
      t = create_token();
      append_token(t, c);
      push_stack(token_stack, t);
      t = NULL;
    } else if (is_escape(c)){
      push_stack(token_stack, t);
      t = create_token();
      append_token(t, c);
    } else {
      append_token(t, c);
    }
  } while (c != EOF);
  //TODO: Restructure loop?
  if (t != NULL){
    push_stack(token_stack, t);
  }
  return token_stack;
}

void parse_file(char* file_name){
  FILE* input_file = fopen(file_name, "r");
  stack* token_stack = parse_to_tokens(input_file);
  fclose(input_file);
  stack* postfix = read_to_postfix(token_stack);
  destroy_stack(token_stack);
  printf("postfix\n");
  print_stack(postfix);
  virtex* result = postfix_to_virtex(postfix);
  vtx_print(result);
  destroy_stack(postfix);
  vtx_destroy(result);
}
