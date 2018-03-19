#ifndef structTypes_h
#define structTypes_h

enum varType { INT_t, BOOL_t };
enum statementType { ASSIGN_STMT, IF_STMT, WHILE_STMT, WRITEINT_STMT };
enum assignmentType { ASSIGN_EXPR, ASSIGN_READINT };
enum factorType { FACTOR_VAR, FACTOR_NUM, FACTOR_BOOL, FACTOR_EXPR };

char * getVarType(enum varType);
char * getStatementType(enum statementType);
char * getAssignmentType(enum assignmentType);
char * getFactorType(enum factorType);

#endif
