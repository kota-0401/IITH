/*Definition Section*/

%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"
void comment();
%}

/* Rules Section */

blanks [ \t]+
letter [a-zA-Z]
digit [0-9]
constant ({digit}+)

%%

\n {yylineno++; fprintf(yyout,"%s",yytext); }
[/][*]([^*]|[*]+[^*/])*[*]+[/] {fprintf(yyout,"%s",yytext); }
[/][/][^\n]* {fprintf(yyout,"%s",yytext); }

"void" {fprintf(yyout,"%s",yytext); return VOID; }
"int" {fprintf(yyout,"%s",yytext); return INT; }
"float" {fprintf(yyout,"%s",yytext); return FLOAT; }
"char" {fprintf(yyout,"%s",yytext); return CHAR; }
"string" {fprintf(yyout,"%s",yytext); return STRING; }
"bool" {fprintf(yyout,"%s",yytext); return BOOL; }
"point" {fprintf(yyout,"%s",yytext); return POINT; }
"line" {fprintf(yyout,"%s",yytext); return LINE; }
"circle" {fprintf(yyout,"%s",yytext); return CIRCLE; }
"parabola" {fprintf(yyout,"%s",yytext); return PARABOLA; }
"hyperbola" {fprintf(yyout,"%s",yytext); return HYPERBOLA; }
"ellipse" {fprintf(yyout,"%s",yytext); return ELLIPSE; }

"NULL" {fprintf(yyout,"%s",yytext); return NULL_; }
"if" {fprintf(yyout,"%s",yytext); return IF; }
"elif" {fprintf(yyout,"%s",yytext); return ELIF; }
"else" {fprintf(yyout,"%s",yytext); return ELSE; }
"continue" {fprintf(yyout,"%s",yytext); return CONTINUE; }
"break" {fprintf(yyout,"%s",yytext); return BREAK; }
"return" {fprintf(yyout,"%s",yytext); return RETURN; }
"for" {fprintf(yyout,"%s",yytext); return FOR; }
"while" {fprintf(yyout,"%s",yytext); return WHILE; }

"equation" {fprintf(yyout,"%s",yytext); return EQUATION; }
"eccentricity" {fprintf(yyout,"%s",yytext); return ECCENTRICITY; }
"tangent" {fprintf(yyout,"%s",yytext); return TANGENT; }
"normal" {fprintf(yyout,"%s",yytext); return NORMAL; }
"is_point" {fprintf(yyout,"%s",yytext); return IS_POINT; }
"centre" {fprintf(yyout,"%s",yytext); return CENTRE; }
"radius" {fprintf(yyout,"%s",yytext); return RADIUS; }
"xcor" {fprintf(yyout,"%s",yytext); return XCOR; }
"ycor" {fprintf(yyout,"%s",yytext); return YCOR; }
"slope"  {fprintf(yyout,"%s",yytext); return SLOPE; }


"," {fprintf(yyout,"%s",yytext); return COMMA; }
";" {fprintf(yyout,"%s",yytext); return SEMICOLON; }
":" {fprintf(yyout,"%s",yytext); return COLON; }

"{" {fprintf(yyout,"%s",yytext); return OFLOWER; }
"}" {fprintf(yyout,"%s",yytext); return CFLOWER; }
"(" {fprintf(yyout,"%s",yytext); return OBRACKET; }
")" {fprintf(yyout,"%s",yytext); return CBRACKET; }
"[" {fprintf(yyout,"%s",yytext); return OSQUARE; }
"]" {fprintf(yyout,"%s",yytext); return CSQUARE; }

NEG {fprintf(yyout,"%s",yytext); return NEG_OP; }
AND {fprintf(yyout,"%s",yytext); return AND_OP; }
OR  {fprintf(yyout,"%s",yytext); return OR_OP; }
"." {fprintf(yyout,"%s",yytext); return ACCESS_OP; }
"++" {fprintf(yyout,"%s",yytext); return INCREMENT_OP; }
"--" {fprintf(yyout,"%s",yytext); return DECREMENT_OP; }

"+=" {fprintf(yyout,"%s",yytext); return ASSIGN_OP; }
"-=" {fprintf(yyout,"%s",yytext); return ASSIGN_OP; }
"*=" {fprintf(yyout,"%s",yytext); return ASSIGN_OP; }
"/=" {fprintf(yyout,"%s",yytext); return ASSIGN_OP; }
"%=" {fprintf(yyout,"%s",yytext); return ASSIGN_OP; }
"|=" {fprintf(yyout,"%s",yytext); return ASSIGN_OP; }
"&=" {fprintf(yyout,"%s",yytext); return ASSIGN_OP; }

"<=" {fprintf(yyout,"%s",yytext); return RELATIVE_OP; }
"<" {fprintf(yyout,"%s",yytext); return RELATIVE_OP; }
">=" {fprintf(yyout,"%s",yytext); return RELATIVE_OP; }
">" {fprintf(yyout,"%s",yytext); return RELATIVE_OP; }
"==" {fprintf(yyout,"%s",yytext); return RELATIVE_OP; }
"!=" {fprintf(yyout,"%s",yytext); return RELATIVE_OP; }

"+" {fprintf(yyout,"%s",yytext); return ARITHMETIC_OP; }
"-" {fprintf(yyout,"%s",yytext); return ARITHMETIC_OP; }
"*" {fprintf(yyout,"%s",yytext); return ARITHMETIC_OP; }
"/" {fprintf(yyout,"%s",yytext); return ARITHMETIC_OP; }
"%" {fprintf(yyout,"%s",yytext); return ARITHMETIC_OP; }
"|" {fprintf(yyout,"%s",yytext); return ARITHMETIC_OP; }
"&" {fprintf(yyout,"%s",yytext); return ARITHMETIC_OP; }

"=" {fprintf(yyout,"%s",yytext); return ASSIGN; }
"TRUE"|"FALSE" {fprintf(yyout,"%s",yytext); return BOOL_CONST; }
{letter}({digit}|{letter}|_)* {fprintf(yyout,"%s",yytext); return ID; }
{constant}?[.]{constant}? {fprintf(yyout,"%s",yytext); return FLOAT_CONST; }
{constant} {fprintf(yyout,"%s",yytext); return INT_CONST; }
['][\\][n]['] {fprintf(yyout,"%s",yytext); return CHAR_CONST; }
['][^'\n]?['] {fprintf(yyout,"%s",yytext); return CHAR_CONST; }
["][^"\n]*["] {fprintf(yyout,"%s",yytext); return STRING_CONST; }
{blanks} {fprintf(yyout,"%s",yytext); }
. {fprintf(yyout,"\nLexical Error at Line %d: %s (invalid token)\n",yylineno, yytext); exit(0);}

%%
/* User Subroutine section */

void comment() {
  int i;
  for (i = 0; i < yyleng; i++) {
    if (yytext[i] == '\n') {
      yylineno++;
    }
  }
}

int yywrap(){
    return 1;
}
