/* Joshua Baker		jkb150030
 * Dr. Gupta
 * CS 4386.001
 * 
 * structs.h
 * 
 * This header file contains the definitions of all the structures
 * that are used to construct the parse tree.
 */

#ifndef structs_h
#define structs_h

#include <stdbool.h>

#include "ops.h"
#include "structTypes.h"

//	Forward-declarations
struct program_s;
struct declarationSeq_s;
struct declaration_s;
struct statementSeq_s;
struct statement_s;
struct assignment_s;
struct elseClause_s;
struct whileStatement_s;
struct writeInt_s;
struct expression_s;
struct simpleExpression_s;
struct term_s;
struct factor_s;

typedef struct program_s {
	//	List of declarations in reverse order
    struct declarationSeq_s * decls;
	
	//	List of statements in reverse order
    struct statementSeq_s * stmts;
} program;

typedef struct declarationSeq_s {
	//	How many declarations there are
    int count;
	
	//	How large the array is
    int size;
	
	//	The array of pointers to declarations
    struct declaration_s ** decls;
} declarationSeq;

typedef struct declaration_s {
	//	Index referencing the variable in the symbol table.
    int varHandle;
} declaration;

typedef struct statementSeq_s {
	//	How many statements there are
    int count;
	
	//	How large the array is
    int size;
	
	//	The array of pointers to declarations
    struct statement_s ** stmts;
} statementSeq;

typedef struct statement_s {
	//	What kind of statement is this?
    statementType_e type;
    union {
		//	If it's an assignment
        struct assignment_s * asgnStmt;
		
		//	If it's an if
        struct ifStatement_s * ifStmt;
		
		//	If it's a while
        struct whileStatement_s * whileStmt;
		
		//	If it's a writeInt
        struct writeInt_s * writeIntStmt;
    } u;
} statement;

typedef struct assignment_s {
	//	What kind of assignment is this?
    assignmentType_e type;
	
	//	What variable is receiving the value?
    int varHandle;
	
	//	If it's not a readInt assignment, what value should be assigned?
    struct expression_s * expr;
} assignment;

typedef struct ifStatement_s {
	//	If it has an else clause
    bool hasElse;
	
	//	Expression to evaluate
    struct expression_s * condition;
	
	//	Statements to execute if true
    struct statementSeq_s * ifTrue;
	
	//	Statements to execute if false and hasElse
    struct statementSeq_s * ifFalse;
} ifStatement;

typedef struct whileStatement_s {
	//	Self-explanatory
    struct expression_s * condition;
	
	//	Statements to execute while true
    struct statementSeq_s * whileTrue;
} whileStatement;

typedef struct writeInt_s {
	//	What should be printed?
    struct expression_s * expr;
} writeInt;

typedef struct expression_s {
	//	Does this expression evaluate to int or bool?
    varType_e type;
	
	//	1 or 2 simpleExpressions?
    int operands;
	
	//	Mandatory simpleExpression
    struct simpleExpression_s * left;
	
	//	Operation used if there are 2 operands
    OP4_e op;
	
	//	Second simpleExpression if present
    struct simpleExpression_s * right;
} expression;

typedef struct simpleExpression_s {
	//	Does this simpleExpression evaluate to int or bool?
    varType_e type;
	
	//	1 or 2 simpleExpressions?
    int operands;
	
	//	Mandatory term
    struct term_s * left;
    
	//	Operation used if there are 2 operands
	OP3_e op;
    
	//	Second term if present
	struct term_s * right;
} simpleExpression;

typedef struct term_s {
	//	Does this term evaluate to int or bool?
    varType_e type;
	
	//	1 or 2 simpleExpressions?
    int operands;
	
	//	Mandatory factor
    struct factor_s * left;
    
	//	Operation used if there are 2 operands
	OP2_e op;
    
	//	Second factor if present
	struct factor_s * right;
} term;

typedef struct factor_s {
	//	Does this factor evaluate to int or bool?
    varType_e type;
	
	//	What kind of factor is this?
    factorType_e factorType;
    union {
		//	If it's a variable
        int varHandle;
		
		//	If it's an integer literal
        int num;
		
		//	If it's a boolean literal
        bool boollit;
		
		//	If it's an expression in parentheses.
        struct expression_s * expr;
    } u;
} factor;

#endif
