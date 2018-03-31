/* Joshua Baker		jkb150030
 * Dr. Gupta
 * CS 4386.001
 * 
 * structTypes.c
 * 
 * This file implements functions which convert enum values to more
 * human-friendly strings. Used when printing the parse tree.
 */

#include <stdlib.h>
#include <stdio.h>

#include "structTypes.h"

char * getVarType(varType_e v) {
    switch(v) {
        case INT_t:
            return "INT";
        case BOOL_t:
            return "BOOL";
        default:
            printf("THIS SHOULD NEVER HAPPEN!!!\n");
            return NULL;
    }
}

char * getStatementType(statementType_e s) {
    switch(s) {
        case ASSIGN_STMT:
            return "ASSIGN_STMT";
        case IF_STMT:
            return "IF_STMT";
        case WHILE_STMT:
            return "WHILE_STMT";
        case WRITEINT_STMT:
            return "WRITEINT_STMT";
        default:
            printf("THIS SHOULD NEVER HAPPEN!!!\n");
            return NULL;
    }
}

char * getAssignmentType(assignmentType_e a) {
    switch(a) {
        case ASSIGN_EXPR:
            return "ASSIGN_EXPR";
        case ASSIGN_READINT:
            return "ASSIGN_READINT";
        default:
            printf("THIS SHOULD NEVER HAPPEN!!!\n");
            return NULL;
    }
}

char * getFactorType(factorType_e f) {
    switch(f) {
        case FACTOR_VAR:
            return "FACTOR_VAR";
        case FACTOR_NUM:
            return "FACTOR_NUM";
        case FACTOR_BOOL:
            return "FACTOR_BOOL";
        case FACTOR_EXPR:
            return "FACTOR_EXPR";
        default:
            printf("THIS SHOULD NEVER HAPPEN!!!\n");
            return NULL;
    }
}
