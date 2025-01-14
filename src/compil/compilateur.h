// compilateur.h
#ifndef COMPILATEUR_H
#define COMPILATEUR_H

#include "../syn/anasynt.h"   
#include "../runtime/runtime.h" // for VM, etc.

void init_processor(void);

int compile_ast_to_vm(AST root);

#endif
