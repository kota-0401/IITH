%{
#include <bits/stdc++.h>
#include "symbol_table.hpp"
extern FILE* yyin;
extern FILE* yyout;
extern FILE* cpp_file;
extern int yylineno;
int yylex(void);
void yyerror(const char *a);
%}

%union {
	struct variable_name {
    char* value;
    char* type ;
    char* eletype;
	} obj; 
}

%token <obj> VOID
%token <obj> INT
%token <obj> FLOAT
%token <obj> CHAR
%token <obj> STRING
%token <obj> BOOL
%token <obj> POINT
%token <obj> LINE
%token <obj> CIRCLE
%token <obj> PARABOLA
%token <obj> HYPERBOLA
%token <obj> ELLIPSE

%token <obj> NULL_
%token <obj> IF
%token <obj> ELIF
%token <obj> ELSE
%token <obj> CONTINUE
%token <obj> BREAK
%token <obj> RETURN
%token <obj> FOR
%token <obj> WHILE
%token <obj> PRINT
%token <obj> INPUT

%token <obj> EQUATION
%token <obj> ECCENTRICITY
%token <obj> TANGENT
%token <obj> NORMAL
%token <obj> IS_POINT
%token <obj> CENTRE
%token <obj> RADIUS
%token <obj> XCOR
%token <obj> YCOR
%token <obj> SLOPE



%token <obj> COMMA
%token <obj> SEMICOLON
%token <obj> COLON

%token <obj> OFLOWER
%token <obj> CFLOWER
%token <obj> OBRACKET
%token <obj> CBRACKET
%token <obj> OSQUARE
%token <obj> CSQUARE

%token <obj> NEG_OP
%token <obj> AND_OP
%token <obj> OR_OP
%token <obj> ACCESS_OP
%token <obj> INCREMENT_OP
%token <obj> DECREMENT_OP

%token <obj> ASSIGN_OP

%token <obj> RELATIVE_OP


%token <obj> SUB_OP
%token <obj> ARITHMETIC_OP

%token <obj> ASSIGN
%token <obj> ID
%token <obj> FLOAT_CONST
%token <obj> INT_CONST
%token <obj> CHAR_CONST
%token <obj> STRING_CONST
%token <obj> BOOL_CONST
 
%type <obj> data_type func_exp call_stmt conditional_exp id_1 initialize_exp_a K I J E M N call_stmt_1 property 


%start S

%%
/*productions*/
S : function_declarations S
  | function_definitions S
  | dec_stmt S 
  | /*empty*/{}
  ;

function_declarations : func_exp OBRACKET data_type CBRACKET OBRACKET par_list CBRACKET SEMICOLON {
                                                                    add_function_value = add_function(charptr_to_string($<obj.value>1), charptr_to_string($<obj.value>3)); 
                                                                    if(add_function_value) {
                                                                      printf("Semantic Error at Line %d: ambiguating new declaration of function %s\n", yylineno, $<obj.value>1); 
                                                                    } 
                                                                    add_function_value = 0; 
                                                                    param_list.clear();
                                                                    delete_symbol_table();
                                                                    }
                      | func_exp OBRACKET data_type CBRACKET OBRACKET CBRACKET SEMICOLON {
                                                                    add_function_value = add_function(charptr_to_string($<obj.value>1), charptr_to_string($<obj.value>3)); 
                                                                    if(add_function_value) {
                                                                      printf("Semantic Error at Line %d: ambiguating new declaration of function %s\n", yylineno, $<obj.value>1); 
                                                                    } 
                                                                    add_function_value = 0; 
                                                                    delete_symbol_table();
                                                                    }                      
                      | func_exp OBRACKET VOID CBRACKET OBRACKET par_list CBRACKET SEMICOLON {
                                                                    add_function_value = add_function(charptr_to_string($<obj.value>1), charptr_to_string($<obj.value>3)); 
                                                                    if(add_function_value) {
                                                                      printf("Semantic Error at Line %d: ambiguating new declaration of function %s\n", yylineno, $<obj.value>1); 
                                                                    } 
                                                                    add_function_value = 0; 
                                                                    param_list.clear();
                                                                    delete_symbol_table();
                                                                    }                      
                      | func_exp OBRACKET VOID CBRACKET OBRACKET CBRACKET SEMICOLON {
                                                                    add_function_value = add_function(charptr_to_string($<obj.value>1), charptr_to_string($<obj.value>3)); 
                                                                    if(add_function_value) {
                                                                      printf("Semantic Error at Line %d: ambiguating new declaration of function %s\n", yylineno, $<obj.value>1); 
                                                                    } 
                                                                    add_function_value = 0; 
                                                                    delete_symbol_table();
                                                                    }                      
                      ;

function_definitions : func_exp OBRACKET data_type CBRACKET OBRACKET par_list CBRACKET OFLOWER stmt CFLOWER {
                                                                    add_function_value = add_function_body(charptr_to_string($<obj.value>1), charptr_to_string($<obj.value>3)); 
                                                                    if(add_function_value == 2) {
                                                                      printf("Semantic Error at Line %d: ambiguating new declaration of function %s\n", yylineno, $<obj.value>1);
                                                                    } 
                                                                    else if(add_function_value == 0) {
                                                                      printf("Semantic Error at Line %d: redefinition of function %s\n", yylineno, $<obj.value>1);  
                                                                    } 
                                                                    add_function_value = 0; 
                                                                    param_list.clear();
                                                                    delete_symbol_table();
                                                                    }                     
                     | func_exp OBRACKET data_type CBRACKET OBRACKET CBRACKET OFLOWER stmt CFLOWER {
                                                                    add_function_value = add_function_body(charptr_to_string($<obj.value>1), charptr_to_string($<obj.value>3)); 
                                                                    if(add_function_value == 2) {
                                                                      printf("Semantic Error at Line %d: ambiguating new declaration of function %s\n", yylineno, $<obj.value>1);
                                                                    } 
                                                                    else if(add_function_value == 0) {
                                                                      printf("Semantic Error at Line %d: redefinition of function %s\n", yylineno, $<obj.value>1);  
                                                                    } 
                                                                    add_function_value = 0; 
                                                                    delete_symbol_table();
                                                                    }                      
                     | func_exp OBRACKET VOID CBRACKET OBRACKET par_list CBRACKET OFLOWER stmt CFLOWER {
                                                                    add_function_value = add_function_body(charptr_to_string($<obj.value>1), charptr_to_string($<obj.value>3)); 
                                                                    if(add_function_value == 2) {
                                                                      printf("Semantic Error at Line %d: ambiguating new declaration of function %s\n", yylineno, $<obj.value>1);
                                                                    } 
                                                                    else if(add_function_value == 0) {
                                                                      printf("Semantic Error at Line %d: redefinition of function %s\n", yylineno, $<obj.value>1);  
                                                                    } 
                                                                    add_function_value = 0; 
                                                                    param_list.clear();
                                                                    delete_symbol_table();
                                                                    }                       
                     | func_exp OBRACKET VOID CBRACKET OBRACKET CBRACKET OFLOWER stmt CFLOWER {
                                                                    add_function_value = add_function_body(charptr_to_string($<obj.value>1), charptr_to_string($<obj.value>3)); 
                                                                    if(add_function_value == 2) {
                                                                      printf("Semantic Error at Line %d: ambiguating new declaration of function %s\n", yylineno, $<obj.value>1);
                                                                    } 
                                                                    else if(add_function_value == 0) {
                                                                      printf("Semantic Error at Line %d: redefinition of function %s\n", yylineno, $<obj.value>1);  
                                                                    } 
                                                                    add_function_value = 0; 
                                                                    delete_symbol_table();
                                                                    } 
                     ;

func_exp : ID {
            if (is_variable_declared(charptr_to_string($<obj.value>1))) {
              printf("Semantic Error at Line %d: redeclaration of global variable %s\n",yylineno, $<obj.value>1); 
            }
            else {
              create_symbol_table();
              strcpy($<obj.value>$, $<obj.value>1);
            }
            }
         ;

par_list : data_type ID COMMA par_list {
                        if (insert_param(charptr_to_string($<obj.value>2), charptr_to_string($<obj.value>2)) == 0) {
                          printf("Semantic Error at Line %d: redeclaration of function parameter %s\n", yylineno, $<obj.value>2); 
                        }
                        }
         | data_type ID {
                        if (insert_param(charptr_to_string($<obj.value>2), charptr_to_string($<obj.value>2)) == 0) {
                          printf("Semantic Error at Line %d: redeclaration of function parameter %s\n", yylineno, $<obj.value>2); 
                        }
                        }
         ;

stmt : scope_inc stmt scope_dec stmt
     | conditional_stmt stmt
     | loop_stmt stmt 
     | exp_stmt stmt
     | dec_stmt stmt
     | print_stmt stmt
     | input_stmt stmt
     | call stmt
     | R stmt
     | BREAK SEMICOLON stmt
     | CONTINUE SEMICOLON stmt
     | /*empty*/
     ;

call : call_stmt SEMICOLON {
                     if (strcmp($<obj.value>1, "void") == 0) {
                      printf("Semantic Error at Line %d: expects an assignment of %s\n", yylineno, $<obj.value>1);
                     }
                     }
     ;

R : RETURN conditional_exp SEMICOLON 
  | RETURN SEMICOLON 
  ;

dec_stmt : data_type dlist SEMICOLON  {eletype = charptr_to_string("");}
         ;
      
dlist : d_list COMMA dlist 
      | d_list
      ; 

d_list : dec_assign
       | ID L {
           if(add_variable(charptr_to_string($<obj.value>1))){
            printf("Semantic Error at Line %d: redeclaration of variable %s\n", yylineno, $<obj.value>1);
           }
           no_of_dim = 0;
           }
       ;

L : /*empty*/ {}
  | L_1 INT_CONST L_2 L 
  | L_1 L_3 CSQUARE L 
  ;

L_1 : OSQUARE {type = charptr_to_string("array");}
    ;

L_2 : CSQUARE {no_of_dim++;}
    ;

L_3 : ID {
           if(!(is_variable_declared(charptr_to_string($<obj.value>1)))){
            printf("Semantic Error at Line %d: undefined variable %s\n", yylineno, $<obj.value>1);
           }
           else if (get_type(charptr_to_string($<obj.value>1)) == charptr_to_string("int")) {
            printf("Semantic Error at Line %d: expected type of int\n", yylineno);
           }
           else {
            no_of_dim++;
           }
        }
    ;

exp_stmt : initialize_exp
         | increment_exp SEMICOLON 
         ;

conditional_stmt : IF while_body scope_inc stmt scope_dec elif_stmt  
                 ;
                           
elif_stmt : ELIF while_body scope_inc stmt scope_dec elif_stmt
          | else_stmt
          ;

else_stmt : ELSE scope_inc stmt scope_dec
          | /*empty*/
          ;

loop_stmt : WHILE while_body scope_inc stmt scope_dec
          | FOR OBRACKET for_body_1 stmt scope_dec
          | FOR for_body_2 OFLOWER stmt scope_dec
          ; 

while_body : OBRACKET conditional_exp CBRACKET {
                if(!(type_checking("bool",charptr_to_string($<obj.value>2)))){
                  printf("Semantic Error at Line %d: expected rhs of type: bool", yylineno);
                }
                }
           ;

for_body_1 : for_body_1_a SEMICOLON increment_exp CBRACKET scope_inc
           ;

for_body_1_a : initialize_exp conditional_exp {
                if(!(type_checking("bool",charptr_to_string($<obj.value>2)))){
                  printf("Semantic Error at Line %d: expected rhs of type: bool", yylineno);
                }
                }
             ;

for_body_2 : for_exp data_type dec_assign for_body_2_a for_body_2_b SEMICOLON increment_exp CBRACKET
           | for_exp data_type id_1 COLON id_2 CBRACKET
           ;

for_body_2_a : SEMICOLON{eletype = charptr_to_string("");}
             ;

for_body_2_b : conditional_exp{
                if(!(type_checking("bool",charptr_to_string($<obj.value>1)))){
                  printf("Semantic Error at Line %d: expected rhs of type: bool", yylineno);
                }
                }
             ;

id_1 : ID {
        if(add_variable(charptr_to_string($<obj.value>1))){
          printf("Semantic Error at Line %d : redeclaration of variable %s\n", yylineno, $<obj.value>1);
        }
        strcpy($<obj.value>$, string_to_char(get_type(charptr_to_string($<obj.value>1))));
        }
     ;

id_2 : ID {
        if(!(is_variable_declared(charptr_to_string($<obj.value>1)))){
          printf("Semantic Error at Line %d : undefined variable %s\n", yylineno, $<obj.value>1);
        }
        }
     ;

dec_assign : id_1 ASSIGN conditional_exp {
                strcpy($<obj.value>1, string_to_char(eletype));
                if(!(type_checking(charptr_to_string($<obj.value>1), charptr_to_string($<obj.value>3)))){
                  printf("Semantic Error at Line %d: expected rhs of type: %s\n", yylineno, $<obj.value>1);
                }
              }
           ;

increment_exp : ID L ASSIGN_OP conditional_exp {
                                     char* temp;
                                     strcpy(temp, string_to_char(get_type(charptr_to_string($<obj.value>1))));
                                     if(strcmp($<obj.value>4, temp) != 0){
                                      printf("Semantic Error at Line %d: expected rhs of type: %s\n", yylineno, temp);
                                     }
                                     }
              | increment_exp_a INCREMENT_OP
              | increment_exp_a DECREMENT_OP 
              ;

increment_exp_a : ID L {
           if(!(is_variable_declared(charptr_to_string($<obj.value>1)))){
            printf("Semantic Error at Line %d : undefined variable %s\n", yylineno, $<obj.value>1);
           }
           else {
            char* temp;
            strcpy(temp, string_to_char(get_type(charptr_to_string($<obj.value>1))));
            if(strcmp("int", temp) != 0 && strcmp("float", temp) != 0 && strcmp("char", temp) != 0 && strcmp("string", temp) != 0){
              printf("Semantic Error at Line %d: expected rhs of type: %s\n", yylineno, temp);
            }
           }
           no_of_dim = 0;
           }

for_exp : OBRACKET {create_symbol_table();}
        ;

scope_inc : OFLOWER {create_symbol_table();}
          ;

scope_dec : CFLOWER {delete_symbol_table();}
          ;

initialize_exp : initialize_exp_a ASSIGN conditional_exp SEMICOLON {
                char* temp;
                strcpy(temp, string_to_char(get_type(charptr_to_string($<obj.value>1))));
                if(!(type_checking(charptr_to_string(temp),charptr_to_string($<obj.value>3)))){
                  printf("Semantic Error at Line %d: expected rhs of type: %s\n", yylineno, $<obj.value>1);
                }
                }
               ;

initialize_exp_a : ID L {
           if(!(is_variable_declared(charptr_to_string($<obj.value>1)))){
            printf("Semantic Error at Line %d : undefined variable %s\n", yylineno, $<obj.value>1);
           }
           else {
            char* temp;
            strcpy(temp, string_to_char(get_type(charptr_to_string($<obj.value>1))));
            strcpy($<obj.value>$, temp);
           }
           no_of_dim = 0;
           }
                 ;

conditional_exp : NEG_OP conditional_exp {strcpy($<obj.value>$, "bool");}
                | K {strcpy($<obj.value>$, $<obj.value>1);}
                ;

K : K AND_OP I {
                if(type_checking(charptr_to_string($<obj.value>1),charptr_to_string($<obj.value>3))){
                  strcpy($<obj.value>$, "bool");
                }
                else{
                  printf("Semantic Error at Line %d: expected rhs of type: %s\n", yylineno, $<obj.value>1);
                }
              }
  | I {strcpy($<obj.value>$, $<obj.value>1);}
  ;
I : I OR_OP J {
                if(type_checking(charptr_to_string($<obj.value>1),charptr_to_string($<obj.value>3))){
                  strcpy($<obj.value>$, "bool");
                }
                else{
                  printf("Semantic Error at Line %d: expected rhs of type: %s\n", yylineno, $<obj.value>1);
                }
              }
  | J {strcpy($<obj.value>$, $<obj.value>1);}
  ; 

J : E {strcpy($<obj.value>$, $<obj.value>1);}
  | OSQUARE E COMMA E CSQUARE {
                                if (type_checking(charptr_to_string($<obj.value>2), "float") && type_checking(charptr_to_string($<obj.value>4), "float")){
                                  strcpy($<obj.value>$, "point");
                                }
                                else{
                                  printf("Semantic Error at Line %d: expected [float, float] types for point\n", yylineno);
                                }
                              }

  | OFLOWER J COMMA J CFLOWER {
                                if (type_checking(charptr_to_string($<obj.value>2), "point") && type_checking(charptr_to_string($<obj.value>4), "float")){
                                  strcpy($<obj.value>$, "line_circle");
                                } 
                                else if(type_checking(charptr_to_string($<obj.value>2), "point") && type_checking(charptr_to_string($<obj.value>4), "point")){
                                  strcpy($<obj.value>$, "parabola");
                                }
                                else{
                                  printf("Semantic Error at Line %d: expected [point,point] types for parabola\n", yylineno);
                                }
                              }
  
  | OFLOWER J COMMA E COMMA E CFLOWER {
                                if (type_checking(charptr_to_string($<obj.value>2), "point") && type_checking(charptr_to_string($<obj.value>4), "float") && type_checking(charptr_to_string($<obj.value>6), "float")){
                                  strcpy($<obj.value>$, "ellipse_hyperbola"); 
                                }
                                else{
                                  printf("Semantic Error at Line %d: expected [point,float,float] types for ellipse\n", yylineno);
                                }
                              }
  ;
E : E ARITHMETIC_OP M {
               if(!type_checking(charptr_to_string($<obj.value>1),charptr_to_string($<obj.value>3))){
                printf("Semantic Error at Line %d: expected rhs of type: %s\n", yylineno, $<obj.value>1);
               } 
               else{
                strcpy($<obj.value>$, $<obj.value>1);
               }
              }
  | E SUB_OP M {
               if(!type_checking(charptr_to_string($<obj.value>1),charptr_to_string($<obj.value>3))){
                printf("Semantic Error at Line %d: expected rhs of type: %s\n", yylineno, $<obj.value>1);
               } 
               else{
                strcpy($<obj.value>$, $<obj.value>1);
               }
              }
  | SUB_OP M {strcpy($<obj.value>$, $<obj.value>2);} 
  | M {strcpy($<obj.value>$, $<obj.value>1);}
  ;

M : M RELATIVE_OP N {strcpy($<obj.value>$, "bool");}
  | N {strcpy($<obj.value>$, $<obj.value>1);}
  ;

N : OBRACKET conditional_exp CBRACKET {strcpy($<obj.value>$, $<obj.value>2);}
  | ID L {strcpy($<obj.value>$, string_to_char(get_type(charptr_to_string($<obj.value>1))));}
  | call_stmt {strcpy($<obj.value>$, $<obj.value>1);}
  | INT_CONST {strcpy($<obj.value>$, "int");}
  | FLOAT_CONST {strcpy($<obj.value>$, "float");}
  | CHAR_CONST {strcpy($<obj.value>$, "char");}
  | STRING_CONST {strcpy($<obj.value>$, "string");}
  | NULL_ {strcpy($<obj.value>$, "NULL");}
  | BOOL_CONST {strcpy($<obj.value>$, "bool");}
  ;

call_stmt : ID OBRACKET call_list CBRACKET {    
                                                if(check_type_eletype(charptr_to_string($<obj.value>1)) == charptr_to_string("not_found")){
                                                  printf("Semantic Error at Line %d: function does not exist", yylineno);
                                                }
                                                else {
                                                  strcpy($<obj.value>$, string_to_char(get_type(charptr_to_string($<obj.value>1))));
                                                }
                                                call_arg_list.clear();
                                           }
          | ID OBRACKET CBRACKET {    
                                                if(check_type_eletype(charptr_to_string($<obj.value>1)) == charptr_to_string("not_found")){
                                                  printf("Semantic Error at Line %d: function does not exist", yylineno);
                                                }
                                                else {
                                                  strcpy($<obj.value>$, string_to_char(get_type(charptr_to_string($<obj.value>1))));
                                                }
                                                call_arg_list.clear();
                                 }
          | call_stmt_1 ACCESS_OP property {
            if((strcmp($<obj.value>1, "circle") == 0)){
              if((strcmp($<obj.type>3, "is_point") == 0) || (strcmp($<obj.type>3, "normal") == 0) || (strcmp($<obj.type>3, "tangent") == 0) || (strcmp($<obj.type>3, "equation") == 0) || (strcmp($<obj.type>3, "eccentricity") == 0) || (strcmp($<obj.type>3, "centre") == 0) || (strcmp($<obj.type>3, "circle") == 0)){
                strcpy($<obj.value>$, $<obj.value>3);
              }
              else {
                printf("Semantic Error at Line %d : invalid access property", yylineno);
              }
            }
            else if(strcmp($<obj.value>1, "line") == 0){
              if((strcmp($<obj.type>3, "line") == 0) || (strcmp($<obj.type>3, "normal") == 0) || (strcmp($<obj.type>3, "is_point") == 0) || (strcmp($<obj.type>3, "tangent") == 0)){
                strcpy($<obj.value>$, $<obj.value>3);
              }
              else {
                printf("Semantic Error at Line %d : invalid access property", yylineno);
              }
            }
            else if(strcmp($<obj.value>1, "point") == 0){
              if(strcmp($<obj.type>3, "point") == 0){
                strcpy($<obj.value>$, $<obj.value>3);
              }
              else {
                printf("Semantic Error at Line %d : invalid access property", yylineno);
              }
            }
            else if((strcmp($<obj.value>1, "parabola") == 0)){
              if((strcmp($<obj.type>3, "is_point") == 0) || (strcmp($<obj.type>3, "normal") == 0) || (strcmp($<obj.type>3, "tangent") == 0) || (strcmp($<obj.type>3, "equation") == 0) || (strcmp($<obj.type>3, "eccentricity") == 0)){
                strcpy($<obj.value>$, $<obj.value>3);
              }
              else {
                printf("Semantic Error at Line %d : invalid access property", yylineno);
              }
            }
            else if((strcmp($<obj.value>1, "ellipse") == 0)){
              if((strcmp($<obj.type>3, "is_point") == 0) || (strcmp($<obj.type>3, "normal") == 0) || (strcmp($<obj.type>3, "tangent") == 0) || (strcmp($<obj.type>3, "equation") == 0) || (strcmp($<obj.type>3, "eccentricity") == 0) || (strcmp($<obj.type>3, "centre") == 0)){
                strcpy($<obj.value>$, $<obj.value>3);
              }
              else {
                printf("Semantic Error at Line %d : invalid access property", yylineno);
              }
            }
            else if((strcmp($<obj.value>1, "hyperbola") == 0)){
              if((strcmp($<obj.type>3, "is_point") == 0) || (strcmp($<obj.type>3, "normal") == 0) || (strcmp($<obj.type>3, "tangent") == 0) || (strcmp($<obj.type>3, "equation") == 0) || (strcmp($<obj.type>3, "eccentricity") == 0) || (strcmp($<obj.type>3, "centre") == 0)){
                strcpy($<obj.value>$, $<obj.value>3);
              }
              else {
                printf("Semantic Error at Line %d : invalid access property", yylineno);
              }
            }
            else {
              printf("Semantic Error at Line %d : invalid access type", yylineno);
            }
          }

call_stmt_1 : ID L{
           if(!(is_variable_declared(charptr_to_string($<obj.value>1)))){
            printf("Semantic Error at Line %d : undefined variable %s\n", yylineno, $<obj.value>1);
           }
           else {
            strcpy($<obj.value>$, string_to_char(get_type(charptr_to_string($<obj.value>1))));
           }
           no_of_dim = 0;
           }

property : IS_POINT OBRACKET J CBRACKET {
                       if(strcpy($<obj.value>3, "point") != 0){
                        printf("Semantic Error at Line %d : invalid argument type", yylineno);
                       }
                       else {
                        strcpy($<obj.value>$, "bool");
                        strcpy($<obj.type>$, "is_point");//line,circle,parabola,ellipse,hyperbola
                       }
                       }
         | EQUATION OBRACKET CBRACKET{
                        strcpy($<obj.value>$, "string"); 
                        strcpy($<obj.type>$, "equation");//line,circle,parabola,ellipse,hyperbola
                        }
         | ECCENTRICITY OBRACKET CBRACKET{
                        strcpy($<obj.value>$, "float"); 
                        strcpy($<obj.type>$, "eccentricity");//circle,parabola,ellipse,hyperbola
                        }
         | TANGENT OBRACKET J CBRACKET {
                       if(strcpy($<obj.value>3, "point") != 0){
                        printf("Semantic Error at Line %d : invalid argument type", yylineno);
                       }
                       else {
                        strcpy($<obj.value>$, "line");
                        strcpy($<obj.type>$, "tangent");//circle,parabola,ellipse,hyperbola
                       }
                       } 
         | NORMAL OBRACKET J CBRACKET {
                       if(strcpy($<obj.value>3, "point") != 0){
                        printf("Semantic Error at Line %d : invalid argument type", yylineno);
                       }
                       else {
                        strcpy($<obj.value>$, "line");
                        strcpy($<obj.type>$, "normal");//line,circle,parabola,ellipse,hyperbola
                       }
                       }
         | CENTRE{
                        strcpy($<obj.value>$, "point"); 
                        strcpy($<obj.type>$, "centre");//circle,ellipse,hyperbola
                        }
         | RADIUS{
                        strcpy($<obj.value>$, "float"); 
                        strcpy($<obj.type>$, "circle");
                        }
         | XCOR{
                        strcpy($<obj.value>$, "float"); 
                        strcpy($<obj.type>$, "point");
                        }
         | YCOR{
                        strcpy($<obj.value>$, "float"); 
                        strcpy($<obj.type>$, "point");
                        }
         | SLOPE{
                        strcpy($<obj.value>$, "float"); 
                        strcpy($<obj.type>$, "line");
                        }
         ;

call_list : conditional_exp COMMA call_list {call_arg_list.push_back(charptr_to_string($<obj.value>1));}
          | conditional_exp {call_arg_list.push_back(charptr_to_string($<obj.value>1));}
          ;

print_stmt : PRINT OBRACKET conditional_exp CBRACKET SEMICOLON
           ;

input_stmt : INPUT OBRACKET input_stmt_1 CBRACKET SEMICOLON
           ;

input_stmt_1 : ID{
           if(!(is_variable_declared(charptr_to_string($<obj.value>1)))){
            printf("Semantic Error at Line %d : undefined variable %s\n", yylineno, $<obj.value>1);
           }
           }
             ;

data_type : INT {strcpy($<obj.value>$, $<obj.value>1); eletype = string_to_char("int");}
          | CHAR {strcpy($<obj.value>$, $<obj.value>1); eletype = string_to_char("char");}
          | STRING {strcpy($<obj.value>$, $<obj.value>1); eletype = string_to_char("string");}
          | BOOL {strcpy($<obj.value>$, $<obj.value>1); eletype = string_to_char("bool");}
          | POINT {strcpy($<obj.value>$, $<obj.value>1); eletype = string_to_char("point");}
          | FLOAT {strcpy($<obj.value>$, $<obj.value>1); eletype = string_to_char("float");}
          | LINE {strcpy($<obj.value>$, $<obj.value>1); eletype = string_to_char("line");}
          | CIRCLE {strcpy($<obj.value>$, $<obj.value>1); eletype = string_to_char("circle");}
          | ELLIPSE {strcpy($<obj.value>$, $<obj.value>1); eletype = string_to_char("ellipse");}
          | PARABOLA {strcpy($<obj.value>$, $<obj.value>1); eletype = string_to_char("parabola");}
          | HYPERBOLA {strcpy($<obj.value>$, $<obj.value>1); eletype = string_to_char("hyperbola");}
          ;
%%

int yywrap(){return 0;}

int main() {
  yyin = fopen("input.txt", "r");
  cpp_file = fopen("output.cpp", "w");
  fprintf(cpp_file,"#include <iostream>\nusing namespace std;\n\n");
  fprintf(cpp_file,"struct point {\n    float x, y;\n};\n\nstruct line {\n    point start, end;");
  int f = yyparse();   
  if(f) {
    printf("Success");
  }
    printf("Failure"); 
    return 0;
}

void yyerror(const char *a) {
  printf("Syntax Error at Line %d: %s\n", yylineno, a);
  exit(0);
}

