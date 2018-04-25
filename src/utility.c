/* Joshua Baker         jkb150030
 * Dr. Gupta
 * CS 4386.001
 *
 * utility.c
 *
 * This file provides functions that help with pretty-printing
 * the output C code or the parse tree.
 */

#include <stdio.h>

#include "utility.h"

void printLeadLines(int tabs) {
    for(int i = 0; i < tabs - 1; i++) {
        printf("|\t");
    }
}

void printLines(int tabs) {
    printLeadLines(tabs);
    if(tabs > 0)
        printf("|------ ");
}

void printTabs(int tabs) {
    for(int i = 0; i < tabs; i++)
        printf("\t");
}
