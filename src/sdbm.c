/* Joshua Baker		jkb150030
 * Dr. Gupta
 * CS 4386.001
 * 
 * sdbm.c
 * 
 * This file implements the sdbm hash function for use in the hash table
 * (see symbolTable.h and symbolTable.c)
 */

#include "sdbm.h"

/*
 *  http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned long hash(const unsigned char * str) {
    unsigned long hash = 0;
    int c;

    while((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}
