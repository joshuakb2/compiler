/* Joshua Baker		jkb150030
 * Dr. Gupta
 * CS 4386.001
 * 
 * lexer.l
 * 
 * This is the input file to flex. It builds the lexical analyzer,
 * which reads in characters from the standard input and outputs tokens.
 */

%{
#define __STDC_WANT_LIB_EXT2__ 1
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../intermediate/parser.tab.h"
#include "ops.h"
#include "main.h"

int getInt(char * s);

%}


/*	token regex/string literal					code	*/
%%

[ \t\r\n]+         			/* White space */ ;
%[^\r\n]*                   /* Comments */ ;
[1-9][0-9]*|0				{ yylval.i = getInt(yytext); /*printf("%-10s %s\n", "NUM", yytext);*/ return NUM; }
false|true					{ yylval.b = yytext[0] == 't'; /*printf("%-10s %s\n", "BOOLLIT", yytext);*/ return BOOLLIT; }
[A-Z][A-Z0-9]*				{ yylval.s = strdup(yytext); /*printf("%-10s %s\n", "IDENT", yytext);*/ return IDENT; }
"("               		  	{ /*printf("%-10s %s\n", "LP", yytext);*/ return LP; }
")"               		  	{ /*printf("%-10s %s\n", "RP", yytext);*/ return RP; }
":="						{ /*printf("%-10s %s\n", "ASGN", yytext);*/ return ASGN; }
";"							{ /*printf("%-10s %s\n", "SC", yytext);*/ return SC; }
"*"|"div"|"mod"				{ yylval.op2 = getOp2(yytext); /*printf("%-10s %s\n", "OP2", yytext);*/ return OP2; }
"+"|"-"             		{ yylval.op3 = getOp3(yytext); /*printf("%-10s %s\n", "OP3", yytext);*/ return OP3; }
"="|"!="|"<"|">"|"<="|">="	{ yylval.op4 = getOp4(yytext); /*printf("%-10s %s\n", "OP4", yytext);*/ return OP4; }
if							{ /*printf("%-10s %s\n", "IF", yytext);*/ return IF; }
then						{ /*printf("%-10s %s\n", "THEN", yytext);*/ return THEN; }
else						{ /*printf("%-10s %s\n", "ELSE", yytext);*/ return ELSE; }
begin						{ /*printf("%-10s %s\n", "BEGIN_", yytext);*/ return BEGIN_; }
end							{ /*printf("%-10s %s\n", "END", yytext);*/ return END; }
while						{ /*printf("%-10s %s\n", "WHILE", yytext);*/ return WHILE; }
do							{ /*printf("%-10s %s\n", "DO", yytext);*/ return DO; }
program						{ /*printf("%-10s %s\n", "PROGRAM", yytext);*/ return PROGRAM; }
var							{ /*printf("%-10s %s\n", "VAR", yytext);*/ return VAR; }
as							{ /*printf("%-10s %s\n", "AS", yytext);*/ return AS; }
int							{ /*printf("%-10s %s\n", "INT", yytext);*/ return INT; }
bool						{ /*printf("%-10s %s\n", "BOOL", yytext);*/ return BOOL; }
writeInt					{ /*printf("%-10s %s\n", "WRITEINT", yytext);*/ return WRITEINT; }
readInt						{ /*printf("%-10s %s\n", "READINT", yytext);*/ return READINT; }

%%

int getInt(char * s) {
    int c = strlen(s);

    //  The maximum value is 10 digits long
    //  The given integer is above the maximum
    if(c > 10)
        yyerror("The integer literal %s is above the maximum allowed value (2147483647).", s);
    //  The given integer is below the maximum
    else if(c < 10)
        return atoi(s);
    else {
        //  Both have 10 digits, so alphanumeric comparison == numeric comparison
        c = strcmp(s, "2147483647");

        //  The given integer is less than or equal to the maximum
        if(c <= 0)
            return atoi(s);
        //  The given integer is above the maximum
        else
            yyerror("The integer literal %s is above the maximum allowed value (2147483647).", s);
    }
}
