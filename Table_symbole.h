#include <stddef.h>  // for NULL

#ifndef TABLE_SYMBOLE_H
#define TABLE_SYMBOLE_H

#include <stdbool.h>

// Définition des catégories de symboles
typedef enum {
    VARIABLE_SYMBOL,
    CONSTANT_SYMBOL,
    FUNCTION_SYMBOL,
    KEYWORD_SYMBOL
} SymbolCategory;

// Définition des types de données
typedef enum {
    DATA_TYPE_INT,
    DATA_TYPE_STRING,
    DATA_TYPE_BOOLEAN,
    DATA_TYPE_FLOAT,
    DATA_TYPE_OBJECT,
    DATA_TYPE_ARRAY,
    DATA_TYPE_VOID

} DataType;

// Définition des types de retour pour les fonctions
// typedef enum {
//     RETURN_VOID,
//     RETURN_INT,
//     RETURN_FLOAT,
//     RETURN_BOOL,
//     RETURN_STRING,
//     RETURN_CHAR
// } ReturnType;





// Définition de la portée
typedef struct Scope {
    char* name;
    struct Scope* parentScope;
    int scopeId;
} Scope;

// Définition de la structure des symboles
typedef struct Symbol {
    char* name;
    SymbolCategory category;
    union {
        struct {
            DataType dataType;
            bool isParameter;
            char* value;
        } variable;
        struct {
            DataType returnType;
            int paramCount;
        } function;
        struct {
            char* keywordValue;
        } keyword;
        struct {
            DataType dataType;
            bool isParameter;
            char* value;
        } constant;
    } details;
    struct Scope* scope;
    struct Symbol* next;
} Symbol;

// Définition de la table des symboles
typedef struct {
    Symbol* variables;
    Symbol* constants;
    Symbol* functions;
    Symbol* keywords;
    int totalSize;
} SymbolTable;

// Prototypes des fonctions
const char* getValue(Symbol* symbol);
void setValue(Symbol* symbol, const char* value) ;
void setType(Symbol* symbol, DataType newType);
DataType getSymbolType(Symbol* symbol);
Scope* createScope(char* scopeName, int scopeId, Scope* parentScope);
Symbol* createSymbol(char* name, SymbolCategory category, Scope* scope,
                     bool isParameter, DataType dataType, DataType returnType, char* value);
void insertSymbol(SymbolTable* table, Symbol* symbol);
Symbol* findSymbol(SymbolTable* table, char* name, Scope* scope);
void insertSymbolIfNotExists(SymbolTable* table, char* name, SymbolCategory category, Scope* scope);
void printSymbolTable(SymbolTable* table); // Optionnel pour visualiser les tests
bool isAlphabetical(const char* str);


#endif // TABLE_SYMBOLE_H
