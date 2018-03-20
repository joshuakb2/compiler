#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "structs.h"
#include "symbolTable.h"
#include "main.h"

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
        printf("\n%s\n", "Input accepted!");

    freeSymbolTable();

    return 0;
}
