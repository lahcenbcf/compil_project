lightscript: lightScriptAnalyseur.l syntaxique.y 
	flex -l lightScriptAnalyseur.l 
	bison -d -v syntaxique.y 
	gcc lex.yy.c syntaxique.tab.c semantic.c quadruplets.c Table_symbole.c stack.c -lfl -o test
