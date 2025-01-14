#include "analex.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./test_analex <FILENAME>\n");
        return 1;
    }

    const char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        return 1;
    }

    char line[256]; // suppose max number of line could be read is 256
    int line_number = 1;

    // Deal with each line
    while(fgets(line, sizeof(line), fp)) {
        lexeme_list result = analyseur_lexical(line);

        printf("Line %d: %s", line_number, line);
        printf("Lexical analysis result: ");
        print_lexeme_list(result);
        printf("\n\n");

        line_number++;
    }

    return 0;
}


