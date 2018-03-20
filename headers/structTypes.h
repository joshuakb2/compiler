#ifndef structTypes_h
#define structTypes_h

typedef enum varType_e { INT_t, BOOL_t } varType_e;
typedef enum statementType_e { ASSIGN_STMT, IF_STMT, WHILE_STMT, WRITEINT_STMT } statementType_e;
typedef enum assignmentType_e { ASSIGN_EXPR, ASSIGN_READINT } assignmentType_e;
typedef enum factorType_e { FACTOR_VAR, FACTOR_NUM, FACTOR_BOOL, FACTOR_EXPR } factorType_e;

char * getVarType(varType_e);
char * getStatementType(statementType_e);
char * getAssignmentType(assignmentType_e);
char * getFactorType(factorType_e);

#endif
