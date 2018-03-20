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
