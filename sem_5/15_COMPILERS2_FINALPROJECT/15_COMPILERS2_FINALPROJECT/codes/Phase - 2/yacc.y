%{
#include <stdio.h>
#include <stdlib.h>
extern FILE* yyout;
extern int yylineno;
int yylex(void);
void yyerror(const char *a);
%}

%token VOID
%token INT
%token FLOAT
%token CHAR
%token STRING
%token BOOL
%token POINT
%token LINE
%token CIRCLE
%token PARABOLA
%token HYPERBOLA
%token ELLIPSE

%token NULL_
%token IF
%token ELIF
%token ELSE
%token CONTINUE
%token BREAK
%token RETURN
%token FOR
%token WHILE

%token EQUATION
%token ECCENTRICITY
%token TANGENT
%token NORMAL
%token IS_POINT
%token CENTRE
%token RADIUS
%token XCOR
%token YCOR
%token SLOPE



%token COMMA
%token SEMICOLON
%token COLON

%token OFLOWER
%token CFLOWER
%token OBRACKET
%token CBRACKET
%token OSQUARE
%token CSQUARE

%token NEG_OP
%token AND_OP
%token OR_OP
%token ACCESS_OP
%token INCREMENT_OP
%token DECREMENT_OP

%token ASSIGN_OP

%token RELATIVE_OP


%token SUB_OP
%token ARITHMETIC_OP

%token ASSIGN
%token ID
%token FLOAT_CONST
%token INT_CONST
%token CHAR_CONST
%token STRING_CONST
%token BOOL_CONST

%start S

%%
/*productions*/
S : function_declarations S
  | function_definitions S
  | dec_stmt S
  | /*empty*/
  ;

function_declarations : ID OBRACKET data_type CBRACKET OBRACKET par_list CBRACKET SEMICOLON {fprintf(yyout," : function declaration");}
                      | ID OBRACKET data_type CBRACKET OBRACKET CBRACKET SEMICOLON {fprintf(yyout," : function declaration");}
                      | ID OBRACKET VOID CBRACKET OBRACKET par_list CBRACKET SEMICOLON {fprintf(yyout," : function declaration");}
                      | ID OBRACKET VOID CBRACKET OBRACKET CBRACKET SEMICOLON {fprintf(yyout," : function declaration");}
                      ;

function_definitions : ID OBRACKET data_type CBRACKET OBRACKET par_list CBRACKET OFLOWER stmt CFLOWER  {fprintf(yyout," : function definition");}
                     | ID OBRACKET data_type CBRACKET OBRACKET CBRACKET OFLOWER stmt CFLOWER {fprintf(yyout," : function definition");}
                     | ID OBRACKET VOID CBRACKET OBRACKET par_list CBRACKET OFLOWER stmt CFLOWER {fprintf(yyout," : function definition");}
                     | ID OBRACKET VOID CBRACKET OBRACKET CBRACKET OFLOWER stmt CFLOWER {fprintf(yyout," : function definition");}
                     ;

par_list : data_type ID COMMA par_list
         | data_type ID
         ;

stmt : OFLOWER stmt CFLOWER stmt
     | conditional_stmt stmt
     | loop_stmt stmt 
     | exp_stmt stmt
     | dec_stmt stmt
     | call stmt
     | R stmt
     | BREAK SEMICOLON stmt
     | CONTINUE SEMICOLON stmt
     | /*empty*/
     ;

call : call_stmt SEMICOLON {fprintf(yyout," : call statement");}
     ;

R : RETURN conditional_exp SEMICOLON {fprintf(yyout," : return statement");}
  | RETURN SEMICOLON {fprintf(yyout," : return statement");}
  ;

dec_stmt : data_type dlist SEMICOLON  {fprintf(yyout," : declaration statement");}
         ;
      
dlist : d_list COMMA dlist
      | d_list
      ; 

d_list : ID ASSIGN conditional_exp
       | ID L
       ;

L : /*empty*/
  | OSQUARE ID CSQUARE L
  | OSQUARE INT_CONST CSQUARE L
  ;

exp_stmt : ID L ASSIGN conditional_exp SEMICOLON {fprintf(yyout," : assignment expression statement");}
         | assign_exp SEMICOLON {fprintf(yyout," : assignment expression statement");}
         | update_exp SEMICOLON {fprintf(yyout," : update expression statement");}
         ;

conditional_stmt : IF while_body OFLOWER stmt CFLOWER elif_stmt {fprintf(yyout," : conditional statement");}
                 ;

elif_stmt : ELIF while_body OFLOWER stmt CFLOWER elif_stmt
          | else_stmt
          ;

else_stmt : ELSE OFLOWER stmt CFLOWER
          | /*empty*/
          ;
                 
loop_stmt : WHILE while_body OFLOWER stmt CFLOWER {fprintf(yyout," : loop");}
          | FOR for_body OFLOWER stmt CFLOWER {fprintf(yyout," : loop");}
          ;
while_body : OBRACKET conditional_exp CBRACKET
           ;
for_body : OBRACKET ID L ASSIGN conditional_exp SEMICOLON conditional_exp SEMICOLON assign_exp CBRACKET
         | OBRACKET data_type ID L ASSIGN conditional_exp SEMICOLON conditional_exp SEMICOLON assign_exp CBRACKET
         | OBRACKET ID L ASSIGN conditional_exp SEMICOLON conditional_exp SEMICOLON update_exp CBRACKET
         | OBRACKET data_type ID L ASSIGN conditional_exp SEMICOLON conditional_exp SEMICOLON update_exp CBRACKET
         | OBRACKET data_type ID COLON ID CBRACKET
         ;

assign_exp : ID L ASSIGN_OP conditional_exp
           ;

conditional_exp : NEG_OP conditional_exp
                | K
                ;
K : K AND_OP I
  | I
  ;

I : I OR_OP J
  | J

J : E
  | OSQUARE E COMMA E CSQUARE
  | OFLOWER J COMMA J CFLOWER
  | OFLOWER J COMMA E COMMA E CFLOWER
  ;

E : E ARITHMETIC_OP M
  | E SUB_OP M
  | SUB_OP M
  | M
  ;

M : M RELATIVE_OP N
  | N
  ;
N : OBRACKET conditional_exp CBRACKET
  | ID L
  | call_stmt
  | INT_CONST
  | FLOAT_CONST
  | CHAR_CONST
  | STRING_CONST
  | NULL_
  | BOOL_CONST
  ;

update_exp : ID L INCREMENT_OP
           | ID L DECREMENT_OP
           ;

call_stmt : ID OBRACKET call_list CBRACKET
          | ID OBRACKET CBRACKET
          | ID L ACCESS_OP property
          ;

property : IS_POINT OBRACKET J CBRACKET
         | EQUATION OBRACKET CBRACKET
         | ECCENTRICITY OBRACKET CBRACKET
         | TANGENT OBRACKET J CBRACKET
         | NORMAL OBRACKET J CBRACKET
         | CENTRE
         | RADIUS
         | XCOR
         | YCOR
         | SLOPE
         ;

call_list : conditional_exp COMMA call_list
          | conditional_exp
          ;

data_type : INT
          | CHAR 
          | STRING 
          | BOOL 
          | POINT 
          | FLOAT 
          | LINE 
          | CIRCLE 
          | ELLIPSE 
          | PARABOLA 
          | HYPERBOLA
          ;
%%

int main() {
    yyparse();   
    return 0;
}

void yyerror(const char *a) {
  printf("\nSyntax Error at Line %d: %s\n", yylineno, a);
  exit(0);
}