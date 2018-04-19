%{
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
%}

%code requires {
    #include "ops.h"
    #include "structs.h"
    #include "makeStructs.h"
}

%{

#include "ops.h"
#include "structs.h"
#include "makeStructs.h"
#include "compile.h"
#include "main.h"

%}

%union {
    int i;
	bool b;
    char * s;
    OP2_e op2;
    OP3_e op3;
    OP4_e op4;
    program * p;
    declarationSeq * decls;
    varType_e vt;
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
}

%token LP RP ASGN SC IF THEN ELSE BEGIN_ END WHILE DO PROGRAM VAR AS WRITEINT READINT INT BOOL
%token <i> NUM
%token <b> BOOLLIT
%token <s> IDENT
%token <op2> OP2
%token <op3> OP3
%token <op4> OP4

%type <p> program
%type <decls> declarations
%type <vt> type
%type <stmts> statementSequence elseClause
%type <stmt> statement
%type <asgn> assignment
%type <ifstmt> ifStatement
%type <wstmt> whileStatement
%type <wint> writeInt
%type <expr> expression
%type <sexpr> simpleExpression
%type <trm> term
%type <fact> factor

%%

main:               program                                                 { /*printProgramTree($1);*/ compile($1); return 0; }
    ;

program:            PROGRAM declarations BEGIN_ statementSequence END       { $$ = makeProgram($2, $4); }
       ;

declarations:       VAR IDENT AS type SC declarations                       { $$ = addDeclaration($6, makeDeclaration($2, $4)); }
            |       /* empty */                                             { $$ = makeDeclarationSeq(); }
            ;

type:               INT     { $$ = INT_t; }
    |               BOOL    { $$ = BOOL_t; }
    ;

statementSequence:  statement SC statementSequence                          { $$ = addStatement($3, $1); }
                 |  /* empty */                                             { $$ = makeStatementSeq(); }
                 ;

statement:          assignment                                              { $$ = makeStatementFromAssign($1); }
         |          ifStatement                                             { $$ = makeStatementFromIf($1); }
         |          whileStatement                                          { $$ = makeStatementFromWhile($1); }
         |          writeInt                                                { $$ = makeStatementFromWriteInt($1); }
         ;

assignment:         IDENT ASGN expression                                   { $$ = makeAssignmentFromExpr($1, $3); }
          |         IDENT ASGN READINT                                      { $$ = makeAssignmentFromReadInt($1); }
          ;

ifStatement:        IF expression THEN statementSequence elseClause END     { $$ = makeIfStatement($2, $4, $5); }
           ;

elseClause:         ELSE statementSequence                                  { $$ = $2; }
          |         /* empty */                                             { $$ = NULL; }
          ;

whileStatement:     WHILE expression DO statementSequence END               { $$ = makeWhileStatement($2, $4); }
              ;

writeInt:           WRITEINT expression                                     { $$ = makeWriteInt($2); }
        ;

expression:         simpleExpression                                        { $$ = makeExpressionFromSimple($1); }
          |         simpleExpression OP4 simpleExpression                   { $$ = makeExpressionFromOp($1, $2, $3); }
          ;

simpleExpression:   term OP3 term                                           { $$ = makeSimpleExpressionFromOp($1, $2, $3); }
                |   term                                                    { $$ = makeSimpleExpressionFromTerm($1); }
                ;

term:               factor OP2 factor                                       { $$ = makeTermFromOp($1, $2, $3); }
    |               factor                                                  { $$ = makeTermFromFactor($1); }
    ;

factor:             IDENT                                                   { $$ = makeFactorFromIdent($1); }
      |             NUM                                                     { $$ = makeFactorFromNum($1); }
      |             BOOLLIT                                                 { $$ = makeFactorFromBool($1); }
      |             LP expression RP                                        { $$ = makeFactorFromExpr($2); }
      ;


%%



/*
void yyerror(const char * s) {
    printf("\n\nError: %s\n\n", s);
    exit(1);
}
*/
