/* Joshua Baker		jkb150030
 * Dr. Gupta
 * CS 4386.001
 * 
 * ops.h
 * 
 * This header file declares the op types which are available in TL13
 * and forward-declares some functions which are used to convert the
 * string representation of those operands in the language to the
 * enum value, and vice versa.
 */

#ifndef ops_h
#define ops_h

//	Types of OP2s
typedef enum { TIMES, DIV, MOD } OP2_e;

//	Types of OP3s
typedef enum { PLUS, MINUS } OP3_e;

//	Types of OP4s
typedef enum { EQ, NE, LT, LE, GT, GE } OP4_e;

//	Converts "+" to OP3_e::PLUS, for instance.
int getOp2(const char * const);
int getOp3(const char * const);
int getOp4(const char * const);

//	Converts OP4_e::EQ to "=", for instance.
char * getOp2Str(OP2_e);
char * getOp3Str(OP3_e);
char * getOp4Str(OP4_e);

//	Converts OP4_e::EQ to "==", for instance
char * getOp2StrC(OP2_e);
char * getOp3StrC(OP3_e);
char * getOp4StrC(OP4_e);

#endif
