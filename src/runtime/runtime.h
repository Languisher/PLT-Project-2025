#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdio.h>
#include <stdlib.h>

#define VM_SIZE 200


typedef void (*base)(void);


typedef struct {
    void (*push)(int);
    int  (*pop)(void);
} Mapile_t;

extern Mapile_t Mapile;

extern int VM[VM_SIZE];
extern int vm_index;

extern base processeur[50];


void Execute(int start_addr);

#endif 
