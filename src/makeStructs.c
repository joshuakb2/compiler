/* Joshua Baker		jkb150030
 * Dr. Gupta
 * CS 4386.001
 * 
 * makeStructs.c
 * 
 * This file implements the functions which contain all the logic
 * for building new parse tree node structures.
 */

#define __STDC_WANT_LIB_EXT2__ 1
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "structs.h"
#include "makeStructs.h"
#include "symbolTable.h"

program * makeProgram(declarationSeq * decls, statementSeq * stmts) {
	//	Allocate memory for the new node
    program * p = (program *) malloc(sizeof(program));

    p->decls = decls;
    p->stmts = stmts;

    return p;
}

declarationSeq * makeDeclarationSeq() {
	//	Allocate memory for the new node
    declarationSeq * d = (declarationSeq *) malloc(sizeof(declarationSeq));

    d->count = 0;
    d->size = 1;
	//	Array of declarations in reverse order
    d->decls = (declaration **) malloc(sizeof(declaration *));

    return d;
}
declarationSeq * addDeclaration(declarationSeq * seq, declaration * d) {
    if(d != NULL) {
		//	Grow the array of declarations if necessary
        if(seq->size == seq->count) {
            seq->size *= 2;
            seq->decls = realloc(seq->decls, sizeof(declaration *) * seq->size);
        }

        seq->decls[seq->count++] = d;
    }

    return seq;
}

declaration * makeDeclaration(char * ident, varType_e type) {
	//	Allocate memory for the new node
    declaration * d = (declaration *) malloc(sizeof(declaration));

	//	Add the variable to the symbol table
    int varHandle = addSymbol(ident, type);

	//	If it was already there
    if(varHandle == -1) {
        printf("Error: Variable \"%s\" was declared more than once.\n\n", ident);
        exit(1);
    }
    
    d->varHandle = varHandle;

	//	The symbol table has a copy of the string. We can free this one.
    free(ident);

    return d;
}

statementSeq * makeStatementSeq() {
	//	Allocate memory for the new node
    statementSeq * s = (statementSeq *) malloc(sizeof(statementSeq));

    s->count = 0;
    s->size = 1;
	//	Array of statements in reverse order
    s->stmts = (statement **) malloc(sizeof(statement *));

    return s;
}
statementSeq * addStatement(statementSeq * seq, statement * s) {
    if(s != NULL) {
		//	Grow the array of statements if necessary
        if(seq->size == seq->count) {
            seq->size *= 2;
            seq->stmts = realloc(seq->stmts, sizeof(statement *) * seq->size); 
        }

        seq->stmts[seq->count++] = s;
    }

    return seq;
}

statement * makeStatementFromAssign(assignment * a) {
	//	Allocate memory for the new node
    statement * s = (statement *) malloc(sizeof(statement));

    s->type = ASSIGN_STMT;
    s->u.asgnStmt = a;

    return s;
}
statement * makeStatementFromIf(ifStatement * i) {
	//	Allocate memory for the new node
    statement * s = (statement *) malloc(sizeof(statement));

    s->type = IF_STMT;
    s->u.ifStmt = i;

    return s;
}
statement * makeStatementFromWhile(whileStatement * w) {
	//	Allocate memory for the new node
    statement * s = (statement *) malloc(sizeof(statement));

    s->type = WHILE_STMT;
    s->u.whileStmt = w;

    return s;
}
statement * makeStatementFromWriteInt(writeInt * w) {
	//	Allocate memory for the new node
    statement * s = (statement *) malloc(sizeof(statement));

    s->type = WRITEINT_STMT;
    s->u.writeIntStmt = w;

    return s;
}

assignment * makeAssignmentFromReadInt(char * ident) {
	//	Allocate memory for the new node
    assignment * a = (assignment *) malloc(sizeof(assignment));

	//	Make sure this variable was declared
    int varHandle = getHandle(ident);

	//	If not
    if(varHandle == -1) {
        printf("Error: Variable \"%s\" was not declared.\n\n", ident);
        exit(1);
    }

	//	Make sure this variable is the right type
    symbol * s = getSymbol(varHandle);

	//	If not
    if(s->type != INT_t) {
        printf("Error: Variable \"%s\" was used as an int but not declared as such.\n\n", ident);
        exit(1);
    }

    a->type = ASSIGN_READINT;
    a->varHandle = varHandle;
	//	This is a readInt assignment, so no expression
    a->expr = NULL;

	//	Done with the variable name
    free(ident);

    return a;
}
assignment * makeAssignmentFromExpr(char * ident, expression * e) {
	//	Allocate memory for the new node
    assignment * a = (assignment *) malloc(sizeof(assignment));

	//	Make sure this variable was declared
    int varHandle = getHandle(ident);

	//	If not
    if(varHandle == -1) {
        printf("Error: Variable \"%s\" was not declared.\n\n", ident);
        exit(1);
    }

	//	Make sure this variable is the right type
    symbol * s = getSymbol(varHandle);

	//	If not
    if(s->type != e->type) {
        printf("Error: \"%s\" has type %s, but the right hand side has type %s.\n\n", ident, getVarType(s->type), getVarType(e->type));
        exit(1);
    }

    a->type = ASSIGN_EXPR;
    a->varHandle = varHandle;
    a->expr = e;

	//	Done with the variable name
    free(ident);

    return a;
}

ifStatement * makeIfStatement(expression * condition, statementSeq * ifTrue, statementSeq * ifFalse) {
	//	Allocate memory for the new node
    ifStatement * i = (ifStatement *) malloc(sizeof(ifStatement));

	//	If the expression doesn't evaluate to bool, it can't be a condition.
    if(condition->type != BOOL_t) {
        printf("Error: The if statement's condition does not evaluate to boolean.\n\n");
        exit(1);
    }

    i->hasElse = (ifFalse != NULL);
    i->condition = condition;
    i->ifTrue = ifTrue;
    i->ifFalse = ifFalse;

    return i;
}

whileStatement * makeWhileStatement(expression * condition, statementSeq * whileTrue) {
	//	Allocate memory for the new node
    whileStatement * w = (whileStatement *) malloc(sizeof(whileStatement));

	//	If the expression doesn't evaluate to bool, it can't be a condition.
    if(condition->type != BOOL_t) {
        printf("Error: The while statement's condition does not evaluate to boolean.\n\n");
        exit(1);
    }

    w->condition = condition;
    w->whileTrue = whileTrue;

    return w;
}

writeInt * makeWriteInt(expression * e) {
	//	Allocate memory for the new node
    writeInt * w = (writeInt *) malloc(sizeof(writeInt));

    w->expr = e;

    return w;
}

expression * makeExpressionFromSimple(simpleExpression * s) {
	//	Allocate memory for the new node
    expression * e = (expression *) malloc(sizeof(expression));

	//	The type of the expression is the same as the type of the simpleExpression
    e->type = s->type;

    e->operands = 1;
    e->left = s;
    e->right = NULL;

    return e;
}
expression * makeExpressionFromOp(simpleExpression * left, OP4_e op, simpleExpression * right) {
	//	Allocate memory for the new node
    expression * e = (expression *) malloc(sizeof(expression));

	//	Both operands must be ints
    if(left->type != INT_t) {
        printf("Error: Left side of %s operation is not an int expression.\n\n", getOp4Str(op));
        exit(1);
    }
    if(right->type != INT_t) {
        printf("Error: Right side of %s operation is not an int expression.\n\n", getOp4Str(op));
        exit(1);
    }

	//	OP4 always produces a bool result
    e->type = BOOL_t;

    e->operands = 2;
    e->left = left;
    e->op = op;
    e->right = right;

    return e;
}

simpleExpression * makeSimpleExpressionFromTerm(term * t) {
	//	Allocate memory for the new node
    simpleExpression * s = (simpleExpression *) malloc(sizeof(simpleExpression));

	//	The type of the simpleExpression is the same as the type of the term
    s->type = t->type;

    s->operands = 1;
    s->left = t;
    s->right = NULL;

    return s;
}
simpleExpression * makeSimpleExpressionFromOp(term * left, OP3_e op, term * right) {
	//	Allocate memory for the new node
    simpleExpression * s = (simpleExpression *) malloc(sizeof(simpleExpression));

	//	Both operands must be ints
    if(left->type != INT_t) {
        printf("Error: Left side of %s operation is not an int expression.\n\n", getOp3Str(op));
        exit(1);
    }
    if(right->type != INT_t) {
        printf("Error: Right side of %s operation is not an int expression.\n\n", getOp3Str(op));
        exit(1);
    }

	//	OP3 always produces an int result
    s->type = INT_t;

    s->operands = 2;
    s->left = left;
    s->op = op;
    s->right = right;

    return s;
}

term * makeTermFromFactor(factor * f) {
	//	Allocate memory for the new node
    term * t = (term *) malloc(sizeof(term));

	//	The type of the term is the same as the type of the factor.
    t->type = f->type;

    t->operands = 1;
    t->left = f;
    t->right = NULL;

    return t;
}
term * makeTermFromOp(factor * left, OP2_e op, factor * right) {
	//	Allocate memory for the new node
    term * t = (term *) malloc(sizeof(term));

	//	Both operands must be ints.
    if(left->type != INT_t) {
        printf("Error: Left side of %s operation is not an int expression.\n\n", getOp2Str(op));
        exit(1);
    }
    if(right->type != INT_t) {
        printf("Error: Right side of %s operation is not an int expression.\n\n", getOp2Str(op));
        exit(1);
    }

	//	OP2 always produces an int result
    t->type = INT_t;

    t->operands = 2;
    t->left = left;
    t->op = op;
    t->right = right;

    return t;
}

factor * makeFactorFromIdent(char * ident) {
	//	Allocate memory for the new node
    factor * f = (factor *) malloc(sizeof(factor));

	//	Make sure this variable was declared.
    int varHandle = getHandle(ident);

	//	If not
    if(varHandle == -1) {
        printf("Error: Variable \"%s\" was not declared.\n\n", ident);
        exit(1);
    }

	//	Find out what this variable's type is
    symbol * s = getSymbol(varHandle);

	//	The type of the factor is the same as the type of the variable.
    f->type = s->type;
    f->factorType = FACTOR_VAR;
    f->u.varHandle = varHandle;

	//	We're done with the variable name
    free(ident);

    return f;
}
factor * makeFactorFromNum(int n) {
	//	Allocate memory for the new node
    factor * f = (factor *) malloc(sizeof(factor));

    f->type = INT_t;
    f->factorType = FACTOR_NUM;
    f->u.num = n;

    return f;
}
factor * makeFactorFromBool(bool b) {
	//	Allocate memory for the new node
    factor * f = (factor *) malloc(sizeof(factor));

    f->type = BOOL_t;
    f->factorType = FACTOR_BOOL;
    f->u.boollit = b;

    return f;
}
factor * makeFactorFromExpr(expression * e) {
	//	Allocate memory for the new node
    factor * f = (factor *) malloc(sizeof(factor));

	//	The type of the factor is the same as the type of the expression.
    f->type = e->type;
    f->factorType = FACTOR_EXPR;
    f->u.expr = e;

    return f;
}
