#ifndef makeStructs_h
#define makeStructs_h

#include <stdbool.h>

#include "ops.h"
#include "structs.h"

program * makeProgram(declarationSeq *, statementSeq *);

declarationSeq * makeDeclarationSeq();
declarationSeq * addDeclaration(declarationSeq *, declaration *);

declaration * makeDeclaration(char * ident, enum varType);

statementSeq * makeStatementSeq();
statementSeq * addStatement(statementSeq *, statement *);

statement * makeStatementFromAssign(assignment *);
statement * makeStatementFromIf(ifStatement *);
statement * makeStatementFromWhile(whileStatement *);
statement * makeStatementFromWriteInt(writeInt *);

assignment * makeAssignmentFromReadInt(char * ident);
assignment * makeAssignmentFromExpr(char * ident, expression *);

ifStatement * makeIfStatement(expression * condition, statementSeq * ifTrue, statementSeq * ifFalse);

whileStatement * makeWhileStatement(expression * condition, statementSeq * whileTrue);

writeInt * makeWriteInt(expression *);

expression * makeExpressionFromSimple(simpleExpression *);
expression * makeExpressionFromOp(simpleExpression * left, enum OP4_v op, simpleExpression * right);

simpleExpression * makeSimpleExpressionFromTerm(term *);
simpleExpression * makeSimpleExpressionFromOp(term * left, enum OP3_v op, term * right);

term * makeTermFromFactor(factor *);
term * makeTermFromOp(factor * left, enum OP2_v op, factor * right);

factor * makeFactorFromIdent(char * ident);
factor * makeFactorFromNum(int);
factor * makeFactorFromBool(bool);
factor * makeFactorFromExpr(expression *);

#endif
