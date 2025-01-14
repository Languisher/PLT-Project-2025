// test_anasem.c
#include <stdio.h>
#include <stdlib.h>
#include "../lex/analex.h"
#include "../syn/anasynt.h"
#include "anasem.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <FILENAME>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);
        return 1;
    }

    char line[256];
    int line_number = 1;

    while (fgets(line, sizeof(line), fp)) {
        printf("Line %d: %s", line_number, line);

        lexeme_list llist = analyseur_lexical(line);
        if (!llist || llist->error) {
            printf("[Skipping: Lexical error]\n\n");
            line_number++;
            continue;
        }

        AST root = analyser_syntaxique(llist);
        if (!root) {
            printf("[Skipping: Syntax error]\n\n");
            line_number++;
            continue;
        }

        AST semantic_root = analyseur_semantique(root);
        if (!semantic_root) {
            printf("[Semantic error]\n\n");
            line_number++;
            continue;
        }

        printf("Valid AST.\n");
        

        print_ast(semantic_root, 0);
        printf("\n");
        line_number++;
    }

    fclose(fp);
    return 0;
}
