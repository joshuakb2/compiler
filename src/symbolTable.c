/* Joshua Baker		jkb150030
 * Dr. Gupta
 * CS 4386.001
 * 
 * symbolTable.c
 * 
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

#define __STDC_WANT_LIB_EXT2__ 1
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "sdbm.h"
#include "structTypes.h"
#include "symbolTable.h"

bool symbolTable_inited = false;

//  Chosen as a prime number near 32.
const unsigned int HASH_MAP_SIZE = 31;

st_node ** hashMap;

int symbolTableSize = 16;
int symbolTableLength = 0;

//	Pointer to symbol table
symbol ** symbolTable;

//	Returns pointer to symbol given its index in the symbol table.
//	Returns NULL if the given index is invalid.
symbol * getSymbol(int i) {
    if(!symbolTable_inited)
        return NULL;

    if(i < 0 || i >= symbolTableLength)
        return NULL;

    return symbolTable[i];
}

//	Returns the index of the given variable in the symbol table.
//	If the given variable is not present in the table, returns -1.
int getHandle(const char * const name) {
    int hashVal = hash(name) % HASH_MAP_SIZE;

    st_node * n = hashMap[hashVal];

	//	Search through linked list for the variable
    while(n != NULL) {
		//	If this symbol is the one we're looking for
        if(strcmp(symbolTable[n->index]->key, name) == 0)
			//	return its index
            return n->index;
        n = n->next;
    }

	//	Didn't find it
    return -1;
}

//	Adds a variable to the symbol table.
//	If that variable was already present, returns -1.
int addSymbol(const char * const name, varType_e type) {
    int hashVal = hash(name) % HASH_MAP_SIZE;

	//	Points to a pointer in the hash table (head of linked list)
    st_node ** p = hashMap + hashVal;

	//	If we have a collision, get to the end of the linked list
    while(*p != NULL) {
        //  If name is already in the symbol table
        if(strcmp(symbolTable[(*p)->index]->key, name) == 0)
            return -1;
		//	Point p to the pointer to the next node.
        p = &((*p)->next);
    }

    //  p points to the pointer to the next node

    //  Grow the symbol table if necessary
    if(symbolTableSize == symbolTableLength) {
        int newSize = symbolTableSize * 2;
        symbolTable = (symbol **) realloc(symbolTable, sizeof(symbol *) * symbolTableSize);
		
		//	Fill the new pointers with NULL.
		for(int i = symbolTableSize; i < newSize; i++)
			symbolTable[i] = NULL;
		
		symbolTableSize = newSize;
    }

	//	index that will serve as the handle for this symbol
    int index = symbolTableLength++;

	//	Create and populate the new node in the hash table.
    (*p) = (st_node *) malloc(sizeof(st_node));
    (*p)->index = index;
    (*p)->next = NULL;

	//	Add the symbol to the symbol table.
    symbolTable[index] = (symbol *) malloc(sizeof(symbol));
    symbolTable[index]->key = (char *) malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(symbolTable[index]->key, name);

    symbolTable[index]->type = type;

	//	Return the symbol's handle.
    return index;
}

//	Create the hash table and symbol table.
void initSymbolTable() {
	//	Only if they haven't been made yet
    if(!symbolTable_inited) {
		//	Allocate hash table
        hashMap = (st_node **) malloc(sizeof(st_node *) * HASH_MAP_SIZE);

		//	Fill it with NULLs
        for(int i = 0; i < HASH_MAP_SIZE; i++)
            hashMap[i] = NULL;

		//	Allocate symbol table
        symbolTable = (symbol **) malloc(sizeof(symbol *) * symbolTableSize);

		//	Fill it with NULLs.
        for(int i = 0; i < symbolTableSize; i++)
            symbolTable[i] = NULL;

		//	Record that we built the tables.
        symbolTable_inited = true;
    }
}

//	Free the hash table and symbol table.
void freeSymbolTable() {
	//	Only if they've been allocated already
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

		//	Return symbolTableSize to default.
        symbolTableSize = 16;

		//	Record that we freed the tables.
        symbolTable_inited = false;
    }
}
