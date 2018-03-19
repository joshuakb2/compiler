#ifndef ops_h
#define ops_h

enum OP2_v { TIMES, DIV, MOD };
enum OP3_v { PLUS, MINUS };
enum OP4_v { EQ, NE, LT, LE, GT, GE };

int getOp2(const char * const);
int getOp3(const char * const);
int getOp4(const char * const);

char * getOp2Str(enum OP2_v);
char * getOp3Str(enum OP3_v);
char * getOp4Str(enum OP4_v);

#endif
