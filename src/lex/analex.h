#ifndef ANALEX_H
#define ANALEX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../global.h"

// token types definition
typedef enum {
    PO,             // '('
    PF,             // ')'
    OP,             // '¬', '∧', '∨', '⇒'
    PROP,           // e.g. "p21"
    PRODUIT         // "→"
} token_type;

// lexeme definition
typedef struct {
    token_type type; // e.g. Prop
    char value[10];  // e.g. "p2" or NULL
} lexeme_;
typedef lexeme_ *lexeme;

// lexeme list definition
#define MAX_LEXMEMES 50
typedef struct {
    lexeme lexemes[MAX_LEXMEMES]; // a list of lexemes
    int size;                     // number of lexemes contained in the list
    bool error;                   // = 1 indicate exists error
} lexeme_list_;
typedef lexeme_list_ *lexeme_list;

// Unicode characters
typedef struct {
    const char *name;
    const char unicode[4];
    int size;
} UnicodeChar;


lexeme_list analyseur_lexical(const char *);
void print_lexeme_list(lexeme_list);

// Define UTF-8 Characters
typedef struct {
    char *name;
    char unicode[4];
    int size;
} Utf8Char;



#define NUM_UTF8_CHARS (int)(sizeof(UTF8_CHARS) / sizeof(Utf8Char))

#endif