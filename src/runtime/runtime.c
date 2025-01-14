#include <stdio.h>
#include <stdlib.h>
#include "runtime.h"

int VM[VM_SIZE];     
int vm_index = 0;    

base processeur[50]; 
#define STACK_MAX  100

typedef struct {
    int stack[STACK_MAX];
    int top;
} Stack;

static Stack s_Mapile = {
    .top = -1
};

void push(int value) {
    if (s_Mapile.top >= STACK_MAX - 1) {
        fprintf(stderr, "[Runtime Error] Stack overflow.\n");
        exit(EXIT_FAILURE);
    }
    s_Mapile.stack[++s_Mapile.top] = value;
}

int pop(void) {
    if (s_Mapile.top < 0) {
        fprintf(stderr, "[Runtime Error] Stack underflow.\n");
        exit(EXIT_FAILURE);
    }
    return s_Mapile.stack[s_Mapile.top--];
}

Mapile_t Mapile = {
    .push = push,
    .pop  = pop
};

void Execute(int start_addr) {
    int ip = start_addr;  

    while (1) {
        int instr = VM[ip++];  // Fetch next instruction

        if (instr == 99) {
            break;
        } else if (instr < 0) {
            Mapile.push(instr);
        } else {
            if (processeur[instr] == NULL) {
                fprintf(stderr, "[Runtime Error] Invalid operator index %d.\n", instr);
                exit(EXIT_FAILURE);
            }
            // Call the corresponding operator function
            processeur[instr]();
        }
    }
}

