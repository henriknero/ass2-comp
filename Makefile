parser: lex.yy.c interpreter.tab.o main.cc
	g++ -std=c++11 -g -ocomp interpreter.tab.o lex.yy.c main.cc
interpreter.tab.o: interpreter.tab.cc
	g++ -std=c++11 -g -c interpreter.tab.cc
interpreter.tab.cc: interpreter.yy
	bison -v --report-file=report.txt  interpreter.yy
lex.yy.c: interpreter.ll interpreter.tab.cc
	flex interpreter.ll
clean: 
	rm -f interpreter.tab.* lex.yy.c* parser comp parse.txt tree.png cfg.* report.txt
