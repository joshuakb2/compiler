/* Joshua Baker		jkb150030
 * Dr. Gupta
 * CS 4386.001
 * 
 * compile.h
 * 
 * This header file contains forward declarations for functions in compile.c
 * which print C code to the standard output given a node from the parse tree.
 */

#ifndef compile_h
#define compile_h

#include "structs.h"

//	Main compile function
void compile(program * p);

//	Header includes C preprocessor directives, utility functions,
//	and the definition of the main function where all the code goes.
void printHeader();

//	Finishes the main function and frees memory.
void printFooter();

//	Print smaller pieces
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
