yacc -d -v yacc.y;
lex lexer.l;
g++ -g y.tab.c lex.yy.c -o src;



# #!/bin/bash
# clear
# bison -d yacc.y
# mv yacc.tab.c yacc.tab.cpp
# sed -i -e 's/yacc.tab.c/yacc.tab.cpp/g' yacc.tab.cpp
# sed -i -e 's/yacc.tab.h/yacc.tab.hpp/g' yacc.tab.cpp
# mv yacc.tab.h yacc.tab.hpp
# # sed -i -e 's/y.tab.h/y.tab.hpp/g' lexer.l
# flex  lexer.l
# mv lex.yy.c lex.yy.cpp
# # sed -i -e 's/y.tab.h/y.tab.hpp/g' lex.yy.cpp
# g++ lex.yy.cpp y.tab.cpp symbol_table.cpp
# ./a.out
# echo "Token's output *********************************"
# cat out_token.txt 