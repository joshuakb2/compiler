#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ops.h"

int getOp2(const char * const s) {
	if(s[0] == '*')
		return TIMES;
	else if(s[0] == 'd')
		return DIV;
	else
		return MOD;
}

int getOp3(const char * const s) {
	if(s[0] == '+')
		return PLUS;
	else
		return MINUS;
}

int getOp4(const char * const s) {
	int length = strlen(s);
	if(length == 1) {
		if(s[0] == '=')
			return EQ;
		else if(s[0] == '<')
			return LT;
		else
			return GT;
	}
	else {
		if(s[0] == '!')
			return NE;
		else if(s[0] == '<')
			return LE;
		else
			return GE;
	}
}

char * getOp2Str(enum OP2_v v) {
    switch(v) {
        case TIMES:
            return "*";
        case DIV:
            return "/";
        case MOD:
            return "%";
        default:
            printf("THIS SHOULD NEVER HAPPEN!!!\n");
            return NULL;
    }
}

char * getOp3Str(enum OP3_v v) {
    switch(v) {
        case PLUS:
            return "+";
        case MINUS:
            return "-";
        default:
            printf("THIS SHOULD NEVER HAPPEN!!!\n");
            return NULL;
    }
}

char * getOp4Str(enum OP4_v v) {
    switch(v) {
        case EQ:
            return "=";
        case NE:
            return "!=";
        case LT:
            return "<";
        case LE:
            return "<=";
        case GT:
            return ">";
        case GE:
            return ">=";
        default:
            printf("THIS SHOULD NEVER HAPPEN!!!\n");
            return NULL;
    }
}