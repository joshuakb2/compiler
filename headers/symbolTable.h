/* Joshua Baker		jkb150030
 * Dr. Gupta
 * CS 4386.001
 * 
 * symbolTable.h
 * 
 * This header file defines the structures used to represent
 * symbols in the symbol table and the hash table.
 * It also forward-declares the functions that implement the symbol
 * table and hash table functionality, defined in symbolTable.c.
 */

#ifndef symbolTable_h
#define symbolTable_h

#include "structTypes.h"

//	Holds the variable name and variable type (bool or int)
typedef struct symbol_s {
    char * key;
    varType_e type;
} symbol;

//	Holds the index of the symbol in the symbol table and
//	a pointer to the next node in case there are collisions.
typedef struct st_node_s {
    int index;
    struct st_node_s * next;
} st_node;

//	Returns the actual symbol from the handle
symbol * getSymbol(int i);

//	Returns the index of the symbol in the symbol table
//	according to the variable name
int getHandle(const char * const name);

//	Adds a new variable to the symbol table. If this symbol already
//	exists in the symbol table, returns -1.
int addSymbol(const char * const name, varType_e type);

//	Set up global vars.
void initSymbolTable();

//	Destroy global vars.
void freeSymbolTable();

#endif
