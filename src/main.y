%{
#include <stdio.h>
#include <stdbool.h>
%}

%code requires {
    #include "ops.h"
    #include "structs.h"
    #include "makeStructs.h"
}

%{
extern int yylex();
extern int yyparse();

void yyerror(const char * s);

#include "ops.h"
#include "structs.h"
#include "makeStructs.h"
#include "symbolTable.h"
#include "compile.h"

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

void printTree(node, nodeType_e, int tabs);
void printProgramTree(program * p);

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

main:               program                                                 { printProgramTree($1); compile($1); return 0; }
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

void yyerror(const char * s) {
    printf("Error: %s\n", s);
}

void printProgramTree(program * p) {
    node n;
    n.p = p;

    printf("\n\n  PARSE TREE:\n---------------\n");

    printTree(n, e_p, 0);
}

void printLeadTabs(int tabs) {
    for(int i = 0; i < tabs - 1; i++) {
        printf("|\t");
    }
}

void printTabs(int tabs) {
    printLeadTabs(tabs);
    if(tabs > 0)
        printf("|------ ");
}

void printTree(node n, nodeType_e t, int tabs) {
    node p;
    switch(t) {
        case e_p:
            printTabs(tabs);
            printf("Node type: %s\n", "program");
            p.decls = n.p->decls;
            printTree(p, e_decls, tabs + 1);
            p.stmts = n.p->stmts;
            printTree(p, e_stmts, tabs + 1);
            free(n.p);
            break;
        case e_decls:
            printTabs(tabs);
            printf("Node type: %s\n", "declarationSeq");
            printTabs(tabs);
            printf("Count: %d\n", n.decls->count);
            printTabs(tabs);
            printf("Size: %d\n", n.decls->size);
            //  The declarations are in reverse order
            for(int i = n.decls->count - 1; i >= 0; i--) {
                p.decl = n.decls->decls[i];
                printTree(p, e_decl, tabs + 1);
                if(i != 0) {
                    printLeadTabs(tabs + 1);
                    printf("=\n");
                }
            }
            free(n.decls);
            break;
        case e_decl: {
            symbol * s = getSymbol(n.decl->varHandle);
            printTabs(tabs);
            printf("Node type: %s\n", "declaration");
            printTabs(tabs);
            printf("Variable identifier: %s\n", s->key);
            printTabs(tabs);
            printf("Variable type: %s\n", getVarType(s->type));
            free(n.decl);
        }   break;
        case e_stmts:
            printTabs(tabs);
            printf("Node type: %s\n", "statementSeq");
            printTabs(tabs);
            printf("Count: %d\n", n.stmts->count);
            printTabs(tabs);
            printf("Size: %d\n", n.stmts->size);
            //  The statements are in reverse order
            for(int i = n.stmts->count - 1; i >= 0; i--) {
                p.stmt = n.stmts->stmts[i];
                printTree(p, e_stmt, tabs + 1);
                if(i != 0) {
                    printLeadTabs(tabs + 1);
                    printf("=\n");
                }
            }
            free(n.stmts);
            break;
        case e_stmt:
            printTabs(tabs);
            printf("Node type: %s\n", "statement");
            printTabs(tabs);
            printf("Statement type: %s\n", getStatementType(n.stmt->type));
            switch(n.stmt->type) {
                case ASSIGN_STMT:
                    p.asgn = n.stmt->u.asgnStmt;
                    printTree(p, e_asgn, tabs + 1);
                    break;
                case IF_STMT:
                    p.ifstmt = n.stmt->u.ifStmt;
                    printTree(p, e_ifstmt, tabs + 1);
                    break;
                case WHILE_STMT:
                    p.wstmt = n.stmt->u.whileStmt;
                    printTree(p, e_wstmt, tabs + 1);
                    break;
                case WRITEINT_STMT:
                    p.wint = n.stmt->u.writeIntStmt;
                    printTree(p, e_wint, tabs + 1);
                    break;
                default:
                    printTabs(tabs);
                    printf("THIS SHOULD NEVER HAPPEN!!!\n");
                    break;
            }
            free(n.stmt);
            break;
        case e_asgn:
            printTabs(tabs);
            printf("Node type: %s\n", "assignment");
            printTabs(tabs);
            printf("Assignment type: %s\n", getAssignmentType(n.asgn->type));
            printTabs(tabs);
            printf("Variable identifier: %s\n", getSymbol(n.asgn->varHandle)->key);
            if(n.asgn->type == ASSIGN_EXPR) {
                p.expr = n.asgn->expr;
                printTree(p, e_expr, tabs + 1);
            }
            free(n.asgn);
            break;
        case e_ifstmt:
            printTabs(tabs);
            printf("Node type: %s\n", "ifStatement");
            printTabs(tabs);
            printf("Has else clause: %s\n", n.ifstmt->hasElse ? "Yes" : "No");
            printTabs(tabs);
            printf("Condition:\n");
            p.expr = n.ifstmt->condition;
            printTree(p, e_expr, tabs + 1);
            printTabs(tabs);
            printf("If true:\n");
            p.stmts = n.ifstmt->ifTrue;
            printTree(p, e_stmts, tabs + 1);
            if(n.ifstmt->hasElse) {
                printTabs(tabs);
                printf("If false:\n");
                p.stmts = n.ifstmt->ifFalse;
                printTree(p, e_stmts, tabs + 1);
            }
            free(n.ifstmt);
            break;
        case e_wstmt:   //  Left off here
            printTabs(tabs);
            printf("Node type: %s\n", "whileStatement");
            printTabs(tabs);
            printf("Condition:\n");
            p.expr = n.wstmt->condition;
            printTree(p, e_expr, tabs + 1);
            printTabs(tabs);
            printf("While true:\n");
            p.stmts = n.wstmt->whileTrue;
            printTree(p, e_stmts, tabs + 1);
            free(n.wstmt);
            break;
        case e_wint:
            printTabs(tabs);
            printf("Node type: %s\n", "writeInt");
            p.expr = n.wint->expr;
            printTree(p, e_expr, tabs + 1);
            free(n.wint);
            break;
        case e_expr:
            printTabs(tabs);
            printf("Node type: %s\n", "expression");
            printTabs(tabs);
            printf("Type: %s\n", getVarType(n.expr->type));
            if(n.expr->operands == 1) {
                p.sexpr = n.expr->left;
                printTree(p, e_sexpr, tabs + 1);
            }
            else {
                printTabs(tabs);
                printf("Op: %s\n", getOp4Str(n.expr->op));
                printTabs(tabs);
                printf("Left:\n");
                p.sexpr = n.expr->left;
                printTree(p, e_sexpr, tabs + 1);
                printTabs(tabs);
                printf("Right:\n");
                p.sexpr = n.expr->right;
                printTree(p, e_sexpr, tabs + 1);
            }
            free(n.expr);
            break;
        case e_sexpr:
            printTabs(tabs);
            printf("Node type: %s\n", "simpleExpression");
            printTabs(tabs);
            printf("Type: %s\n", getVarType(n.sexpr->type));
            if(n.sexpr->operands == 1) {
                p.trm = n.sexpr->left;
                printTree(p, e_trm, tabs + 1);
            }
            else {
                printTabs(tabs);
                printf("Op: %s\n", getOp3Str(n.sexpr->op));
                printTabs(tabs);
                printf("Left:\n");
                p.trm = n.sexpr->left;
                printTree(p, e_trm, tabs + 1);
                printTabs(tabs);
                printf("Right:\n");
                p.trm = n.sexpr->right;
                printTree(p, e_trm, tabs + 1);
            }
            free(n.sexpr);
            break;
        case e_trm:
            printTabs(tabs);
            printf("Node type: %s\n", "term");
            printTabs(tabs);
            printf("Type: %s\n", getVarType(n.trm->type));
            if(n.trm->operands == 1) {
                p.fact = n.trm->left;
                printTree(p, e_fact, tabs + 1);
            }
            else {
                printTabs(tabs);
                printf("Op: %s\n", getOp2Str(n.trm->op));
                printTabs(tabs);
                printf("Left:\n");
                p.fact = n.trm->left;
                printTree(p, e_fact, tabs + 1);
                printTabs(tabs);
                printf("Right:\n");
                p.fact = n.trm->right;
                printTree(p, e_fact, tabs + 1);
            }
            free(n.trm);
            break;
        case e_fact:
            printTabs(tabs);
            printf("Node type: %s\n", "factor");
            printTabs(tabs);
            printf("Factor type: %s\n", getFactorType(n.fact->type));
            printTabs(tabs);
            printf("Type: %s\n", getVarType(n.fact->type));
            switch(n.fact->type) {
                case FACTOR_VAR:
                    printTabs(tabs);
                    printf("Variable identifier: %s\n", getSymbol(n.fact->u.varHandle)->key);
                    break;
                case FACTOR_NUM:
                    printTabs(tabs);
                    printf("Numeric literal: %d\n", n.fact->u.num);
                    break;
                case FACTOR_BOOL:
                    printTabs(tabs);
                    printf("Boolean literal: %s\n", n.fact->u.boollit ? "true" : "false");
                    break;
                case FACTOR_EXPR:
                    p.expr = n.fact->u.expr;
                    printTree(p, e_expr, tabs + 1);
                    break;
                default:
                    printTabs(tabs);
                    printf("THIS SHOULD NEVER HAPPEN!!!\n");
                    break;
            }
            free(n.fact);
            break;
        default:
            printTabs(tabs);
            printf("THIS SHOULD NEVER HAPPEN!!!\n");
            break;
    }
}

int main() {
    initSymbolTable();

    printf("%-10s %s\n", "Token", "Lexeme");
    printf("%s", "-------------------\n");

    if(!yyparse())
        printf("%s", "\nInput accepted!\n");

    freeSymbolTable();

    return 0;
}
