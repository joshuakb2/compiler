#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "sdbm.h"
#include "structTypes.h"
#include "symbolTable.h"

/*
 *  General strategy:
 *      symbolTable:
 *          Array of symbol structs.
 *
 *      hashMap:
 *          Array of st_nodes
 *          Each st_node contains an index referring to symbolTable
 *      
 *      symbol * getSymbol(int i):
 *          Function that returns a pointer to the const symbol struct
 *          at index i of symbolTable.
 *          Returns NULL if the index is invalid or there is no symbol at that index.
 *
 *      int addSymbol(const char * const name, varType_e type):
 *          Function that adds the given variable to the symbol table and returns
 *          the index of symbolTable where that data was placed.
 *          The returned integer can be used as a handle for that variable.
 *          If the given variable name is already present in the symbolTable,
 *          returns -1.
 *
 *      void initSymbolTable():
 *          Allocates memory for symbolTable and hashMap.
 *
 *      void freeSymbolTable():
 *          Frees the memory used by symbolTable and hashMap (including all st_nodes).
 */

bool symbolTable_inited = false;


//  Chosen as a prime number near 32.
const unsigned int HASH_MAP_SIZE = 31;

st_node ** hashMap;

int symbolTableSize = 16;
int symbolTableLength = 0;
symbol ** symbolTable;

symbol * getSymbol(int i) {
    if(!symbolTable_inited)
        return NULL;

    if(i < 0 || i >= symbolTableLength)
        return NULL;

    return symbolTable[i];
}

int getHandle(const char * const name) {
    int hashVal = hash(name) % HASH_MAP_SIZE;

    st_node * n = hashMap[hashVal];

    while(n != NULL) {
        if(strcmp(symbolTable[n->index]->key, name) == 0)
            return n->index;
        n = n->next;
    }

    return -1;
}

int addSymbol(const char * const name, varType_e type) {
    int hashVal = hash(name) % HASH_MAP_SIZE;

    st_node ** p = hashMap + hashVal;

    while(*p != NULL) {
        //  If name is already in the symbol table
        if(strcmp(symbolTable[(*p)->index]->key, name) == 0)
            return -1;
        p = &((*p)->next);
    }

    //  p points to the pointer to the next node

    //  Grow the symbol table
    if(symbolTableSize == symbolTableLength) {
        symbolTableSize *= 2;
        symbolTable = (symbol **) realloc(symbolTable, sizeof(symbol *) * symbolTableSize);
    }

    int index = symbolTableLength++;

    (*p) = (st_node *) malloc(sizeof(st_node));
    (*p)->index = index;
    (*p)->next = NULL;

    symbolTable[index] = (symbol *) malloc(sizeof(symbol));
    symbolTable[index]->key = (char *) malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(symbolTable[index]->key, name);

    symbolTable[index]->type = type;

    return index;
}

void initSymbolTable() {
    if(!symbolTable_inited) {
        hashMap = (st_node **) malloc(sizeof(st_node *) * HASH_MAP_SIZE);

        st_node ** st = hashMap;
        for(int i = 0; i < HASH_MAP_SIZE; i++)
            *st++ = NULL;

        symbolTable = (symbol **) malloc(sizeof(symbol *) * symbolTableSize);

        symbol ** sy = symbolTable;
        for(int i = 0; i < symbolTableSize; i++)
            *st++ = NULL;

        symbolTable_inited = true;
    }
}

void freeSymbolTable() {
    if(symbolTable_inited) {
        //  Free each st_node
        st_node * p;
        for(int i = 0; i < HASH_MAP_SIZE; i++) {
            p = hashMap[i];
            while(p != NULL) {
                st_node * temp = p->next;
                free(p);
                hashMap[i] = NULL;
                p = temp;
            }
        }
        
        free(hashMap);
        hashMap = NULL;

        //  Free each symbol
        for(int i = 0; i < symbolTableLength; i++) {
            free(symbolTable[i]);
            symbolTable[i] = NULL;
        }

        free(symbolTable);
        symbolTable = NULL;

        symbolTableSize = 16;

        symbolTable_inited = false;
    }
}
