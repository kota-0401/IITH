%{
#include <bits/stdc++.h>
#include "symbol_table.cpp"
extern FILE* yyin;
extern FILE *tokfile, *parsefile;

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

%type <obj> N E
%type <obj> M S L J 
%type <obj> function_declarations function_definitions par_list dec_stmt data_type func_exp L_1


%start S

%%
/*productions*/
S : function_declarations S
  | function_definitions S
  | dec_stmt S 
  | /*empty*/{}
  ;

function_declarations : func_exp OBRACKET data_type CBRACKET OBRACKET par_list CBRACKET SEMICOLON {
                                                                    add_function_value = add_function($<obj.value>1, $<obj.value>3); 
                                                                    if(add_function_value) {
                                                                      cout << "Semantic Error at Line" << yylineno << ": ambiguating new declaration of function" << $<obj.value>1 << endl; 
                                                                    } 
                                                                    add_function_value = 0; 
                                                                    param_list.clear();
                                                                    delete_symbol_table();
                                                                    }
                      | func_exp OBRACKET data_type CBRACKET OBRACKET CBRACKET SEMICOLON {
                                                                    add_function_value = add_function($<obj.value>1, $<obj.value>3); 
                                                                    if(add_function_value) {
                                                                      cout << "Semantic Error at Line" << yylineno << ": ambiguating new declaration of function" << $<obj.value>1 << endl; 
                                                                    } 
                                                                    add_function_value = 0; 
                                                                    delete_symbol_table();
                                                                    }                      
                      | func_exp OBRACKET VOID CBRACKET OBRACKET par_list CBRACKET SEMICOLON {
                                                                    add_function_value = add_function($<obj.value>1, ""); 
                                                                    if(add_function_value) {
                                                                      cout << "Semantic Error at Line" << yylineno << ": ambiguating new declaration of function" << $<obj.value>1 << endl; 
                                                                    } 
                                                                    add_function_value = 0; 
                                                                    param_list.clear();
                                                                    delete_symbol_table();
                                                                    }                      
                      | func_exp OBRACKET VOID CBRACKET OBRACKET CBRACKET SEMICOLON {
                                                                    add_function_value = add_function($<obj.value>1, ""); 
                                                                    if(add_function_value) {
                                                                      cout << "Semantic Error at Line" << yylineno << ": ambiguating new declaration of function" << $<obj.value>1 << endl; 
                                                                    } 
                                                                    add_function_value = 0; 
                                                                    delete_symbol_table();
                                                                    }                      
                      ;

function_definitions : func_exp OBRACKET data_type CBRACKET OBRACKET par_list CBRACKET OFLOWER stmt CFLOWER {
                                                                    add_function_value = add_function_body($<obj.value>1, $<obj.value>3); 
                                                                    if(add_function_value == 2) {
                                                                      cout << "Semantic Error at Line" << yylineno << ": ambiguating new declaration of function" << $<obj.value>1 << endl; 
                                                                    } 
                                                                    else if(add_function_value == 0) {
                                                                      cout << "Semantic Error at Line" << yylineno << ": redefinition of function" << $<obj.value>1 << endl; 
                                                                    } 
                                                                    add_function_value = 0; 
                                                                    param_list.clear();
                                                                    delete_symbol_table();
                                                                    }                     
                     | func_exp OBRACKET data_type CBRACKET OBRACKET CBRACKET OFLOWER stmt CFLOWER {
                                                                    add_function_value = add_function_body($<obj.value>1, $<obj.value>3); 
                                                                    if(add_function_value == 2) {
                                                                      cout << "Semantic Error at Line" << yylineno << ": ambiguating new declaration of function" << $<obj.value>1 << endl; 
                                                                    } 
                                                                    else if(add_function_value == 0) {
                                                                      cout << "Semantic Error at Line" << yylineno << ": redefinition of function" << $<obj.value>1 << endl; 
                                                                    } 
                                                                    add_function_value = 0; 
                                                                    delete_symbol_table();
                                                                    }                      
                     | func_exp OBRACKET VOID CBRACKET OBRACKET par_list CBRACKET OFLOWER stmt CFLOWER {
                                                                    add_function_value = add_function_body($<obj.value>1, ""); 
                                                                    if(add_function_value == 2) {
                                                                      cout << "Semantic Error at Line" << yylineno << ": ambiguating new declaration of function" << $<obj.value>1 << endl; 
                                                                    } 
                                                                    else if(add_function_value == 0) {
                                                                      cout << "Semantic Error at Line" << yylineno << ": redefinition of function" << $<obj.value>1 << endl; 
                                                                    } 
                                                                    add_function_value = 0; 
                                                                    param_list.clear();
                                                                    delete_symbol_table();
                                                                    }                       
                     | func_exp OBRACKET VOID CBRACKET OBRACKET CBRACKET OFLOWER stmt CFLOWER {
                                                                    add_function_value = add_function_body($<obj.value>1, ""); 
                                                                    if(add_function_value == 2) {
                                                                      cout << "Semantic Error at Line" << yylineno << ": ambiguating new declaration of function" << $<obj.value>1 << endl; 
                                                                    } 
                                                                    else if(add_function_value == 0) {
                                                                      cout << "Semantic Error at Line" << yylineno << ": redefinition of function" << $<obj.value>1 << endl; 
                                                                    } 
                                                                    add_function_value = 0; 
                                                                    delete_symbol_table();
                                                                    } 
                     ;

func_exp : ID {
            if (is_variable_declared($<obj.value>1)) {
              cout << "Semantic Error at Line" << yylineno << ": redeclaration of global variable " << $<obj.value>1 << endl; 
            }
            else {
              create_symbol_table();
            }
            }
         ;

par_list : data_type ID COMMA par_list {
                        if (insert_param($<obj.value>2, $<obj.value>1) == 0) {
                          cout << "Semantic Error at Line" << yylineno << ": redeclaration of function parameter " << $<obj.value>1 << endl; 
                        }
                        }
         | data_type ID {
                        if (insert_param($<obj.value>2, $<obj.value>1) == 0) {
                          cout << "Semantic Error at Line" << yylineno << ": redeclaration of function parameter " << $<obj.value>1 << endl; 
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
                     if ($<obj.eletype>1 == "") {
                      cout << "Semantic Error at Line" << yylineno << ": expects an assignment of " << $<obj.value>1 << endl; 
                     }
                     }
     ;

R : RETURN conditional_exp SEMICOLON {$<obj.eletype>$ = $<obj.eletype>2;}
  | RETURN SEMICOLON {$<obj.eletype>$ = string_to_char("");}
  ;

dec_stmt : data_type dlist SEMICOLON  {eletype = "";}
         ;
      
dlist : d_list COMMA dlist 
      | d_list
      ; 

d_list : dec_assign
       | ID L {
           if(add_variable($<obj.value>1)){
            cout << "Semantic Error at Line " << yylineno << ": redeclaration of variable " << $<obj.value>1 << endl;
           }
           no_of_dim = 0;
           }
       ;

L : /*empty*/ {}
  | L_1 INT_CONST L_2 L 
  | L_1 L_3 CSQUARE L 
  ;

L_1 : OSQUARE {type = "array";}
    ;

L_2 : CSQUARE {no_of_dim++;}
    ;

L_3 : ID {
           if(!(is_variable_declared($<obj.value>1))){
            cout << "Semantic Error at Line " << yylineno << ": undefined variable " << $<obj.value>1 << endl;
           }
           else if (get_type($<obj.value>1) != "int") {
            cout << "Semantic Error at Line " << yylineno << ": expected type of int" << endl;
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
                if(!(type_checking("bool",$<obj.eletype>2))){
                  cout << "Semantic Error at Line" << yylineno << ": expected rhs of type: bool" << endl;
                }
                }
           ;

for_body_1 : initialize_exp  for_body_1_a SEMICOLON increment_exp CBRACKET scope_inc
           ;

for_body_1_a : conditional_exp {
                if(!(type_checking("bool",$<obj.eletype>1))){
                  cout << "Semantic Error at Line" << yylineno << ": expected rhs of type: bool" << endl;
                }
                }
             ;

for_body_2 : for_exp data_type dec_assign for_body_2_a for_body_1_a SEMICOLON increment_exp CBRACKET
           | for_exp data_type for_body_2_b COLON for_body_2_c CBRACKET
           ;

for_body_2_a : SEMICOLON{eletype = "";}
             ;

for_body_2_b : ID {
              if(add_variable($<obj.value>1)){
                cout << "Semantic Error at Line " << yylineno << ": redeclaration of variable " << $<obj.value>1 << endl;
              }
              }
             ;
for_body_2_c : ID {
              if(!(is_variable_declared($<obj.value>1))){
                cout << "Semantic Error at Line " << yylineno << ": undefined variable " << $<obj.value>1 << endl;
              }
              }
             ;
dec_assign : dec_assign_1 ASSIGN conditional_exp {
                $<obj.eletype>1 = string_to_char(eletype);
                if(!(type_checking($<obj.eletype>1,$<obj.eletype>3))){
                  cout << "Semantic Error at Line" << yylineno << ": expected rhs of type: " << $<obj.eletype>1 << endl;
                }
              }
           ;
dec_assign_1 : ID {
           if(add_variable($<obj.value>1)){
            cout << "Semantic Error at Line " << yylineno << ": redeclaration of variable " << $<obj.value>1 << endl;
           }
           else {
            $<obj.eletype>$ = $<obj.eletype>1;
           }
           }
increment_exp : ID L ASSIGN_OP conditional_exp {
                                     if($<obj.eletype>4 != $<obj.eletype>1){
                                      cout << "Semantic Error at Line" << yylineno << ": expected rhs of type: " << $<obj.eletype>1 << endl;
                                     }
                                     }
              | increment_exp_1 INCREMENT_OP
              | increment_exp_1 DECREMENT_OP 
              ;

increment_exp_1 : ID L {
           if(!(is_variable_declared($<obj.value>1))){
            cout << "Semantic Error at Line " << yylineno << ": undefined variable " << $<obj.value>1 << endl;
           }
           no_of_dim = 0;
           }
               ;

for_exp : OBRACKET {create_symbol_table();}
        ;

scope_inc : OFLOWER {create_symbol_table();}
          ;

scope_dec : CFLOWER {delete_symbol_table();}
          ;

initialize_exp : increment_exp_1 ASSIGN conditional_exp SEMICOLON {
                string temp_type = get_type($<obj.value>1);
                if(!(type_checking(temp_type,$<obj.eletype>4))){
                  cout << "Semantic Error at Line" << yylineno << ": expected rhs of type: " << $<obj.eletype>1 << endl;
           }
           }
               ;

conditional_exp : NEG_OP conditional_exp {$<obj.eletype>$ = $<obj.eletype>2;}
                | K {$<obj.eletype>$ = $<obj.eletype>1;}
                ;
K : K AND_OP I {
                if(type_checking($<obj.eletype>1,$<obj.eletype>3)){
                  $<obj.eletype>$ = $<obj.eletype>1;
                }
                else{
                  cout << "Semantic Error at Line" << yylineno << ": expected rhs of type: " << $<obj.eletype>1 << endl;
                }
              }
  | I {$<obj.type>$ = $<obj.type>1;}
  ;
I : I OR_OP J {
                if(type_checking($<obj.eletype>1,$<obj.eletype>3)){
                  $<obj.eletype>$ = $<obj.eletype>1;
                }
                else{
                  cout << "Semantic Error at Line" << yylineno << ": expected rhs of type: " << $<obj.eletype>1 << endl;
                }
              }
  | J {$<obj.type>$ = $<obj.type>1;}
  ; 

J : E {$<obj.eletype>$ = $<obj.eletype>1;}
  | OSQUARE E COMMA E CSQUARE {
                                if (type_checking($<obj.eletype>2 , "float") && type_checking($<obj.eletype>4 , "float")){
                                  $<obj.eletype>$ = string_to_char("point");}
                                else{yyerror("1");}
                              }

  | OFLOWER J COMMA J CFLOWER {
                                if (type_checking($<obj.eletype>2 , "point") && type_checking($<obj.eletype>4 , "float")){
                                  $<obj.eletype>$ = string_to_char("line_circle");} 
                                else if(type_checking($<obj.eletype>2 , "point") && type_checking($<obj.eletype>4 , "float")){
                                  $<obj.eletype>$ = string_to_char("parabola");}
                                else {yyerror("2");}
                              }
  
  | OFLOWER J COMMA E COMMA E CFLOWER {
                                if (type_checking($<obj.eletype>2 , "point") && type_checking($<obj.eletype>4 , "float") && type_checking($<obj.eletype>6 , "float")){
                                  $<obj.eletype>$ = string_to_char("ellipse_hyperbola");} 
                                else {yyerror("3");}
                              }
  ;
E : E ARITHMETIC_OP M {
               if(!type_checking($<obj.eletype>1,$<obj.eletype>3)){
                cout << "Semantic Error at Line" << yylineno << ": expected rhs of type: " << $<obj.eletype>1 << endl;
               } 
               else{
                $<obj.eletype>$ = $<obj.eletype>1;
               }
              }
  | E SUB_OP M {
               if(!type_checking($<obj.eletype>1,$<obj.eletype>3)){
                cout << "Semantic Error at Line" << yylineno << ": expected rhs of type: " << $<obj.eletype>1 << endl;
               } 
               else{
                $<obj.eletype>$ = $<obj.eletype>1;
               }
              }
  | SUB_OP M {$<obj.eletype>$ = $<obj.eletype>2;} 
  | M {$<obj.eletype>$ = $<obj.eletype>1;}
  ;

M : M RELATIVE_OP N {$<obj.eletype>$ = string_to_char("bool");}
  | N {$<obj.eletype>$ = $<obj.eletype>1;}
  ;

N : OBRACKET conditional_exp CBRACKET {$<obj.eletype>$ = $<obj.eletype>2;}
  | ID L {$<obj.eletype>$ = string_to_char(get_type($<obj.eletype>1));}
  | call_stmt {$<obj.eletype>$ = $<obj.eletype>1;}
  | INT_CONST {$<obj.eletype>$ = string_to_char("int");}
  | FLOAT_CONST {$<obj.eletype>$ = string_to_char("float");}
  | CHAR_CONST {$<obj.eletype>$ = string_to_char("char");}
  | STRING_CONST {$<obj.eletype>$ = string_to_char("string");}
  | NULL_ {$<obj.eletype>$ = string_to_char("NULL");}
  | BOOL_CONST {$<obj.eletype>$ = string_to_char("bool");}
  ;

call_stmt : call_stmt_1 OBRACKET call_list CBRACKET {    string temp = check_type_eletype($<obj.value>1);
                                                if(check_type_eletype($<obj.value>1) == "not_found"){
                                                  cout << "Semantic Error at Line" << yylineno << ": function doe not exist" << endl;
                                                }
                                                else {
                                                  $<obj.type>$ = string_to_char(temp);
                                                }
                                              call_arg_list.clear();
                                            }
          | call_stmt_1 OBRACKET CBRACKET {    string temp = check_type_eletype($<obj.value>1);
                                                if(check_type_eletype($<obj.value>1) == "not_found"){
                                                  cout << "Semantic Error at Line" << yylineno << ": function doe not exist" << endl;
                                                }
                                                else {
                                                  $<obj.type>$ = string_to_char(temp);
                                                }
                                            }
          | call_stmt_2 ACCESS_OP property {$<obj.eletype>$ = $<obj.eletype>3;}

call_stmt_1 : ID{
              if(!(is_variable_declared($<obj.value>1))){
                cout << "Semantic Error at Line " << yylineno << ": undefined variable " << $<obj.value>1 << endl;
              }
              }
            ;

call_stmt_2 : ID L{
           if(!(is_variable_declared($<obj.value>1))){
            cout << "Semantic Error at Line " << yylineno << ": undefined variable " << $<obj.value>1 << endl;
           }
           else {
            if(($<obj.eletype>1 != "point") && ($<obj.eletype>1 != "line") && ($<obj.eletype>1 != "circle") && ($<obj.eletype>1 !="parabola") && ($<obj.eletype>1 !="ellipse") && ($<obj.eletype>1 !="hyperbola")){
              cout << "Semantic Error at Line " << yylineno << ": invalid access type" << endl;
            }
           }
           no_of_dim = 0;
           }
            ;

property : IS_POINT OBRACKET E CBRACKET {
                       if($<obj.eletype>3 != "point"){
                        cout << "Semantic Error at Line " << yylineno << ": invalid argument type" << endl;
                       }
                       else {
                        $<obj.eletype>$ = string_to_char("bool");
                       }
                       }
         | EQUATION OBRACKET CBRACKET{$<obj.eletype>$ = string_to_char("string");}
         | ECCENTRICITY OBRACKET CBRACKET{$<obj.eletype>$ = string_to_char("float");}
         | TANGENT OBRACKET E CBRACKET {
                       if($<obj.eletype>3 != "point"){
                        cout << "Semantic Error at Line " << yylineno << ": invalid argument type" << endl;
                       }
                       else {
                        $<obj.eletype>$ = string_to_char("line");
                       }
                       } 
         | NORMAL OBRACKET E CBRACKET {
                       if($<obj.eletype>3 != "point"){
                        cout << "Semantic Error at Line " << yylineno << ": invalid argument type" << endl;
                       }
                       else {
                        $<obj.eletype>$ = string_to_char("line");
                       }
                       }
         | CENTRE{$<obj.eletype>$ = string_to_char("point");}
         | RADIUS{$<obj.eletype>$ = string_to_char("float");}
         | XCOR{$<obj.eletype>$ = string_to_char("float");}
         | YCOR{$<obj.eletype>$ = string_to_char("float");}
         | SLOPE{$<obj.eletype>$ = string_to_char("float");}
         ;

call_list : conditional_exp COMMA call_list {call_arg_list.push_back($<obj.value>1);}
          | conditional_exp {call_arg_list.push_back($<obj.value>1);}
          ;

print_stmt : PRINT OBRACKET E CBRACKET SEMICOLON
           ;

input_stmt : INPUT OBRACKET call_stmt_1 CBRACKET SEMICOLON
           ;

data_type : INT {$<obj.eletype>$ = string_to_char("int"); eletype = "int";}
          | CHAR {$<obj.eletype>$ = string_to_char("char"); eletype = "char";}
          | STRING {$<obj.eletype>$ = string_to_char("string"); eletype = "string";}
          | BOOL {$<obj.eletype>$ = string_to_char("bool"); eletype = "bool";}
          | POINT {$<obj.eletype>$ = string_to_char("point"); eletype = "point";}
          | FLOAT {$<obj.eletype>$ = string_to_char("float"); eletype = "float";}
          | LINE {$<obj.eletype>$ = string_to_char("line"); eletype = "line";}
          | CIRCLE {$<obj.eletype>$ = string_to_char("circle"); eletype = "circle";}
          | ELLIPSE {$<obj.eletype>$ = string_to_char("ellipse"); eletype = "ellipse";}
          | PARABOLA {$<obj.eletype>$ = string_to_char("parabola"); eletype = "parabola";}
          | HYPERBOLA {$<obj.eletype>$ = string_to_char("hyperbola"); eletype = "hyperbola";}
          ;
%%

void yyerror(const char *a) {
	printf("Syntax Error : In line number %d\n",yylineno);
	fprintf(parsefile," %s\n",a);
}



int main(int argc ,char * argv[]){
    char inp_file[100],tok[100],parse[100],out_file[100];

    sprintf(inp_file,"inp_1.txt");

    yyin = fopen(inp_file,"r");

	int i = yyparse();

	if(i) printf("Failure\n");
	else printf("Success\n");

	return 0;
}