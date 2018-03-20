#ifndef main_h
#define main_h

extern int yylex();
extern int yyparse();

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

typedef enum { e_p, e_decls, e_decl, e_stmts, e_stmt, e_asgn, e_ifstmt, e_wstmt, e_wint, e_expr, e_sexpr, e_trm, e_fact } nodeType_e;

void printProgramTree(program * p);
void printLeadTabs(int tabs);
void printTabs(int tabs);
void printTree(node, nodeType_e, int tabs);
int main();


#endif
