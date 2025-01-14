#include "analex.h"

Utf8Char UTF8_CHARS[] = {
    {"IMPLIQUE", {(char)0xE2, (char)0x87, (char)0x92}, 3}, // ⇒
    {"PRODUIT", {(char)0xE2, (char)0x86, (char)0x92}, 3},  // →
    {"NON", {(char)0xC2, (char)0xAC}, 2},                  // ¬
    {"ET", {(char)0xE2, (char)0x88, (char)0xA7}, 3},       // ∧
    {"OU", {(char)0xE2, (char)0x88, (char)0xA8}, 3}        // ∨
};

// The basic types
static bool is_whitespace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

static bool is_alpha(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

static bool is_digit(char c) {
    return (c >= '0' && c <= '9');
}


// Auxiliary function to help create a lexeme "instance"
static lexeme create_lexeme(token_type t, const char *val) {
    lexeme new_lex = (lexeme)malloc(sizeof(lexeme_));
    if (!new_lex) {
        fprintf(stderr, "Memory allocation error in creating new lexeme.\n");
        return NULL;
    }
    new_lex->type = t;
    if (val) {
        strncpy(new_lex->value, val, sizeof(new_lex->value) - 1);
        new_lex->value[sizeof(new_lex->value) - 1] = '\0';
    } else {
        new_lex->value[0] = '\0';
    }
    return new_lex;
}

static int match_utf8_char(const char *str, Utf8Char *matched_char) {
    for (int i = 0; i < NUM_UTF8_CHARS; i++) {
        if (strncmp(str, UTF8_CHARS[i].unicode, UTF8_CHARS[i].size) == 0) {
            if (matched_char) {
                *matched_char = UTF8_CHARS[i];
            }
            return UTF8_CHARS[i].size;
        }
    }
    return 0;
}

lexeme_list analyseur_lexical(const char *line_str) {
    lexeme_list result = (lexeme_list)malloc(sizeof(lexeme_list_));
    if (!result) {
        fprintf(stderr, "Memory allocation error in creating lexeme list.\n");
        return NULL;
    }

    result->size = 0;
    result->error = false;

    int i = 0, length = strlen(line_str);
    char buffer[10];
    int buffer_idx = 0;

    while (i < length) {
        if (is_whitespace(line_str[i])) {
            i++;
            continue;
        }

        if (result->size >= MAX_LEXMEMES) {
            fprintf(stderr, "[Lexical Error] Too many tokens.\n");
            result->error = true;
            return result;
        }

        if (line_str[i] == '(') {
            result->lexemes[result->size++] = create_lexeme(PO, "(");
            i++;
            continue;
        }

        if (line_str[i] == ')') {
            result->lexemes[result->size++] = create_lexeme(PF, ")");
            i++;
            continue;
        }

        Utf8Char matched_char;
        int utf8_len = match_utf8_char(&line_str[i], &matched_char);
        if (utf8_len > 0) {
            token_type type;
            if (strcmp(matched_char.name, "IMPLIQUE") == 0) {
                type = OP;
            } else if (strcmp(matched_char.name, "PRODUIT") == 0) {
                type = PRODUIT;
            } else if (strcmp(matched_char.name, "NON") == 0) {
                type = OP;
            } else if (strcmp(matched_char.name, "ET") == 0) {
                type = OP;
            } else if (strcmp(matched_char.name, "OU") == 0) {
                type = OP;
            } else {
                fprintf(stderr, "[Lexical Error] Unrecognized UTF-8 character.\n");
                result->error = true;
                return result;
            }
            result->lexemes[result->size++] = create_lexeme(type, matched_char.name);
            i += utf8_len;
            continue;
        }

        if (is_alpha(line_str[i])) {
            buffer_idx = 0;
            while (is_alpha(line_str[i]) || is_digit(line_str[i])) {
                if (buffer_idx < (int)sizeof(buffer) - 1) {
                    buffer[buffer_idx++] = line_str[i];
                }
                i++;
            }
            buffer[buffer_idx] = '\0';
            result->lexemes[result->size++] = create_lexeme(PROP, buffer);
            continue;
        }

        fprintf(stderr, "[Lexical Error] Unexpected character: '%c'\n", line_str[i]);
        result->error = true;
        return result;
    }

    return result;
}

// Print to stdout stream
void print_lexeme_list(const lexeme_list result) {
    if (result->error) {
        fprintf(stderr, "Lexical analysis has failed.\n");
        return;
    }

    printf("[");
    for (int i = 0; i < result->size; i++) {
        switch (result->lexemes[i]->type) {
            case PO:
                printf("PO, ");
                break;
            case PF:
                printf("PF, ");
                break;
            case OP:
                printf("Op(\"%s\"), ", result->lexemes[i]->value);
                break;
            case PROP:
                printf("Prop(\"%s\"), ", result->lexemes[i]->value);
                break;
            case PRODUIT:
                printf("PRODUIT, ");
                break;
            default:
                fprintf(stderr, "Unrecognized lexeme type.\n");
                return;
        }
    }
    printf("]\n");
}
