#ifndef structs_h
#define structs_h

#include <stdbool.h>

#include "ops.h"
#include "structTypes.h"

/* typedef */ struct program_s /* program */;
/* typedef */ struct declarationSeq_s /* declarationSeq */;
/* typedef */ struct declaration_s /* declaration */;
/* typedef */ struct statementSeq_s /* statementSeq */;
/* typedef */ struct statement_s /* statement */;
/* typedef */ struct assignment_s /* assignment */;
/* typedef */ struct elseClause_s /* elseClause */;
/* typedef */ struct whileStatement_s /* whileStatement */;
/* typedef */ struct writeInt_s /* writeInt */;
/* typedef */ struct expression_s /* expression */;
/* typedef */ struct simpleExpression_s /* simpleExpression */;
/* typedef */ struct term_s /* term */;
/* typedef */ struct factor_s /* factor */;

typedef struct program_s {
    struct declarationSeq_s * decls;
    struct statementSeq_s * stmts;
} program;

typedef struct declarationSeq_s {
    int count;
    int size;
    struct declaration_s ** decls;
} declarationSeq;

typedef struct declaration_s {
    char * ident;
    enum varType type;
} declaration;

typedef struct statementSeq_s {
    int count;
    int size;
    struct statement_s ** stmts;
} statementSeq;

typedef struct statement_s {
    enum statementType type;
    union {
        struct assignment_s * asgnStmt;
        struct ifStatement_s * ifStmt;
        struct whileStatement_s * whileStmt;
        struct writeInt_s * writeIntStmt;
    } u;
} statement;

typedef struct assignment_s {
    enum assignmentType type;
    char * ident;
    struct expression_s * expr;
} assignment;

typedef struct ifStatement_s {
    bool hasElse;
    struct expression_s * condition;
    struct statementSeq_s * ifTrue;
    struct statementSeq_s * ifFalse;
} ifStatement;

typedef struct whileStatement_s {
    struct expression_s * condition;
    struct statementSeq_s * whileTrue;
} whileStatement;

typedef struct writeInt_s {
    struct expression_s * expr;
} writeInt;

typedef struct expression_s {
    int operands;
    struct simpleExpression_s * left;
    enum OP4_v op;
    struct simpleExpression_s * right;
} expression;

typedef struct simpleExpression_s {
    int operands;
    struct term_s * left;
    enum OP3_v op;
    struct term_s * right;
} simpleExpression;

typedef struct term_s {
    int operands;
    struct factor_s * left;
    enum OP2_v op;
    struct factor_s * right;
} term;

typedef struct factor_s {
    enum factorType type;
    union {
        char * ident;
        int num;
        bool boollit;
        struct expression_s * expr;
    } u;
} factor;

#endif
