%define parse.error verbose

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define YYDEBUG 1

%}

%code requires{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "semantic.h"
#include <math.h>
#include "Table_symbole.h"
#include "quadruplets.h"
#include "stack.h"

}

%union{
    char identifier[255];
    DataType type;
    int integerValue;
    double floatValue;
    bool booleanValue;
    char stringValue[255];
    Symbol * symbole;
    expression expression;
    
}

%token IMPORT 
%token FROM 
%token CONST 

%token <type> INT 
%token <type> STRING 
%token <type> BOOLEAN 
%token <type> FLOAT 

%token FUNCTION 
%token RETURN 
%token OBJECT 
%token IF 
%token ELSE 
%token FOR 
%token WHILE 
%token IN
%token DIV 
%token MUL 

%token MOD 
%token O_PARENT 
%token C_PARENT 
%token O_ACCOL 
%token C_ACCOL 
%token O_BRACK 
%token C_BRACK 

%token <booleanValue> BOOL
%token <stringValue> CHAINE 
%token <floatValue> FLOAT_NUMBER 
%token <integerValue> INT_NUMBER 

%token <identifier> ID 

%token POINT 
%token EQUALS 
%token N_EQUALS 
%token NEGATION 
%token SUP_EQ 
%token INF_EQ 
%token DOUBLE_EQUALS
%token INF 
%token SUP
%token AND 
%token OR 
%token SUB_ASSIGN 
%token ADD_ASSIGN 
%token ASSIGN 
%token INC 
%token DEC 
%token PLUS_SIGN 
%token MINUS_SIGN
%token COMMA 
%token COLON 
%token SEMICOLON


%type <expression> Expression;
%type <symbole> Variable;
%type <type> Type;
%type <type> SimpleType;
%type <symbole> Declaration;
%type <symbole> DeclarationSimple;

%left COMMA
%left OR
%left AND
%left NEGATION

%nonassoc ASSIGN INF SUP_EQ SUP INF_EQ
%nonassoc N_EQUALS ADD_ASSIGN SUB_ASSIGN 
%left PLUS_SIGN MINUS_SIGN
%left MUL DIV MOD

%left POINT C_BRACK O_BRACK
%left O_PARENT C_PARENT



%{
extern FILE *yyin;
extern int yylineno;
extern int yyleng;
extern int yylex();

char* file = "input.txt";

int currentColumn = 1;
quadruplet * quad;
int quadIndex = 1;
pile * stack;


Scope* currentScope = NULL;  // Pointer to the current scope
int nextScopeId = 1;         // Counter for unique scope IDs
SymbolTable table = { NULL, NULL, NULL, NULL, 0};

void yysuccess();
void yyerror(const char *s);
void showLexicalError();
%}
%%




Program: Importations Code
    ;


Importations: %empty
    | Importation Importations
    ;


Importation: 
    IMPORT Functions FROM CHAINE SEMICOLON 
    |     IMPORT O_ACCOL Functions C_ACCOL FROM CHAINE SEMICOLON 

    ;

Functions: ID
    | ID COMMA Functions
    ;

Code: %empty
    | Statement Code
    ;








Statement:

    Declaration SEMICOLON
    | DeclarationObjet SEMICOLON
    | FunctionCall SEMICOLON
    | Assignement SEMICOLON
    | Condition
    | WhileLoop
    | ForLoop
    | RETURN SEMICOLON
    | RETURN Expression SEMICOLON
    | FunctionDeclaration
    ;







FunctionDeclaration:
    FUNCTION ID O_PARENT FuncArgDeclarations C_PARENT COLON Type O_ACCOL Code C_ACCOL
    | FUNCTION ID O_PARENT FuncArgDeclarations C_PARENT O_ACCOL Code C_ACCOL

    ;

FuncArgDeclarations:
    %empty
    | Type ID FuncArgDeclaration

FuncArgDeclaration:
    %empty
    | COMMA Type ID FuncArgDeclaration









ForLoop: 
    FOR O_PARENT DeclarationInit SEMICOLON Expression SEMICOLON Assignement C_PARENT O_ACCOL Code C_ACCOL
    | FOR O_PARENT DeclarationSimple IN Tableau C_PARENT O_ACCOL Code C_ACCOL
    | FOR O_PARENT Declaration IN Variable C_PARENT O_ACCOL Code C_ACCOL
    ;






WhileLoop:
    WhileHead Code C_ACCOL 
    {
        char adresse[10];
    char adresseCondWhile [10];
    


    int addrDebutWhile = depiler(stack); 
    int addrCondWhile = depiler(stack);  
    sprintf(adresseCondWhile,"%d",addrCondWhile);
    insertQuadruplet(&quad,"BR",adresseCondWhile,"","",quadIndex);
    quadIndex++;
    sprintf(adresse,"%d",quadIndex);
    updateQuadruplet(quad,adresse, addrDebutWhile);
    currentScope = currentScope->parentScope;
    }
    ;

WhileHead:
    WhileCondition Expression C_PARENT O_ACCOL {
        if($2.type == DATA_TYPE_BOOLEAN){
        char r[10]; 
        sprintf(r,"R%d",quadIndex-1);	
		insertQuadruplet(&quad,"BZ","tmp","",r,quadIndex);  
		empiler(stack,quadIndex);  
		quadIndex++;
    }else{
        printf("Semantic error : cannot evaluate non boolean expression as condition");
    }
    }


WhileCondition:
    WHILE O_PARENT {
        empiler(stack,quadIndex); 
        currentScope = createScope("while", nextScopeId++, currentScope);


    }









Condition:
    IF O_PARENT Expression C_PARENT O_ACCOL Code C_ACCOL ConditionELSE
    ;
ConditionELSE: %empty
    | ELSE Condition 
    | ELSE O_ACCOL  Code C_ACCOL











Assignement:
    Variable POINT Assignement     
    | Variable RapidAffectation
    | Variable ASSIGN Expression
    {
        if($1 != NULL){
            
            if ($1->category == CONSTANT_SYMBOL) {
                printf("Cannot reassign value to already declared constant\n");
            }else {
            if(getSymbolType($1) != $3.type){
                
                printf("type mismatch\n");
            }else{
                char valeurString[255];
                getStringValue($3,valeurString);
                if(getSymbolType($1) != DATA_TYPE_ARRAY)

                    {
                        setValue($1, valeurString);
                            insertQuadruplet(&quad, ":=", valeurString, "", $1->name, quadIndex);
                            quadIndex++;
                    }
                else
                    {
                        printf("tableau");
                    }

            }
            }
        }
    };




RapidAffectation:
    INC
    | DEC
    | ADD_ASSIGN Expression
    | SUB_ASSIGN Expression

    ;








SimpleType: INT  { $$ = DATA_TYPE_INT;}
    | BOOLEAN { $$ = DATA_TYPE_BOOLEAN;}
    | STRING { $$ = DATA_TYPE_STRING;}
    | FLOAT { $$ = DATA_TYPE_FLOAT;}
    | ID { $$ = DATA_TYPE_OBJECT;}
    ;

Type:
    SimpleType  { $$ = $1; }
    | SimpleType Brackets { $$ = DATA_TYPE_ARRAY; }
    ;
    

Brackets: O_BRACK C_BRACK Brackets
    | %empty







Declaration: 
    DeclarationInit 
    | DeclarationSimple
    ;

DeclarationSimple: Type ID 
    {
        if(findSymbol(&table, $2, currentScope) == NULL){
            Symbol * nouveauSymbole = createSymbol($2, VARIABLE_SYMBOL, currentScope, false, $1, $1, NULL);
            insertSymbol(&table, nouveauSymbole);
            $$ = nouveauSymbole;
        }else{
            printf("Identifiant deja declare : %s\n", $2);
            $$ = NULL;
        }
    }
    | CONST Type ID
    {
        if(findSymbol(&table, $3, currentScope) == NULL){
            Symbol * nouveauSymbole = createSymbol($3, CONSTANT_SYMBOL, currentScope, false, $2, $2, NULL);
            insertSymbol(&table, nouveauSymbole);
            $$ = nouveauSymbole;
        }else{
            printf("Identifiant deja declare : %s\n", $3);
            $$ = NULL;
        }
    }
    ;

DeclarationInit: DeclarationSimple ASSIGN Expression {
    
    if($1 != NULL){
            if(getSymbolType($1) == $3.type){
                char val[255];
                getStringValue($3, val);
                
                setValue($1, val);

                insertQuadruplet(&quad, ":=", val, "", $1->name, quadIndex);
                quadIndex++;
            }else{
                printf("Type mismatch\n");
            }
        }
}





DeclarationObjet: OBJECT ID O_ACCOL MultiplesDeclarations C_ACCOL;

MultiplesDeclarations: DeclarationSimple SEMICOLON
    | DeclarationSimple SEMICOLON MultiplesDeclarations
    ;






FunctionCall:
    ID O_PARENT FuncArguments C_PARENT
    | ID O_PARENT C_PARENT
    ;
FuncArguments:
    Expression
    | Expression COMMA FuncArguments
    ;





Expression:
    INT_NUMBER {$$.type=DATA_TYPE_INT; $$.integerValue = $1;}
    | FLOAT_NUMBER {$$.type=DATA_TYPE_FLOAT; $$.floatValue = $1;}
    | CHAINE {$$.type=DATA_TYPE_STRING; strcpy($$.stringValue, $1);} 
    | BOOL {$$.type=DATA_TYPE_BOOLEAN; $$.booleanValue = $1;}
    | Variable {
        if($1 != NULL){
            char valeurString[255];
            if(getSymbolType($1) != DATA_TYPE_ARRAY){
                strcpy(valeurString, getValue($1));
                switch (getSymbolType($1)){
                    case DATA_TYPE_INT:
                        $$.integerValue = atoi(valeurString);
                        $$.type = DATA_TYPE_INT;
                        break;
                    case DATA_TYPE_FLOAT:
                        $$.floatValue = atof(valeurString);
                        $$.type = DATA_TYPE_FLOAT;
                        break;
                    case DATA_TYPE_STRING:
                        strcpy($$.stringValue, valeurString);
                        $$.type = DATA_TYPE_STRING;
                        break;
                    case DATA_TYPE_BOOLEAN:
                        $$.booleanValue = strcmp(valeurString, "true") == 0;
                        $$.type = DATA_TYPE_BOOLEAN;
                        break;
                    default :
                        $$.type = -1;
                        break;
                    }}else{
                        printf("Les tableux ne sont pas supporte\n");
            }
        }
    }
    | Tableau
    | FunctionCall
    | O_PARENT Expression C_PARENT 
    | NEGATION Expression 
    | MINUS_SIGN Expression 
    | PLUS_SIGN Expression 
    | Expression PLUS_SIGN Expression 
    | Expression MINUS_SIGN Expression 
    | Expression MUL Expression 
    | Expression DIV Expression 
    | Expression INF Expression 
    | Expression INF_EQ Expression 
    | Expression SUP Expression
    {
        if($1.type == $3.type){
                $$.type=DATA_TYPE_BOOLEAN;
                if  ($1.type ==DATA_TYPE_STRING ) 
                    
                    {
                        if(strcmp($1.stringValue,$3.stringValue)> 0)
                        {
                            $$.booleanValue=true;
                        }
                        else{
                            $$.booleanValue=false;
                        }
                        char buff[255];
                        char buff2[255];
                        char qcString[20];
                        strcpy(buff, $1.stringValue);
                        strcpy(buff2, $3.stringValue);
                        sprintf(qcString, "%s%d", "R",quadIndex);
                        insertQuadruplet(&quad, "GT",buff, buff2,qcString, quadIndex);
                        quadIndex++;
                        
                    } else if ($1.type ==DATA_TYPE_INT ) 
                        
                 
                    {
                        if($1.integerValue > $3.integerValue)
                        {
                            $$.booleanValue=true;
                        }
                        else{
                            $$.booleanValue=false;
                        }
                        char buff[255];
                        char buff2[255];
                        char qcString[20];
                        sprintf(buff, "%d", $1.integerValue);
                        sprintf(buff2, "%d", $3.integerValue);
                        sprintf(qcString, "%s%d", "R",quadIndex);
                        insertQuadruplet(&quad, "GT",buff, buff2,qcString, quadIndex);
                        quadIndex++;
                        break;
                    } else if ($1.type ==DATA_TYPE_FLOAT ) 
                            
                 
                    {
                        if($1.floatValue > $3.floatValue)
                        {
                            $$.booleanValue=true;
                        }
                        else{
                            $$.booleanValue=false;
                        }
                        char buff[255];
                        char buff2[255];
                        char qcString[20];
                        sprintf(buff, "%f", $1.floatValue);
                        sprintf(buff2, "%f", $3.floatValue);
                        sprintf(qcString, "%s%d", "R",quadIndex);
                        insertQuadruplet(&quad, "GT",buff, buff2,qcString, quadIndex);
                        quadIndex++;
                        break;
                    }
                            
                         
            }
            else
            {
                printf("Type mismatch\n");
            }
    }
    | Expression SUP_EQ Expression 
    | Expression DOUBLE_EQUALS Expression
    | Expression AND Expression 
    | Expression OR Expression
    | ObjectJson
    ;


Variable: ID POINT Variable
    | ID CrochetsTab
    | ID {
        Symbol * s = findSymbol(&table, $1 , currentScope);
        if(s==NULL){
            printf("Variable inconnue dans ce scope: %s", $1);
            $$ = NULL;
        }else if(getSymbolType(s) == DATA_TYPE_ARRAY){
          
            printf("table");
            $$ = NULL;
        }else{
            $$ = s;
        }
    }
    ;

CrochetsTab:
    O_BRACK Expression C_BRACK CrochetsTab
    | %empty




ObjectJson: O_ACCOL JsonAffectiation C_ACCOL ;

JsonAffectiation: ID COLON Expression SEMICOLON 
    | ID COLON Expression SEMICOLON JsonAffectiation
    ;


Tableau:    
    O_BRACK TabArguments C_BRACK

TabArguments:
    Expression
    | Expression COMMA TabArguments
    | %empty
    ;



%%
 void yysuccess(){
    currentColumn+=yyleng;
}

void yyerror(const char *s) {
  fprintf(stdout, "File '%s', line %d, character %d :  %s \n", file, yylineno, currentColumn, s);
}

int main (void)
{
    /* yydebug = 1; */
    currentScope = createScope("globalScope", nextScopeId++, currentScope);
    stack = (pile *)malloc(sizeof(pile));

    yyin=fopen(file, "r");
    if(yyin==NULL){
        printf("erreur dans l'ouverture du fichier");
        return 1;
    }
    yyparse();  


    fclose(yyin);
    printSymbolTable(&table);
    printAllQuadruplets(quad);

    return 0;
}

void showLexicalError() {

    char line[256], introError[80]; 

    fseek(yyin, 0, SEEK_SET);
    
    int i = 0; 

    while (fgets(line, sizeof(line), yyin)) { 
        i++; 
        if(i == yylineno) break;  
    } 
        
    sprintf(introError, "Lexical error in Line %d : Unrecognized character : ", yylineno);
    printf("%s%s", introError, line);  
    int j=1;
    while(j<currentColumn+strlen(introError)) { printf(" "); j++; }
    printf("^\n");
}