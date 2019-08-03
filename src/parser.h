#ifndef __PARSER_H__
#define __PARSER_H__

#include<stdlib.h>
#include<stdio.h>

#include "virtex.h"

#define ESCAPE ('\\')
#define OPERATORS ("*+-")
#define NUM_OPERATORS (4)
#define BRACES ("{}")
#define NUM_BRACES (2)

#define MAX_TOKEN_SIZE (16)
#define MAX_STACK_SIZE (32)

typedef struct token token;
typedef struct stack stack;
typedef struct vtx_stack vtx_stack;

struct token{
  char* sym;
  int size;
};

struct stack{
  token** syms;
  int size;
};

struct vtx_stack{
  virtex** vtcs;
  int size;
};

token* create_token();
void destroy_token(token*);
int append_token(token*, char);
void print_token(token* t);
int token_equals(token*, token*);

stack* create_stack();
void destroy_stack(stack*);
int push_stack(stack*, token*);
token* pop_stack(stack*);
token* peek_stack(stack*);
void print_stack(stack*);

vtx_stack* create_vtx_stack();
void destroy_vtx_stack();
int push_vtx_stack(vtx_stack*, virtex*);
virtex* pop_vtx_stack(vtx_stack*);

int is_operator(char);
int is_escape(char);
int token_is_operator(token*);
int token_is_escaped(token*);
int token_escape_identify(token*);
int is_delim_char(char);
int is_brace(char);
int token_is_brace(token*);
int operator_precedence(token*);
int operator_associativity(token*);

void parse_file(char*);
stack* parse_to_tokens(FILE*);
stack* read_to_postfix(stack*);
virtex* postfix_to_virtex(stack*);

#endif
