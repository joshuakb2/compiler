#ifndef symbolTable_h
#define symbolTable_h

#include "structTypes.h"

typedef struct symbol_s {
    char * key;
    varType_e type;
} symbol;

typedef struct st_node_s {
    int index;
    struct st_node_s * next;
} st_node;

symbol * getSymbol(int i);
int getHandle(const char * const name);
int addSymbol(const char * const name, varType_e type);
void initSymbolTable();
void freeSymbolTable();

#endif
