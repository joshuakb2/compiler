/* Joshua Baker		jkb150030
 * Dr. Gupta
 * CS 4386.001
 * 
 * main.h
 * 
 * This header file contains forward declarations for functions in main.c,
 * typedefs used by those functions, and extern function declarations to call
 * the parser and lexer.
 */

#ifndef main_h
#define main_h

#include <stdarg.h>

extern int yylex();
extern int yyparse();

//	Used for walking through the tree.
typedef union {
    program * p;
    declarationSeq * decls;
    declaration * decl;
    statementSeq * stmts;
    statement * stmt;
    assignment * asgn;
    ifStatement * ifstmt;
    whileStatement * wstmt;
    writeInt * wint;
    expression * expr;
    simpleExpression * sexpr;
    term * trm;
    factor * fact;
} node;

//	Used for printing the tree
typedef enum { e_p, e_decls, e_decl, e_stmts, e_stmt, e_asgn, e_ifstmt, e_wstmt, e_wint, e_expr, e_sexpr, e_trm, e_fact } nodeType_e;

void printProgramTree(program * p);
void printTree(node, nodeType_e, int tabs);

void yyerror(const char * s, ...);

int main();

#endif
