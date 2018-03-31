/* Joshua Baker		jkb150030
 * Dr. Gupta
 * CS 4386.001
 * 
 * makeStructs.h
 * 
 * This header file contains forward declarations for functions in makeStructs.c.
 * Each function serves the purpose of building and returning a pointer to structures
 * that become nodes in the parse tree. By moving the logic of building these
 * structures into a separate file, the parser file is kept tidy.
 */

#ifndef makeStructs_h
#define makeStructs_h

#include <stdbool.h>

#include "ops.h"
#include "structs.h"

program * makeProgram(declarationSeq *, statementSeq *);

//	Creates a new declarationSeq structure
declarationSeq * makeDeclarationSeq();
//	Adds a declaration to the list and returns the list
declarationSeq * addDeclaration(declarationSeq *, declaration *);

declaration * makeDeclaration(char * ident, varType_e);

//	Creates a new statementSeq structure
statementSeq * makeStatementSeq();
//	Adds a statement to the list and returns the list
statementSeq * addStatement(statementSeq *, statement *);

//	Assignment
statement * makeStatementFromAssign(assignment *);
//	If statement
statement * makeStatementFromIf(ifStatement *);
//	While statement
statement * makeStatementFromWhile(whileStatement *);
//	WriteInt statement
statement * makeStatementFromWriteInt(writeInt *);

//	Value to be assigned is read from the console
assignment * makeAssignmentFromReadInt(char * ident);
//	Value to be assigned is the result of an expression.
assignment * makeAssignmentFromExpr(char * ident, expression *);

ifStatement * makeIfStatement(expression * condition, statementSeq * ifTrue, statementSeq * ifFalse);

whileStatement * makeWhileStatement(expression * condition, statementSeq * whileTrue);

writeInt * makeWriteInt(expression *);

//	1 operand
expression * makeExpressionFromSimple(simpleExpression *);
//	2 operands
expression * makeExpressionFromOp(simpleExpression * left, OP4_e op, simpleExpression * right);

//	1 operand
simpleExpression * makeSimpleExpressionFromTerm(term *);
//	2 operands
simpleExpression * makeSimpleExpressionFromOp(term * left, OP3_e op, term * right);

//	1 operand
term * makeTermFromFactor(factor *);
//	2 operands
term * makeTermFromOp(factor * left, OP2_e op, factor * right);

//	The factor is a reference to a variable
factor * makeFactorFromIdent(char * ident);
//	The factor is an integer literal
factor * makeFactorFromNum(int);
//	The factor is a bool
factor * makeFactorFromBool(bool);
//	The factor is an expression in parentheses
factor * makeFactorFromExpr(expression *);

#endif
