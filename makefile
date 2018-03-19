main:	intermediate/main.tab.o intermediate/lex.yy.o intermediate/ops.o intermediate/structTypes.o intermediate/makeStructs.o
	gcc intermediate/*.o -lfl -ggdb

intermediate/main.tab.o: intermediate/main.tab.c
	gcc -Iheaders -o intermediate/main.tab.o -c intermediate/main.tab.c

intermediate/lex.yy.o: intermediate/lex.yy.c
	gcc -Iheaders -o intermediate/lex.yy.o -c intermediate/lex.yy.c

intermediate/ops.o: src/ops.c
	gcc -Iheaders -o intermediate/ops.o -c src/ops.c

intermediate/structTypes.o: src/structTypes.c
	gcc -Iheaders -o intermediate/structTypes.o -c src/structTypes.c

intermediate/makeStructs.o: src/makeStructs.c
	gcc -Iheaders -o intermediate/makeStructs.o -c src/makeStructs.c

intermediate/main.tab.c: src/main.y
	bison -o intermediate/main.tab.c -d src/main.y

intermediate/lex.yy.c: src/main.l intermediate/main.tab.h
	flex -o intermediate/lex.yy.c src/main.l

clean:
	rm -f core a.out intermediate/*