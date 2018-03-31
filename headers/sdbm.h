/* Joshua Baker		jkb150030
 * Dr. Gupta
 * CS 4386.001
 * 
 * sdbm.h
 * 
 * This header file contains a forward declaration for the hash function
 * used by my hash table implementation.
 *
 * The hash function I am using was taken from http://www.cse.yorku.ca/~oz/hash.html
 */

#ifndef sdbm_h
#define sdbm_h

//	Given a string, returns a hash.
unsigned long hash(const unsigned char *);

#endif
