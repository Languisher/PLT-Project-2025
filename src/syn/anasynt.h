// anasynt.h
#ifndef ANASYNT_H
#define ANASYNT_H

#include "../lex/analex.h" 
#include <stdbool.h>

// AST node type
typedef enum {
    AST_PROP,
    AST_OP,
    AST_PRODUCE
} ast_node_type;

// AST node struct
typedef struct ast_node {
    ast_node_type type;
    char prop[10];                  // valid if type==AST_PROP
    char operator[10];              // valid if type==AST_OP
    struct ast_node *left;
    struct ast_node *right;
} ast_node;
typedef ast_node* AST;

AST analyser_syntaxique(lexeme_list list);

void print_ast(AST node, int indent);

#endif
