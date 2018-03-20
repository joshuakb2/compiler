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

void compile(program * p) {
    printHeader();
    printf("\n");
    declareVars(p->decls, 1);
    printf("\n");
    printStatements(p->stmts, 1);
    printf("\n");
    printFooter();

    free(p);
}

void printHeader() {
    printf("%s", "#include <stdlib.h>\n"
                 "#include <stdio.h>\n"
                 "#include <stdbool.h>\n"
                 "#include <string.h>\n"
                 "\n"
                 "char * buffer;\n"
                 "size_t size;"
                 "\n"
                 "int readInt() {\n"
                   "\tprintf(\"%s\", \"Please type an integer: \");\n"
                   "\tgetline(&buffer, &size, stdin);\n"
                   "\treturn atoi(buffer);\n"
                 "}\n"
                 "\n"
                 "int main() {\n"
                   "\tsize = 30;\n"
                   "\tbuffer = (char *) malloc(sizeof(char) * size);\n"
    );
}

void printFooter() {
    printf("%s",   "\tfree(buffer);\n"
                   "\n"
                   "\treturn 0;\n}");
}

void declareVars(declarationSeq * decls, int tabs) {
    //  Declarations are in reverse order. Not that it matters :P
    for(int i = decls->count - 1; i >= 0; i--) {
        symbol * s = getSymbol(decls->decls[i]->varHandle);

        printTabs(tabs);
        printf("%s %s = %s;\n", s->type == INT_t ? "int" : "bool", s->key, s->type == INT_t ? "0" : "false");

        free(decls->decls[i]);
    }

    free(decls->decls);
    free(decls);
}

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

void printAssignment(assignment * a, int tabs) {
    switch(a->type) {
        case ASSIGN_EXPR:
            printTabs(tabs);
            printf("%s = ", getSymbol(a->varHandle)->key);
            printExpression(a->expr);
            printf(";\n");
            break;
        case ASSIGN_READINT:
            printTabs(tabs);
            printf("%s = readInt();\n", getSymbol(a->varHandle)->key);
            break;
    }

    free(a);
}

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

void printWriteInt(writeInt * w, int tabs) {
    printTabs(tabs);
    printf("printf(\"%%d\\n\", ");
    printExpression(w->expr);
    printf(");\n");

    free(w);
}

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

void printFactor(factor * f) {
    switch(f->factorType) {
        case FACTOR_VAR:
            printf("%s", getSymbol(f->u.varHandle)->key);
            break;
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
