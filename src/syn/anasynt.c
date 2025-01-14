// anasynt.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "anasynt.h"        
#include "../lex/analex.h" 

// record the pos in the lexeme list
static lexeme_list current_list = NULL;
static int current_index = 0;

static AST parseStart();

static AST parseFormula();
static AST parseImplication();
static AST parseDisjunction();
static AST parseConjunction();
static AST parseNegation();

AST analyser_syntaxique(lexeme_list list) {
    current_list = list;
    current_index = 0;

    AST root = parseStart();  

    // If there exists lexemes that has not been parsed, then
    // there exists errors.
    if (root && current_index < list->size) {
        fprintf(stderr, "[Syntax Error] Extra tokens at end.\n");
        root = NULL;
    }

    return root;
}

static void advance() {
    if (current_index < current_list->size) {
        current_index++;
    }
}

static lexeme current_token() {
    if (current_index < current_list->size) {
        return current_list->lexemes[current_index];
    }
    return NULL; // = No tokens left
}

AST parse_start(lexeme_list list) {
    current_list = list;
    current_index = 0;

    AST root = parseStart();

    if (root && current_index < current_list->size) {
        fprintf(stderr, "[Syntax Error] Extra tokens after complete parse.\n");
        return NULL;
    }
    return root;
}

// <Start> ::= <Formula>
static AST parseStart() {
    // <S> = <Formula>
    return parseFormula();
}


// <Formula> ::= <Implication> 
//             | <Implication> PRODUIT <Implication>
static AST parseFormula() {
    AST left = parseImplication();
    if (!left) return NULL;

    if (current_token() && current_token()->type == PRODUIT) {
        advance();

        // parse the second implication
        AST right = parseImplication();
        if (!right) return NULL;

        AST node = (AST)malloc(sizeof(ast_node));
        node->type = AST_PRODUCE;
        strcpy(node->operator, "PRODUIT");
        node->prop[0] = '\0'; 
        node->left = left;
        node->right = right;
        return node;
    }

    return left;
}

// <Implication> ::= <Disjunction> 
//                 | <Disjunction> OP(⇒) <Disjunction>
static AST parseImplication() {
    AST left = parseDisjunction();
    if (!left) return NULL;

    if (current_token() 
        && current_token()->type == OP
        && strcmp(current_token()->value, "IMPLIQUE") == 0) {

        lexeme tok = current_token();
        advance();

        AST right = parseDisjunction();
        if (!right) return NULL;

        AST node = (AST)malloc(sizeof(ast_node));
        node->type = AST_OP;
        strcpy(node->operator, tok->value); // =IMPLIQUE
        node->prop[0] = '\0';
        node->left = left;
        node->right = right;

        return node;
    }

    return left;
}

// <Disjunction> ::= <Conjunction>
//                 | <Disjunction> OP(∨) <Conjunction>
static AST parseDisjunction() {
    AST left = parseConjunction();
    if (!left) return NULL;

    while (current_token() 
           && current_token()->type == OP
           && strcmp(current_token()->value, "OU") == 0) {

        lexeme tok = current_token(); 
        advance(); 

        AST right = parseConjunction();
        if (!right) return NULL;

        AST node = (AST)malloc(sizeof(ast_node));
        node->type = AST_OP;
        strcpy(node->operator, tok->value); //  ="OU"
        node->prop[0] = '\0';
        node->left = left;
        node->right = right;

        left = node;
    }

    return left;
}

// <Conjunction> ::= <Negation>
//                 | <Conjunction> OP(∧) <Negation>
static AST parseConjunction() {
    AST left = parseNegation();
    if (!left) return NULL;

    while (current_token() 
           && current_token()->type == OP
           && strcmp(current_token()->value, "ET") == 0) {

        lexeme tok = current_token();
        advance(); 

        AST right = parseNegation();
        if (!right) return NULL;

        AST node = (AST)malloc(sizeof(ast_node));
        node->type = AST_OP;
        strcpy(node->operator, tok->value); // ="ET"
        node->prop[0] = '\0';
        node->left = left;
        node->right = right;

        left = node;  
    }

    return left;
}

// <Negation> ::= OP(¬) <Negation>
//              | "(" <Formula> ")"
//              | PROP
static AST parseNegation() {
    lexeme tok = current_token();
    if (!tok) {
        fprintf(stderr, "[Syntax Error] Unexpected end of tokens in parseNegation.\n");
        return NULL;
    }

    if (tok->type == OP && strcmp(tok->value, "NON") == 0) {
        AST node = (AST)malloc(sizeof(ast_node));
        node->type = AST_OP;
        strcpy(node->operator, "NON");
        node->prop[0] = '\0';
        node->left = NULL;
        node->right = NULL;

        advance(); 

        AST sub = parseNegation();
        if (!sub) {
            free(node);
            return NULL;
        }
        node->left = sub;
        return node;
    }

    if (tok->type == PO) {
        advance(); 

        AST inside = parseFormula();
        if (!inside) return NULL;

        if (!current_token() || current_token()->type != PF) {
            fprintf(stderr, "[Syntax Error] Missing closing parenthesis.\n");
            return NULL;
        }
        advance(); 
        return inside;
    }

    if (tok->type == PROP) {
        AST node = (AST)malloc(sizeof(ast_node));
        node->type = AST_PROP;
        strcpy(node->prop, tok->value);  
        node->operator[0] = '\0';
        node->left = NULL;
        node->right = NULL;
        advance(); 
        return node;
    }

    fprintf(stderr, "[Syntax Error] Expected 'NON', '(', or PROP but found '%s'\n",
            tok->value);
    return NULL;
}


void print_ast(AST node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }

    switch (node->type) {
        case AST_PROP:
            printf("PROP(%s)\n", node->prop);
            break;
        case AST_OP:
            printf("OP(%s)\n", node->operator);
            break;
        case AST_PRODUCE:
            printf("PRODUIT\n");
            break;
    }

    print_ast(node->left, indent + 1);
    print_ast(node->right, indent + 1);
}
