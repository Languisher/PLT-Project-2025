#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../global.h"            

#include "../lex/analex.h"
#include "../syn/anasynt.h"
#include "../sem/anasem.h"

#include "../compil/compilateur.h"
#include "runtime.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <FILENAME>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        fprintf(stderr, "Cannot open file: %s\n", argv[1]);
        return 1;
    }

   
    char buffer[4096];
    size_t totalRead = 0;
    while (!feof(fp) && totalRead < sizeof(buffer) - 1) {
        size_t n = fread(buffer + totalRead, 1, sizeof(buffer) - 1 - totalRead, fp);
        if (n > 0) {
            totalRead += n;
        }
    }
    buffer[totalRead] = '\0'; 
    fclose(fp);

    lexeme_list llist = analyseur_lexical(buffer);
    if (!llist || llist->error) {
        fprintf(stderr, "[Error] Lexical analysis failed.\n");
        return 1;
    }

    AST root = analyser_syntaxique(llist);
    if (!root) {
        fprintf(stderr, "[Error] Syntax analysis failed.\n");
        return 1;
    }

    AST semantic_root = analyseur_semantique(root);
    if (!semantic_root) {
        fprintf(stderr, "[Error] Semantic analysis failed.\n");
        return 1;
    }

    //  Initialize the operator function table 
    init_processor();

    // Compile the AST into VM instructions
    int ad = compile_ast_to_vm(semantic_root);
    if (ad < 0) {
        fprintf(stderr, "[Error] Compilation to VM failed.\n");
        return 1;
    }

    Mapile.push(-1); // q12 = true
    Mapile.push(-1); // r5 = true
    Mapile.push(0);  // a = false
    Mapile.push(0);  // c = false

    Execute(ad);

    int result = Mapile.pop();

    if (result == -1) {
        printf("Proposition evaluated to TRUE (-1)\n");
    } else if (result == 0) {
        printf("Proposition evaluated to FALSE (0)\n");
    } else {
        printf("Proposition evaluated to unexpected value: %d\n", result);
    }

    return 0;
}
