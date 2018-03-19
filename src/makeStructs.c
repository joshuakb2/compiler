#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "structs.h"
#include "makeStructs.h"

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

declaration * makeDeclaration(char * ident, enum varType type) {
    declaration * d = (declaration *) malloc(sizeof(declaration));

    /*
    d->ident = (char *) malloc(sizeof(char) * (1 + strlen(ident)));
    strcpy(d->ident, ident);
    d->type = type;
    */
    d->ident = ident;

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

    a->type = ASSIGN_READINT;
    /*
    a->ident = (char *) malloc(sizeof(char) * (1 + strlen(ident)));
    strcpy(a->ident, ident);
    */
    a->ident = ident;
    a->expr = NULL;

    return a;
}
assignment * makeAssignmentFromExpr(char * ident, expression * e) {
    assignment * a = (assignment *) malloc(sizeof(assignment));

    a->type = ASSIGN_EXPR;
    /*
    a->ident = (char *) malloc(sizeof(char) * (1 + strlen(ident)));
    strcpy(a->ident, ident);
    */
    a->ident = ident;
    a->expr = e;

    return a;
}

ifStatement * makeIfStatement(expression * condition, statementSeq * ifTrue, statementSeq * ifFalse) {
    ifStatement * i = (ifStatement *) malloc(sizeof(ifStatement));

    i->hasElse = (ifFalse != NULL);
    i->condition = condition;
    i->ifTrue = ifTrue;
    i->ifFalse = ifFalse;

    return i;
}

whileStatement * makeWhileStatement(expression * condition, statementSeq * whileTrue) {
    whileStatement * w = (whileStatement *) malloc(sizeof(whileStatement));

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

    e->operands = 1;
    e->left = s;
    e->right = NULL;

    return e;
}
expression * makeExpressionFromOp(simpleExpression * left, enum OP4_v op, simpleExpression * right) {
    expression * e = (expression *) malloc(sizeof(expression));

    e->operands = 2;
    e->left = left;
    e->op = op;
    e->right = right;

    return e;
}

simpleExpression * makeSimpleExpressionFromTerm(term * t) {
    simpleExpression * s = (simpleExpression *) malloc(sizeof(simpleExpression));

    s->operands = 1;
    s->left = t;
    s->right = NULL;

    return s;
}
simpleExpression * makeSimpleExpressionFromOp(term * left, enum OP3_v op, term * right) {
    simpleExpression * s = (simpleExpression *) malloc(sizeof(simpleExpression));

    s->operands = 2;
    s->left = left;
    s->op = op;
    s->right = right;

    return s;
}

term * makeTermFromFactor(factor * f) {
    term * t = (term *) malloc(sizeof(term));

    t->operands = 1;
    t->left = f;
    t->right = NULL;

    return t;
}
term * makeTermFromOp(factor * left, enum OP2_v op, factor * right) {
    term * t = (term *) malloc(sizeof(term));

    t->operands = 2;
    t->left = left;
    t->op = op;
    t->right = right;

    return t;
}

factor * makeFactorFromIdent(char * ident) {
    factor * f = (factor *) malloc(sizeof(factor));

    f->type = FACTOR_VAR;
    /*
    f->u.ident = (char *) malloc(sizeof(char) * (1 + strlen(ident)));
    strcpy(f->u.ident, ident);
    */
    f->u.ident = ident;

    return f;
}
factor * makeFactorFromNum(int n) {
    factor * f = (factor *) malloc(sizeof(factor));

    f->type = FACTOR_NUM;
    f->u.num = n;

    return f;
}
factor * makeFactorFromBool(bool b) {
    factor * f = (factor *) malloc(sizeof(factor));

    f->type = FACTOR_BOOL;
    f->u.boollit = b;

    return f;
}
factor * makeFactorFromExpr(expression * e) {
    factor * f = (factor *) malloc(sizeof(factor));

    f->type = FACTOR_EXPR;
    f->u.expr = e;
}
