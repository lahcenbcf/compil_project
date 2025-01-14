%{
#include <stdio.h>
#include "syntaxique.tab.h"
#include "Table_symbole.h"


extern void yysuccess();
extern void yyerror(const char *s);
extern void showLexicalError();
extern int currentColumn;
%}

%option yylineno 
%option noyywrap

DIGIT    [0-9]+
ID       [a-zA-Z][a-zA-Z0-9_]*



%%
"import"    		{
        yysuccess();
        return IMPORT;	
        } 

"from" 				{
        yysuccess();
        return FROM;	
        } 

"const" 			{
        yysuccess();
        return CONST;	
        } 

"int" 				{
        yysuccess();
        yylval.type = DATA_TYPE_INT;
        return INT;	
        } 

"string" 			{
        yysuccess();
        yylval.type= DATA_TYPE_STRING;
        return STRING;	
        } 

"boolean" 			{
        yysuccess();
        yylval.type= DATA_TYPE_BOOLEAN ;
        return BOOLEAN;	
        } 

"float" 			{
        yysuccess();
        yylval.type= DATA_TYPE_FLOAT ;
        return FLOAT;	
        } 

"function" 			{
        yysuccess();
        return FUNCTION;	
        } 

"return" 			{
        yysuccess();
        return RETURN;	
        } 

"object" 			{
        yysuccess();
        return OBJECT;	
        } 

"if" 				{
        yysuccess();
        return IF;	
        } 

"else" 				{
        yysuccess();
        return ELSE;	
        } 

"for" 				{
        yysuccess();
        return FOR;	
        } 

"while" 			{
        yysuccess();
        return WHILE;	
        } 

"in" 				{
        yysuccess();
        return IN;	
        } 

"/" 				{
        yysuccess();
        return DIV;	
        } 
"*" 				{
        yysuccess();
        return MUL;	
        } 

"mod" 				{
        yysuccess();
        return MOD;	
        } 

"(" 				{
        yysuccess();
        return O_PARENT;	
        } 

")" 				{
        yysuccess();
        return C_PARENT;	
        } 

"{" 				{
        yysuccess();
        return O_ACCOL;
        } 

"}"  				{

        yysuccess();
        return C_ACCOL;	
        } 

"[" 				{
        yysuccess();
        return O_BRACK;
        } 

"]" 				{
        yysuccess();
        return C_BRACK;	
        } 

"true"|"false" 			{
        yysuccess();
        if(!strcmp(yytext, "true")){
        yylval.booleanValue = true;
        }else{
        yylval.booleanValue = false;
        }
        return BOOL;
        } 



\/\/.*$       						; // Commentaire sur une ligne 
"/*"([^*]|[\r\n]|(\*+([^*/]|[\r\n])))*\*+"/" 		; //Commentaire sur plusieurs lignes 



\.			{
        yysuccess();
        return POINT;	
        } 

\"[^\"]*\"    			{
        yysuccess();
        strcpy(yylval.stringValue, yytext);
        return CHAINE; 
        } 

\'\'

"==" 				{
        yysuccess();
        return DOUBLE_EQUALS;	
        } 

"!=" 				{
        yysuccess();
        return N_EQUALS;	
        } 

"!" 				{
        yysuccess();
        return NEGATION;	
        } 


">=" 				{
        yysuccess();
        return SUP_EQ;	
        } 

"<=" 				{
        yysuccess();
        return INF_EQ;	
        } 

"<" 				{
        yysuccess();
        return INF;	
        } 

">" 				{
        yysuccess();
        return SUP;	
        } 

"&" 				{
        yysuccess();
        return AND;	
        } 

"|" 				{
        yysuccess();
        return OR;	
        } 

"-=" 				{
        yysuccess();
        return SUB_ASSIGN;	
        } 

"+=" 				{
        yysuccess();
        return ADD_ASSIGN;	
        } 

"=" 				{
        yysuccess();
        return ASSIGN;	
        } 


"++"			{
        yysuccess();
        return INC;	
        } 

"--"		{	
        yysuccess();
        return DEC;
    	} 


"+" 				{
        yysuccess();
        return PLUS_SIGN;	
        } 

"-" 				{
        yysuccess();
        return MINUS_SIGN;	
        } 

{DIGIT}\.{DIGIT}		{
        yysuccess();
        yylval.floatValue = strtod(yytext, NULL);
        return FLOAT_NUMBER;   
        }

{DIGIT}	 			{
        yysuccess();
        yylval.integerValue = atoi(yytext);

        return INT_NUMBER;	
        }

{ID}	 			{
        yysuccess();
        strcpy(yylval.identifier, yytext);

        return ID;	
        }

"," 				{
        yysuccess();
        return COMMA;	
        } 

":" 				{
        yysuccess();
        return COLON;	
        } 

";" 				{
        return SEMICOLON;	
        } 

"\n"|"\r" {
    currentColumn = 1;
    }

[ \t]+ {
    currentColumn+=yyleng;
}


<<EOF>> yyterminate();

. {
    yyerror("Unexpected token");
    showLexicalError();
    yyterminate();
    }

%%

