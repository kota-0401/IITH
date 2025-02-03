/*Definition Section*/

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
int yylex();
void comment();
FILE* cpp_file;
%}

/* Rules Section */

blanks [ \t]+
letter [a-zA-Z]
digit [0-9]
constant ({digit}+)

%%

\n {yylineno++; fprintf(cpp_file,"%s",yytext);}
[/][*]([^*]|[*]+[^*/])*[*]+[/] {comment(); fprintf(cpp_file,"%s",yytext);}
[/][/][^\n]* {fprintf(cpp_file,"%s",yytext);}

"void" { strcpy(yylval.obj.value, yytext); return VOID; }
"int" { strcpy(yylval.obj.value, yytext); return INT; }
"float" { strcpy(yylval.obj.value, yytext); return FLOAT; }
"char" { strcpy(yylval.obj.value, yytext); return CHAR; }
"string" { strcpy(yylval.obj.value, yytext); return STRING; }
"bool" { strcpy(yylval.obj.value, yytext); return BOOL; }
"point" { strcpy(yylval.obj.value, yytext); return POINT; }
"line" { strcpy(yylval.obj.value, yytext); return LINE; }
"circle" { strcpy(yylval.obj.value, yytext); return CIRCLE; }
"parabola" { strcpy(yylval.obj.value, yytext); return PARABOLA; }
"hyperbola" { strcpy(yylval.obj.value, yytext); return HYPERBOLA; }
"ellipse" { strcpy(yylval.obj.value, yytext); return ELLIPSE; }

"NULL" { strcpy(yylval.obj.value, yytext); return NULL_; }
"if" { strcpy(yylval.obj.value, yytext); return IF; }
"elif" { strcpy(yylval.obj.value, yytext); return ELIF; }
"else" { strcpy(yylval.obj.value, yytext); return ELSE; }
"continue" { strcpy(yylval.obj.value, yytext); return CONTINUE; }
"break" { strcpy(yylval.obj.value, yytext); return BREAK; }
"return" { strcpy(yylval.obj.value, yytext); return RETURN; }
"for" { strcpy(yylval.obj.value, yytext); return FOR; }
"while" { strcpy(yylval.obj.value, yytext); return WHILE; }
"print" { strcpy(yylval.obj.value, yytext); return PRINT; }
"input" { strcpy(yylval.obj.value, yytext); return INPUT; }

"equation" { strcpy(yylval.obj.value, yytext); return EQUATION; }
"eccentricity" { strcpy(yylval.obj.value, yytext); return ECCENTRICITY; }
"tangent" { strcpy(yylval.obj.value, yytext); return TANGENT; }
"normal" { strcpy(yylval.obj.value, yytext); return NORMAL; }
"is_point" { strcpy(yylval.obj.value, yytext); return IS_POINT; }
"centre" { strcpy(yylval.obj.value, yytext); return CENTRE; }
"radius" { strcpy(yylval.obj.value, yytext); return RADIUS; }
"xcor" { strcpy(yylval.obj.value, yytext); return XCOR; }
"ycor" { strcpy(yylval.obj.value, yytext); return YCOR; }
"slope"  { strcpy(yylval.obj.value, yytext); return SLOPE; }


"," { strcpy(yylval.obj.value, yytext); return COMMA; }
";" { strcpy(yylval.obj.value, yytext); return SEMICOLON; }
":" { strcpy(yylval.obj.value, yytext); return COLON; }

"{" { strcpy(yylval.obj.value, yytext); return OFLOWER; }
"}" { strcpy(yylval.obj.value, yytext); return CFLOWER; }
"(" { strcpy(yylval.obj.value, yytext); return OBRACKET; }
")" { strcpy(yylval.obj.value, yytext); return CBRACKET; }
"[" { strcpy(yylval.obj.value, yytext); return OSQUARE; }
"]" { strcpy(yylval.obj.value, yytext); return CSQUARE; }

NEG { strcpy(yylval.obj.value, yytext); return NEG_OP; }
AND { strcpy(yylval.obj.value, yytext); return AND_OP; }
OR  { strcpy(yylval.obj.value, yytext); return OR_OP; }
"." { strcpy(yylval.obj.value, yytext); return ACCESS_OP; }
"++" { strcpy(yylval.obj.value, yytext); return INCREMENT_OP; }
"--" { strcpy(yylval.obj.value, yytext); return DECREMENT_OP; }

"+=" { strcpy(yylval.obj.value, yytext); return ASSIGN_OP; }
"-=" { strcpy(yylval.obj.value, yytext); return ASSIGN_OP; }
"*=" { strcpy(yylval.obj.value, yytext); return ASSIGN_OP; }
"/=" { strcpy(yylval.obj.value, yytext); return ASSIGN_OP; }
"%=" { strcpy(yylval.obj.value, yytext); return ASSIGN_OP; }
"|=" { strcpy(yylval.obj.value, yytext); return ASSIGN_OP; }
"&=" { strcpy(yylval.obj.value, yytext); return ASSIGN_OP; }

"<=" { strcpy(yylval.obj.value, yytext); return RELATIVE_OP; }
"<" { strcpy(yylval.obj.value, yytext); return RELATIVE_OP; }
">=" { strcpy(yylval.obj.value, yytext); return RELATIVE_OP; }
">" { strcpy(yylval.obj.value, yytext); return RELATIVE_OP; }
"==" { strcpy(yylval.obj.value, yytext); return RELATIVE_OP; }
"!=" { strcpy(yylval.obj.value, yytext); return RELATIVE_OP; }

"-" { strcpy(yylval.obj.value, yytext); return SUB_OP; }
"+" {  strcpy(yylval.obj.value, yytext); return ARITHMETIC_OP; }
"*" {  strcpy(yylval.obj.value, yytext); return ARITHMETIC_OP; }
"/" {  strcpy(yylval.obj.value, yytext); return ARITHMETIC_OP; }
"%" {  strcpy(yylval.obj.value, yytext); return ARITHMETIC_OP; }
"|" {  strcpy(yylval.obj.value, yytext); return ARITHMETIC_OP; }
"&" {  strcpy(yylval.obj.value, yytext); return ARITHMETIC_OP; }

"=" { strcpy(yylval.obj.value, yytext); return ASSIGN; }
"TRUE"|"FALSE" { strcpy(yylval.obj.value, yytext); strcpy(yylval.obj.eletype, "bool"); return BOOL_CONST; }
{letter}({digit}|{letter}|_)* { strcpy(yylval.obj.value, yytext); return ID; }
{constant}?[.]{constant}? { strcpy(yylval.obj.value, yytext); strcpy(yylval.obj.eletype, "float"); return FLOAT_CONST; }
{constant} { strcpy(yylval.obj.value, yytext); strcpy(yylval.obj.eletype, "int"); return INT_CONST; }
['][\\][n]['] { strcpy(yylval.obj.value, yytext); strcpy(yylval.obj.eletype, "char"); return CHAR_CONST; }
['][^'\n]?['] { strcpy(yylval.obj.value, yytext); strcpy(yylval.obj.eletype, "char"); return CHAR_CONST; }
["][^"\n]*["] { strcpy(yylval.obj.value, yytext); strcpy(yylval.obj.eletype, "string"); return STRING_CONST; }
{blanks} {fprintf(cpp_file,"%s",yytext);}
. {fprintf(yyout,"Lexical Error at Line %d: %s (invalid token)\n",yylineno, yytext); return 0;}

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
