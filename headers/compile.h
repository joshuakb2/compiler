#ifndef compile_h
#define compile_h

#include "structs.h"

void compile(program * p);
void printHeader();
void printFooter();
void declareVars(declarationSeq * decls, int tabs);
void printStatements(statementSeq * stmts, int tabs);
void printAssignment(assignment * a, int tabs);
void printIf(ifStatement * i, int tabs);
void printWhile(whileStatement * w, int tabs);
void printWriteInt(writeInt * w, int tabs);
void printExpression(expression * e);
void printSimpleExpression(simpleExpression * e);
void printTerm(term * t);
void printFactor(factor * f);

#endif
