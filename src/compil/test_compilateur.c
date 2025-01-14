// test_compilateur.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lex/analex.h"
#include "../syn/anasynt.h"
#include "../sem/anasem.h"
#include "compilateur.h"
#include "../runtime/runtime.h" 

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <FILENAME>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);
        return 1;
    }

    init_processor();

    vm_index = 0; 

    char line[256];
    int line_num = 1;

    while (fgets(line, sizeof(line), fp)) {
        printf("Line %d: %s", line_num, line);

        lexeme_list llist = analyseur_lexical(line);
        if (!llist || llist->error) {
            printf("[Skipping: Lexical error]\n\n");
            line_num++;
            continue;
        }

        AST root = analyser_syntaxique(llist);
        if (!root) {
            printf("[Skipping: Syntax error]\n\n");
            line_num++;
            continue;
        }

        AST sem_root = analyseur_semantique(root);
        if (!sem_root) {
            printf("[Skipping: Semantic error]\n\n");
            line_num++;
            continue;
        }

        // 4) Compilation
        int cfa = compile_ast_to_vm(sem_root);
        if (cfa < 0) {
            printf("[Compilation error]\n\n");
            line_num++;
            continue;
        }
        printf("Compiled to VM at start address = %d\n", cfa);

        // Print VM
        printf("Current VM code:\n");
        for (int i = 0; i < vm_index; i++) {
            printf("%d ", VM[i]);
        }
        printf("\n\n");

        line_num++;
    }

    fclose(fp);
    return 0;
}
