#include <stdlib.h>
#include <stdio.h>

#include "structTypes.h"

char * getVarType(enum varType v) {
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

char * getStatementType(enum statementType s) {
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

char * getAssignmentType(enum assignmentType a) {
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

char * getFactorType(enum factorType f) {
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
