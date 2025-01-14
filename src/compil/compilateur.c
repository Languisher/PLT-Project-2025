// compilateur.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compilateur.h"
#include "../sem/anasem.h"   
#include "../lex/analex.h"   
#include "../runtime/runtime.h"

extern base processeur[50];  // function pointer array

// op implementations
//    read operands
// .  ex
// .  write back
void Non(void) {
    int x = Mapile.pop(); 
    // True = -1, False = 0
    int result = (x == -1) ? 0 : -1; 
    Mapile.push(result);
}

void Ou(void) {
    int r = Mapile.pop();
    int l = Mapile.pop();
    int result = ((l == -1) || (r == -1)) ? -1 : 0;
    Mapile.push(result);
}

void Et(void) {
    int r = Mapile.pop();
    int l = Mapile.pop();
    int result = ((l == -1) && (r == -1)) ? -1 : 0;
    Mapile.push(result);
}

void Implique(void) {
    int q = Mapile.pop();
    int p = Mapile.pop();
    // p => q is (!p) or q
    if (p == -1 && q == 0)
        Mapile.push(0);   // false
    else
        Mapile.push(-1); // true
}

// set of instructions (operators)
void init_processor(void) {
    processeur[0] = &Non;
    processeur[1] = &Ou;
    processeur[2] = &Et;
    processeur[3] = &Implique;
}

// AST -> VM
extern int VM[200];    
extern int vm_index;   

// mapping
static int map_op_to_code(const char *op_name) {
    if (strcmp(op_name, "NON") == 0)       return 0; 
    if (strcmp(op_name, "OU") == 0)        return 1;
    if (strcmp(op_name, "ET") == 0)        return 2;
    if (strcmp(op_name, "IMPLIQUE") == 0)  return 3;
    if (strcmp(op_name, "PRODUIT") == 0)   return 4; 
    fprintf(stderr, "[Compile Error] Unknown operator: %s\n", op_name);
    return -1;
}

static int var_counter = -1;

// Recursive function
static void compile_node(AST node) {
    if (!node) return; 

    switch (node->type) {
    case AST_PROP: {
        int code_for_var = var_counter--;
        VM[vm_index++] = code_for_var;
        break;
    }
    case AST_OP: {
        // post-order: compile left, compile right, then operator
        compile_node(node->left);
        if (node->right) {
            compile_node(node->right);
        }
        int op_code = map_op_to_code(node->operator);
        VM[vm_index++] = op_code;
        break;
    }
    case AST_PRODUCE: {
        // same logic if we treat PRODUIT like a binary operator
        compile_node(node->left);
        compile_node(node->right);
        int code = map_op_to_code("PRODUIT");
        VM[vm_index++] = code;
        break;
    }
    default:
        fprintf(stderr, "[Compile Error] Unknown AST node type.\n");
        break;
    }
}

int compile_ast_to_vm(AST root) {
    if (!root) {
        fprintf(stderr, "[Compile Error] AST is null!\n");
        return -1;
    }

    // remember start address
    int start_addr = vm_index;

    compile_node(root);

    // place sentinel (99) to mark end
    VM[vm_index++] = 99;

    return start_addr;
}
