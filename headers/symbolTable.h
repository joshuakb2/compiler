#ifndef symbolTable_h
#define symbolTable_h

#include "structTypes.h"

typedef struct symbol_s {
    char * key;
    enum varType type;
} symbol;

typedef struct st_node_s {
    int index;
    struct st_node_s * next;
} st_node;

symbol * getSymbol(int i);
int addSymbol(const char * const name, enum varType type);
void initSymbolTable();
void freeSymbolTable();

#endif
