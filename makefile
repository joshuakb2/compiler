main:	intermediate/parser.tab.o intermediate/lex.yy.o intermediate/ops.o intermediate/structTypes.o intermediate/makeStructs.o intermediate/compile.o intermediate/symbolTable.o intermediate/sdbm.o intermediate/main.o intermediate/utility.o
	gcc intermediate/*.o -lfl -g -o tl13compiler

intermediate/parser.tab.o: intermediate/parser.tab.c
	gcc -g -Iheaders -o intermediate/parser.tab.o -c intermediate/parser.tab.c

intermediate/lex.yy.o: intermediate/lex.yy.c
	gcc -g -Iheaders -o intermediate/lex.yy.o -c intermediate/lex.yy.c

intermediate/ops.o: src/ops.c
	gcc -g -Iheaders -o intermediate/ops.o -c src/ops.c

intermediate/structTypes.o: src/structTypes.c
	gcc -g -Iheaders -o intermediate/structTypes.o -c src/structTypes.c

intermediate/makeStructs.o: src/makeStructs.c
	gcc -g -Iheaders -o intermediate/makeStructs.o -c src/makeStructs.c

intermediate/compile.o: src/compile.c
	gcc -g -Iheaders -o intermediate/compile.o -c src/compile.c

intermediate/symbolTable.o: src/symbolTable.c
	gcc -g -Iheaders -o intermediate/symbolTable.o -c src/symbolTable.c

intermediate/sdbm.o: src/sdbm.c
	gcc -g -Iheaders -o intermediate/sdbm.o -c src/sdbm.c

intermediate/main.o: src/main.c
	gcc -g -Iheaders -o intermediate/main.o -c src/main.c

intermediate/utility.o: src/utility.c
	gcc -g -Iheaders -o intermediate/utility.o -c src/utility.c

intermediate/parser.tab.c: src/parser.y
	bison -o intermediate/parser.tab.c -d src/parser.y

intermediate/lex.yy.c: src/lexer.l intermediate/parser.tab.h
	flex -o intermediate/lex.yy.c src/lexer.l

clean:
	rm -f core tl13compiler intermediate/* examples/*.c examples/*.out
