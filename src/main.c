/* Joshua Baker		jkb150030
 * Dr. Gupta
 * CS 4386.001
 * 
 * main.c
 * 
 * This file contains the main function as well as the logic for
 * printing the parse tree to the standard output after it's been built.
 */

#define __STDC_WANT_LIB_EXT2__ 1
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "utility.h"
#include "structs.h"
#include "symbolTable.h"
#include "main.h"

void printProgramTree(program * p) {
    node n;
    n.p = p;

    printf("\n\n  PARSE TREE:\n---------------\n");

    printTree(n, e_p, 0);
}

void printTree(node n, nodeType_e t, int tabs) {
    node p;
    switch(t) {
        case e_p:
            printLines(tabs);
            printf("Node type: %s\n", "program");
            p.decls = n.p->decls;
            printTree(p, e_decls, tabs + 1);
            p.stmts = n.p->stmts;
            printTree(p, e_stmts, tabs + 1);
            //free(n.p);
            break;
        case e_decls:
            printLines(tabs);
            printf("Node type: %s\n", "declarationSeq");
            printLines(tabs);
            printf("Count: %d\n", n.decls->count);
            printLines(tabs);
            printf("Size: %d\n", n.decls->size);
            //  The declarations are in reverse order
            for(int i = n.decls->count - 1; i >= 0; i--) {
                p.decl = n.decls->decls[i];
                printTree(p, e_decl, tabs + 1);
                if(i != 0) {
                    printLeadLines(tabs + 1);
                    printf("=\n");
                }
            }
            //free(n.decls);
            break;
        case e_decl: {
            symbol * s = getSymbol(n.decl->varHandle);
            printLines(tabs);
            printf("Node type: %s\n", "declaration");
            printLines(tabs);
            printf("Variable identifier: %s\n", s->key);
            printLines(tabs);
            printf("Variable type: %s\n", getVarType(s->type));
            //free(n.decl);
        }   break;
        case e_stmts:
            printLines(tabs);
            printf("Node type: %s\n", "statementSeq");
            printLines(tabs);
            printf("Count: %d\n", n.stmts->count);
            printLines(tabs);
            printf("Size: %d\n", n.stmts->size);
            //  The statements are in reverse order
            for(int i = n.stmts->count - 1; i >= 0; i--) {
                p.stmt = n.stmts->stmts[i];
                printTree(p, e_stmt, tabs + 1);
                if(i != 0) {
                    printLeadLines(tabs + 1);
                    printf("=\n");
                }
            }
            //free(n.stmts);
            break;
        case e_stmt:
            printLines(tabs);
            printf("Node type: %s\n", "statement");
            printLines(tabs);
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
                    printLines(tabs);
                    printf("THIS SHOULD NEVER HAPPEN!!!\n");
                    break;
            }
            //free(n.stmt);
            break;
        case e_asgn:
            printLines(tabs);
            printf("Node type: %s\n", "assignment");
            printLines(tabs);
            printf("Assignment type: %s\n", getAssignmentType(n.asgn->type));
            printLines(tabs);
            printf("Variable identifier: %s\n", getSymbol(n.asgn->varHandle)->key);
            if(n.asgn->type == ASSIGN_EXPR) {
                p.expr = n.asgn->expr;
                printTree(p, e_expr, tabs + 1);
            }
            //free(n.asgn);
            break;
        case e_ifstmt:
            printLines(tabs);
            printf("Node type: %s\n", "ifStatement");
            printLines(tabs);
            printf("Has else clause: %s\n", n.ifstmt->hasElse ? "Yes" : "No");
            printLines(tabs);
            printf("Condition:\n");
            p.expr = n.ifstmt->condition;
            printTree(p, e_expr, tabs + 1);
            printLines(tabs);
            printf("If true:\n");
            p.stmts = n.ifstmt->ifTrue;
            printTree(p, e_stmts, tabs + 1);
            if(n.ifstmt->hasElse) {
                printLines(tabs);
                printf("If false:\n");
                p.stmts = n.ifstmt->ifFalse;
                printTree(p, e_stmts, tabs + 1);
            }
            //free(n.ifstmt);
            break;
        case e_wstmt:   //  Left off here
            printLines(tabs);
            printf("Node type: %s\n", "whileStatement");
            printLines(tabs);
            printf("Condition:\n");
            p.expr = n.wstmt->condition;
            printTree(p, e_expr, tabs + 1);
            printLines(tabs);
            printf("While true:\n");
            p.stmts = n.wstmt->whileTrue;
            printTree(p, e_stmts, tabs + 1);
            //free(n.wstmt);
            break;
        case e_wint:
            printLines(tabs);
            printf("Node type: %s\n", "writeInt");
            p.expr = n.wint->expr;
            printTree(p, e_expr, tabs + 1);
            //free(n.wint);
            break;
        case e_expr:
            printLines(tabs);
            printf("Node type: %s\n", "expression");
            printLines(tabs);
            printf("Type: %s\n", getVarType(n.expr->type));
            if(n.expr->operands == 1) {
                p.sexpr = n.expr->left;
                printTree(p, e_sexpr, tabs + 1);
            }
            else {
                printLines(tabs);
                printf("Op: %s\n", getOp4Str(n.expr->op));
                printLines(tabs);
                printf("Left:\n");
                p.sexpr = n.expr->left;
                printTree(p, e_sexpr, tabs + 1);
                printLines(tabs);
                printf("Right:\n");
                p.sexpr = n.expr->right;
                printTree(p, e_sexpr, tabs + 1);
            }
            //free(n.expr);
            break;
        case e_sexpr:
            printLines(tabs);
            printf("Node type: %s\n", "simpleExpression");
            printLines(tabs);
            printf("Type: %s\n", getVarType(n.sexpr->type));
            if(n.sexpr->operands == 1) {
                p.trm = n.sexpr->left;
                printTree(p, e_trm, tabs + 1);
            }
            else {
                printLines(tabs);
                printf("Op: %s\n", getOp3Str(n.sexpr->op));
                printLines(tabs);
                printf("Left:\n");
                p.trm = n.sexpr->left;
                printTree(p, e_trm, tabs + 1);
                printLines(tabs);
                printf("Right:\n");
                p.trm = n.sexpr->right;
                printTree(p, e_trm, tabs + 1);
            }
            //free(n.sexpr);
            break;
        case e_trm:
            printLines(tabs);
            printf("Node type: %s\n", "term");
            printLines(tabs);
            printf("Type: %s\n", getVarType(n.trm->type));
            if(n.trm->operands == 1) {
                p.fact = n.trm->left;
                printTree(p, e_fact, tabs + 1);
            }
            else {
                printLines(tabs);
                printf("Op: %s\n", getOp2Str(n.trm->op));
                printLines(tabs);
                printf("Left:\n");
                p.fact = n.trm->left;
                printTree(p, e_fact, tabs + 1);
                printLines(tabs);
                printf("Right:\n");
                p.fact = n.trm->right;
                printTree(p, e_fact, tabs + 1);
            }
            //free(n.trm);
            break;
        case e_fact:
            printLines(tabs);
            printf("Node type: %s\n", "factor");
            printLines(tabs);
            printf("Factor type: %s\n", getFactorType(n.fact->factorType));
            printLines(tabs);
            printf("Type: %s\n", getVarType(n.fact->type));
            switch(n.fact->factorType) {
                case FACTOR_VAR:
                    printLines(tabs);
                    printf("Variable identifier: %s\n", getSymbol(n.fact->u.varHandle)->key);
                    break;
                case FACTOR_NUM:
                    printLines(tabs);
                    printf("Numeric literal: %d\n", n.fact->u.num);
                    break;
                case FACTOR_BOOL:
                    printLines(tabs);
                    printf("Boolean literal: %s\n", n.fact->u.boollit ? "true" : "false");
                    break;
                case FACTOR_EXPR:
                    p.expr = n.fact->u.expr;
                    printTree(p, e_expr, tabs + 1);
                    break;
                default:
                    printLines(tabs);
                    printf("THIS SHOULD NEVER HAPPEN!!!\n");
                    break;
            }
            //free(n.fact);
            break;
        default:
            printLines(tabs);
            printf("THIS SHOULD NEVER HAPPEN!!!\n");
            break;
    }
}

int main() {
    initSymbolTable();

//    printf("%-10s %s\n", "Token", "Lexeme");
//    printf("%s", "-------------------\n");

    if(!yyparse())
        printf("\n%s\n", "");//"Input accepted!");

    freeSymbolTable();

    return 0;
}
