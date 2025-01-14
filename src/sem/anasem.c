// anasem.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "anasem.h"

// operator struct
typedef struct {
    const char *name;
    int param_count;  // 1 for NOT, 2 for AND/OR/IMPLIQUE
    int cfa_code;
} operator_info;

// operator table
static operator_info OP_TABLE[] = {
    { "NON",       1,  0 },
    { "ET",        2,  1 },
    { "OU",        2,  2 },
    { "IMPLIQUE",  2,  3 },
    { "PRODUIT",   2,  4 },  
};

#define OP_TABLE_SIZE ((int)(sizeof(OP_TABLE)/sizeof(operator_info)))

// Lookup the operator
static operator_info* lookup_operator(const char *op_name) {
    for (int i = 0; i < OP_TABLE_SIZE; i++) {
        if (strcmp(OP_TABLE[i].name, op_name) == 0) {
            return &OP_TABLE[i];
        }
    }
    return NULL;
}

static int check_semantics_recursive(AST node);

// entry point
AST analyseur_semantique(AST root) {
    if (!root) {
        fprintf(stderr, "[Semantic Error] The AST is empty.\n");
        return NULL;
    }

    // do recursively
    int result = check_semantics_recursive(root);
    if (result != 0) {
        return NULL;
    }

    return root; 
}

// recursive function that applies to each node
static int check_semantics_recursive(AST node) {
    // reaches end
    if (!node) {
        return 0; 
    }

    switch (node->type) {
    case AST_PROP:
        if (node->left != NULL || node->right != NULL) {
            fprintf(stderr, "[Semantic Error] Proposition '%s' has children but shouldn't.\n", node->prop);
            return -1;
        }
        return 0;

    case AST_OP: {
        operator_info *info = lookup_operator(node->operator);
        if (!info) {
            fprintf(stderr, "[Semantic Error] Unknown operator '%s'.\n", node->operator);
            return -1;
        }
        
        // count children
        int child_count = 0;
        if (node->left)  child_count++;
        if (node->right) child_count++;

        // Compare with info->param_count
        if (child_count != info->param_count) {
            fprintf(stderr,
                    "[Semantic Error] Operator '%s' expects %d parameter(s), but found %d.\n",
                    node->operator, info->param_count, child_count);
            return -1;
        }

        if (node->left) {
            int res_left = check_semantics_recursive(node->left);
            if (res_left != 0) return -1;
        }
        if (node->right) {
            int res_right = check_semantics_recursive(node->right);
            if (res_right != 0) return -1;
        }

        return 0;
    }

    case AST_PRODUCE: {
        operator_info *info = lookup_operator("PRODUIT");
        if (!info) {
            fprintf(stderr, "[Semantic Error] 'PRODUIT' not found in operator table.\n");
            return -1;
        }

        int child_count = 0;
        if (node->left)  child_count++;
        if (node->right) child_count++;

        if (child_count != info->param_count) {
            fprintf(stderr,
                    "[Semantic Error] 'PRODUIT' expects %d children, but found %d.\n",
                    info->param_count, child_count);
            return -1;
        }

        if (node->left) {
            int res_left = check_semantics_recursive(node->left);
            if (res_left != 0) return -1;
        }
        if (node->right) {
            int res_right = check_semantics_recursive(node->right);
            if (res_right != 0) return -1;
        }

        return 0;
    }

    default:
        fprintf(stderr, "[Semantic Error] Unknown AST node type.\n");
        return -1;
    }
}
