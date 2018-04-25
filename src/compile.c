/* Joshua Baker		jkb150030
 * Dr. Gupta
 * CS 4386.001
 * 
 * compile.c
 * 
 * This file converts parse tree nodes to C code and prints that code
 * out onto the standard output.
 * As soon as the code has been printed for any particular node, that
 * node's memory is freed.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "utility.h"
#include "structs.h"
#include "structTypes.h"
#include "ops.h"
#include "symbolTable.h"
#include "compile.h"

const char VAR_PREFIX[] = "tl13_";
const int VAR_PREFIX_LENGTH = 5;

void compile(program * p) {
    printHeader();
    declareVars(p->decls, 1);
    printf("\n");
    printStatements(p->stmts, 1);
    printFooter();

    free(p);
}

void printHeader() {
    printf("%s", "#include <stdlib.h>\n"
                 "#include <stdio.h>\n"
                 "#include <stdbool.h>\n"
                 "\n"
                 "int readInt() {\n"
                   "\tprintf(\"%s\", \"Please type an integer: \");\n"
                   "\tchar * s = NULL;\n"
                   "\tsize_t length;\n"
                   "\tgetline(&s, &length, stdin);\n"
                   "\tint n = atoi(s);\n"
                   "\tfree(s);\n"
                   "\treturn n;\n"
                 "}\n"
                 "\n"
                 "int main() {\n"
    );
}

void printFooter() {
    printf("%s", "\n\treturn 0;\n}");
}

//	Print all the variable declarations.
void declareVars(declarationSeq * decls, int tabs) {
    //  Declarations are in reverse order. Not that it matters :P
    for(int i = decls->count - 1; i >= 0; i--) {
        symbol * s = getSymbol(decls->decls[i]->varHandle);

        bool isInt = s->type == INT_t;
        char * name = getVarName(s);

        printTabs(tabs);
        printf("%s %s = %s;\n",
				isInt ? "int" : "bool",	//	type
				name,					//	name
				isInt ? "0" : "false"	//	initial value
		);

        free(name);
        free(decls->decls[i]);
    }

    free(decls->decls);
    free(decls);
}

//	Print all the statements
//	Keeping track of tabs makes it prettier
void printStatements(statementSeq * stmts, int tabs) {
    //  Statements are in reverse order. This part actually really does matter D:
    for(int i = stmts->count - 1; i >= 0; i--) {
        statement * s = stmts->stmts[i];
        switch(s->type) {
            case ASSIGN_STMT:
                printAssignment(s->u.asgnStmt, tabs);
                break;
            case IF_STMT:
                printIf(s->u.ifStmt, tabs);
                break;
            case WHILE_STMT:
                printWhile(s->u.whileStmt, tabs);
                break;
            case WRITEINT_STMT:
                printWriteInt(s->u.writeIntStmt, tabs);
                break;
        }

        free(s);
    }

    free(stmts->stmts);
    free(stmts);
}

//	Print an assignment statement
void printAssignment(assignment * a, int tabs) {
    char * varName = getVarName(getSymbol(a->varHandle));

    switch(a->type) {
        case ASSIGN_EXPR:
            printTabs(tabs);
            printf("%s = ", varName);
            printExpression(a->expr);
            printf(";\n");
            break;
        case ASSIGN_READINT:
            printTabs(tabs);
			//	See printHeader() for definition of readInt()
            printf("%s = readInt();\n", varName);
            break;
    }

    free(varName);
    free(a);
}

//	Print an if statement (and else clause if present)
void printIf(ifStatement * i, int tabs) {
    printTabs(tabs);
    printf("if (");
    printExpression(i->condition);
    printf(") {\n");
    printStatements(i->ifTrue, tabs + 1);
    printTabs(tabs);
    printf("}\n");
    if(i->hasElse) {
        printTabs(tabs);
        printf("else {");
        printStatements(i->ifFalse, tabs + 1);
        printTabs(tabs);
        printf("}\n");
    }

    free(i);
}

//	Print a while loop
void printWhile(whileStatement * w, int tabs) {
    printTabs(tabs);
    printf("while (");
    printExpression(w->condition);
    printf(") {\n");
    printStatements(w->whileTrue, tabs + 1);
    printTabs(tabs);
    printf("}\n");
    
    free(w);
}

//	Print a writeInt statement
void printWriteInt(writeInt * w, int tabs) {
    printTabs(tabs);
	//	Expression is known to evaluate to int.
    printf("%s", "printf(\"%d\\n\", ");
    printExpression(w->expr);
    printf("%s", ");\n");

    free(w);
}

//	Print an expression
void printExpression(expression * e) {
    switch(e->operands) {
        case 1:
            printSimpleExpression(e->left);
            break;
        case 2:
            printSimpleExpression(e->left);
            printf(" %s ", getOp4StrC(e->op));
            printSimpleExpression(e->right);
            break;
    }

    free(e);
}

//	Print a simple expression
void printSimpleExpression(simpleExpression * e) {
    switch(e->operands) {
        case 1:
            printTerm(e->left);
            break;
        case 2:
            printTerm(e->left);
            printf(" %s ", getOp3StrC(e->op));
            printTerm(e->right);
            break;
    }

    free(e);
}

//	Print a term
void printTerm(term * t) {
    switch(t->operands) {
        case 1:
            printFactor(t->left);
            break;
        case 2:
            printFactor(t->left);
            printf(" %s ", getOp2StrC(t->op));
            printFactor(t->right);
            break;
    }

    free(t);
}

//	Print a factor
void printFactor(factor * f) {
    switch(f->factorType) {
        case FACTOR_VAR: {
            char * varName = getVarName(getSymbol(f->u.varHandle));
            printf("%s", varName);
            free(varName);
        }   break;
        case FACTOR_NUM:
            printf("%d", f->u.num);
            break;
        case FACTOR_BOOL:
            printf("%s", f->u.boollit ? "true" : "false");
            break;
        case FACTOR_EXPR:
            printf("(");
            printExpression(f->u.expr);
            printf(")");
            break;
    }

    free(f);
}

//  Adds the prefix to the symbol name
char * getVarName(symbol * s) {
    char * name = (char *) malloc(sizeof(char) * (strlen(s->key) + 1 + VAR_PREFIX_LENGTH));

    strcpy(name, VAR_PREFIX);
    strcpy(name + VAR_PREFIX_LENGTH, s->key);

    return name;
}
