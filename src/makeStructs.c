#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "structs.h"
#include "makeStructs.h"
#include "symbolTable.h"

program * makeProgram(declarationSeq * decls, statementSeq * stmts) {
    program * p = (program *) malloc(sizeof(program));

    p->decls = decls;
    p->stmts = stmts;

    return p;
}

declarationSeq * makeDeclarationSeq() {
    declarationSeq * d = (declarationSeq *) malloc(sizeof(declarationSeq));

    d->count = 0;
    d->size = 1;
    d->decls = (declaration **) malloc(sizeof(declaration *));
}
declarationSeq * addDeclaration(declarationSeq * seq, declaration * d) {
    if(d != NULL) {
        if(seq->size == seq->count) {
            seq->size *= 2;
            seq->decls = realloc(seq->decls, sizeof(declaration *) * seq->size);
        }

        seq->decls[seq->count++] = d;
    }

    return seq;
}

declaration * makeDeclaration(char * ident, varType_e type) {
    declaration * d = (declaration *) malloc(sizeof(declaration));

    int varHandle = addSymbol(ident, type);

    if(varHandle == -1) {
        printf("Error: \"%s\" declared more than once.\n\n", ident);
        exit(1);
    }
    
    d->varHandle = varHandle;

    free(ident);

    return d;
}

statementSeq * makeStatementSeq() {
    statementSeq * s = (statementSeq *) malloc(sizeof(statementSeq));

    s->count = 0;
    s->size = 1;
    s->stmts = (statement **) malloc(sizeof(statement *));

    return s;
}
statementSeq * addStatement(statementSeq * seq, statement * s) {
    if(s != NULL) {
        if(seq->size == seq->count) {
            seq->size *= 2;
            seq->stmts = realloc(seq->stmts, sizeof(statement *) * seq->size); 
        }

        seq->stmts[seq->count++] = s;
    }

    return seq;
}

statement * makeStatementFromAssign(assignment * a) {
    statement * s = (statement *) malloc(sizeof(statement));

    s->type = ASSIGN_STMT;
    s->u.asgnStmt = a;

    return s;
}
statement * makeStatementFromIf(ifStatement * i) {
    statement * s = (statement *) malloc(sizeof(statement));

    s->type = IF_STMT;
    s->u.ifStmt = i;

    return s;
}
statement * makeStatementFromWhile(whileStatement * w) {
    statement * s = (statement *) malloc(sizeof(statement));

    s->type = WHILE_STMT;
    s->u.whileStmt = w;

    return s;
}
statement * makeStatementFromWriteInt(writeInt * w) {
    statement * s = (statement *) malloc(sizeof(statement));

    s->type = WRITEINT_STMT;
    s->u.writeIntStmt = w;

    return s;
}

assignment * makeAssignmentFromReadInt(char * ident) {
    assignment * a = (assignment *) malloc(sizeof(assignment));

    int varHandle = getHandle(ident);

    if(varHandle == -1) {
        printf("Error: \"%s\" was not declared.\n\n", ident);
        exit(1);
    }

    symbol * s = getSymbol(varHandle);

    if(s->type != INT_t) {
        printf("Error: \"%s\" was not declared as an int.\n\n", ident);
        exit(1);
    }

    a->type = ASSIGN_READINT;
    a->varHandle = varHandle;
    a->expr = NULL;

    free(ident);

    return a;
}
assignment * makeAssignmentFromExpr(char * ident, expression * e) {
    assignment * a = (assignment *) malloc(sizeof(assignment));

    int varHandle = getHandle(ident);

    if(varHandle == -1) {
        printf("Error: \"%s\" was not declared.\n\n", ident);
        exit(1);
    }

    symbol * s = getSymbol(varHandle);

    if(s->type != e->type) {
        printf("Error: \"%s\" has type %s, but the right hand side has type %s.\n\n", ident, getVarType(s->type), getVarType(e->type));
        exit(1);
    }

    a->type = ASSIGN_EXPR;
    a->varHandle = varHandle;
    a->expr = e;

    free(ident);

    return a;
}

ifStatement * makeIfStatement(expression * condition, statementSeq * ifTrue, statementSeq * ifFalse) {
    ifStatement * i = (ifStatement *) malloc(sizeof(ifStatement));

    if(condition->type != BOOL_t) {
        printf("Error: The expression does not evaluate to boolean.\n\n");
        exit(1);
    }

    i->hasElse = (ifFalse != NULL);
    i->condition = condition;
    i->ifTrue = ifTrue;
    i->ifFalse = ifFalse;

    return i;
}

whileStatement * makeWhileStatement(expression * condition, statementSeq * whileTrue) {
    whileStatement * w = (whileStatement *) malloc(sizeof(whileStatement));

    if(condition->type != BOOL_t) {
        printf("Error: The expression does not evaluate to boolean.\n\n");
        exit(1);
    }

    w->condition = condition;
    w->whileTrue = whileTrue;

    return w;
}

writeInt * makeWriteInt(expression * e) {
    writeInt * w = (writeInt *) malloc(sizeof(writeInt));

    w->expr = e;
}

expression * makeExpressionFromSimple(simpleExpression * s) {
    expression * e = (expression *) malloc(sizeof(expression));

    e->type = s->type;

    e->operands = 1;
    e->left = s;
    e->right = NULL;

    return e;
}
expression * makeExpressionFromOp(simpleExpression * left, OP4_e op, simpleExpression * right) {
    expression * e = (expression *) malloc(sizeof(expression));

    if(left->type != INT_t) {
        printf("Error: Left side of %s operation is not an int expression.\n\n", getOp4Str(op));
        exit(1);
    }

    if(right->type != INT_t) {
        printf("Error: Right side of %s operation is not an int expression.\n\n", getOp4Str(op));
        exit(1);
    }

    e->type = BOOL_t;

    e->operands = 2;
    e->left = left;
    e->op = op;
    e->right = right;

    return e;
}

simpleExpression * makeSimpleExpressionFromTerm(term * t) {
    simpleExpression * s = (simpleExpression *) malloc(sizeof(simpleExpression));

    s->type = t->type;

    s->operands = 1;
    s->left = t;
    s->right = NULL;

    return s;
}
simpleExpression * makeSimpleExpressionFromOp(term * left, OP3_e op, term * right) {
    simpleExpression * s = (simpleExpression *) malloc(sizeof(simpleExpression));

    if(left->type != INT_t) {
        printf("Error: Left side of %s operation is not an int expression.\n\n", getOp3Str(op));
        exit(1);
    }

    if(right->type != INT_t) {
        printf("Error: Right side of %s operation is not an int expression.\n\n", getOp3Str(op));
        exit(1);
    }

    s->type = INT_t;

    s->operands = 2;
    s->left = left;
    s->op = op;
    s->right = right;

    return s;
}

term * makeTermFromFactor(factor * f) {
    term * t = (term *) malloc(sizeof(term));

    t->type = f->type;

    t->operands = 1;
    t->left = f;
    t->right = NULL;

    return t;
}
term * makeTermFromOp(factor * left, OP2_e op, factor * right) {
    term * t = (term *) malloc(sizeof(term));

    if(left->type != INT_t) {
        printf("Error: Left side of %s operation is not an int expression.\n\n", getOp2Str(op));
        exit(1);
    }

    if(right->type != INT_t) {
        printf("Error: Right side of %s operation is not an int expression.\n\n", getOp2Str(op));
        exit(1);
    }

    t->type = INT_t;

    t->operands = 2;
    t->left = left;
    t->op = op;
    t->right = right;

    return t;
}

factor * makeFactorFromIdent(char * ident) {
    factor * f = (factor *) malloc(sizeof(factor));

    int varHandle = getHandle(ident);

    if(varHandle == -1) {
        printf("Error: \"%s\" was not declared.\n\n", ident);
        exit(1);
    }

    symbol * s = getSymbol(varHandle);

    f->type = s->type;
    f->factorType = FACTOR_VAR;
//    f->type = FACTOR_VAR;
    f->u.varHandle = varHandle;

    free(ident);

    return f;
}
factor * makeFactorFromNum(int n) {
    factor * f = (factor *) malloc(sizeof(factor));

    f->type = INT_t;
    f->factorType = FACTOR_NUM;
//    f->type = FACTOR_NUM
    f->u.num = n;

    return f;
}
factor * makeFactorFromBool(bool b) {
    factor * f = (factor *) malloc(sizeof(factor));

    f->type = BOOL_t;
    f->factorType = FACTOR_BOOL;
//    f->type = FACTOR_BOOL;
    f->u.boollit = b;

    return f;
}
factor * makeFactorFromExpr(expression * e) {
    factor * f = (factor *) malloc(sizeof(factor));

    f->type = e->type;
    f->factorType = FACTOR_EXPR;
//    f->type = FACTOR_EXPR;
    f->u.expr = e;
}
