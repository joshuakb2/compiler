#ifndef ops_h
#define ops_h

typedef enum { TIMES, DIV, MOD } OP2_e;
typedef enum { PLUS, MINUS } OP3_e;
typedef enum { EQ, NE, LT, LE, GT, GE } OP4_e;

int getOp2(const char * const);
int getOp3(const char * const);
int getOp4(const char * const);

char * getOp2Str(OP2_e);
char * getOp3Str(OP3_e);
char * getOp4Str(OP4_e);

char * getOp2StrC(OP2_e);
char * getOp3StrC(OP3_e);
char * getOp4StrC(OP4_e);

#endif
