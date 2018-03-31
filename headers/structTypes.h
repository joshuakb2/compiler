/* Joshua Baker		jkb150030
 * Dr. Gupta
 * CS 4386.001
 * 
 * structTypes.h
 * 
 * This header file defines each variant of each structure that
 * can be composed of different sets of data.
 * It also forward-declares a few functions that convert the enum
 * value to a string representation to print when displaying the parse tree.
 */

#ifndef structTypes_h
#define structTypes_h

//	Types of structs (some can be built multiple ways)
typedef enum varType_e { INT_t, BOOL_t } varType_e;
typedef enum statementType_e { ASSIGN_STMT, IF_STMT, WHILE_STMT, WRITEINT_STMT } statementType_e;
typedef enum assignmentType_e { ASSIGN_EXPR, ASSIGN_READINT } assignmentType_e;
typedef enum factorType_e { FACTOR_VAR, FACTOR_NUM, FACTOR_BOOL, FACTOR_EXPR } factorType_e;

//	Converts factorType_e::FACTOR_VAR to "FACTOR_VAR", for instance.
char * getVarType(varType_e);
char * getStatementType(statementType_e);
char * getAssignmentType(assignmentType_e);
char * getFactorType(factorType_e);

#endif
