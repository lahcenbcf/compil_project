#include "Table_symbole.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


const char* getValue(Symbol* symbol) {
    if (!symbol) {
        fprintf(stderr, "Error: Symbol is NULL, cannot retrieve value.\n");
        return NULL;
    }

    switch (symbol->category) {
        case VARIABLE_SYMBOL:
            return symbol->details.variable.value;

        case CONSTANT_SYMBOL:
            return symbol->details.constant.value;

        default:
            fprintf(stderr, "Error: This symbol category does not have a value.\n");
            return NULL;
    }
}



void setValue(Symbol* symbol, const char* value) {
    if (!symbol) {
        fprintf(stderr, "Error: Symbol is NULL, cannot set value.\n");
        return;
    }

    if (!value) {
        fprintf(stderr, "Error: Value is NULL, cannot set value.\n");
        return;
    }

    switch (symbol->category) {
        case VARIABLE_SYMBOL:
            if (symbol->details.variable.value) {
                free(symbol->details.variable.value); // Free existing value if allocated
            }
            symbol->details.variable.value = strdup(value); // Assign new value
            break;

        case CONSTANT_SYMBOL:
            if (symbol->details.constant.value == NULL) {
                symbol->details.constant.value = strdup(value); // Free existing value if allocated
            } else{
                printf("cannot reassign value to constant variable");
            }
            
             // Assign new value
            break;

        default:
            fprintf(stderr, "Error: Value cannot be set for this symbol category.\n");
            break;
    }
}


void setType(Symbol* symbol, DataType newType) {
    if (!symbol) {
        fprintf(stderr, "Error: Symbol is NULL, cannot set type.\n");
        return;
    }

    switch (symbol->category) {
        case VARIABLE_SYMBOL:
            symbol->details.variable.dataType = newType;
            break;

        case CONSTANT_SYMBOL:
            symbol->details.constant.dataType = newType;
            break;

        case FUNCTION_SYMBOL:
            symbol->details.function.returnType = newType;
            break;

        default:
            fprintf(stderr, "Error: Unknown symbol category, cannot set type.\n");
    }
}


DataType getSymbolType(Symbol* symbol) {
    if (!symbol) {
        return DATA_TYPE_VOID;
    }

    switch (symbol->category) {
        case VARIABLE_SYMBOL:
            return symbol->details.variable.dataType;

        case CONSTANT_SYMBOL:
            return (symbol->details.constant.dataType) ;
                
        case FUNCTION_SYMBOL:
            return (symbol->details.function.returnType);
         

        default:
            return DATA_TYPE_VOID;
    }
}
// Création d'une nouvelle portée
Scope* createScope(char* scopeName, int scopeId, Scope* parentScope) {
    Scope* scope = (Scope*)malloc(sizeof(Scope));
    if (!scope) {
        perror("Erreur d'allocation pour la portée");
        exit(EXIT_FAILURE);
    }
    scope->name = strdup(scopeName);
    scope->scopeId = scopeId;
    scope->parentScope = parentScope;
    return scope;
}

// Création d'un nouveau symbole
Symbol* createSymbol(char* name, SymbolCategory category, Scope* scope,
                     bool isParameter, DataType dataType, DataType returnType, char* value) {
    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    if (!newSymbol) {
        perror("Erreur d'allocation pour le symbole");
        exit(EXIT_FAILURE);
    }

    newSymbol->name = strdup(name);
    newSymbol->category = category;
    newSymbol->scope = scope;

    switch (category) {
        case VARIABLE_SYMBOL:
            newSymbol->details.variable.isParameter = isParameter;
            newSymbol->details.variable.dataType = dataType;
            newSymbol->details.variable.value = value ? strdup(value) : NULL;
            break;
        case FUNCTION_SYMBOL:
            newSymbol->details.function.returnType = returnType;
            newSymbol->details.function.paramCount = isParameter ? 1 : 0;
            break;
        case CONSTANT_SYMBOL:
            newSymbol->details.variable.isParameter = isParameter;
            newSymbol->details.variable.dataType = dataType;
            newSymbol->details.constant.value = value ? strdup(value) : NULL;
            break;
        case KEYWORD_SYMBOL:
            newSymbol->details.keyword.keywordValue = value ? strdup(value) : NULL;
            break;
    }

    newSymbol->next = NULL;
    return newSymbol;
}

// Insertion d'un symbole dans la table
void insertSymbol(SymbolTable* table, Symbol* symbol) {
    if (!table || !symbol) {
        fprintf(stderr, "Table ou symbole invalide\n");
        return;
    }

    Symbol** list = NULL;

    switch (symbol->category) {
        case VARIABLE_SYMBOL:
            list = &(table->variables);
            break;
        case CONSTANT_SYMBOL:
            list = &(table->constants);
            break;
        case FUNCTION_SYMBOL:
            list = &(table->functions);
            break;
        case KEYWORD_SYMBOL:
            list = &(table->keywords);
            break;
    }

    if (list) {
        symbol->next = *list;
        *list = symbol;
        table->totalSize++;
    } else {
        fprintf(stderr, "Catégorie de symbole inconnue\n");
    }
}




// Recherche d'un symbole
Symbol* findSymbol(SymbolTable* table, char* name, Scope* scope) {
    while (scope) {
        Symbol* categories[] = {table->variables, table->constants, table->functions, table->keywords};
        for (int i = 0; i < 4; i++) {
            Symbol* current = categories[i];
            while (current) {
                if (strcmp(current->name, name) == 0 && current->scope == scope) {
                    return current;
                }
                current = current->next;
            }
        }
        scope = scope->parentScope;
    }
    return NULL;
}

// Insertion sécurisée d'un symbole
void insertSymbolIfNotExists(SymbolTable* table, char* name, SymbolCategory category, Scope* scope) {
    Symbol* existingSymbol = findSymbol(table, name, scope);
    if (existingSymbol) {
        fprintf(stderr, "Erreur : Le symbole '%s' existe déjà dans la portée '%s'\n", name, scope->name);
        return;
    }

    Symbol* newSymbol = createSymbol(name, category, scope, false, DATA_TYPE_INT, DATA_TYPE_VOID, NULL);
    insertSymbol(table, newSymbol);
}

// Impression de la table des symboles
// void printSymbolTable(SymbolTable* table) {
//     printf("Table des symboles :\n");
//     printf("Variables :\n");
//     for (Symbol* var = table->variables; var; var = var->next) {
//         printf("  %s\n", var->name);
//     }
//     // Idem pour les autres catégories (constants, functions, keywords)
// }

// Helper function to print details of a variable symbol
















void printVariableSymbol(Symbol* symbol) {
    printf("| %-15s | VARIABLE  | %-10s | %-10s | %-10s |\n",
           symbol->name,
           (symbol->details.variable.dataType == DATA_TYPE_INT) ? "INT" :
           (symbol->details.variable.dataType == DATA_TYPE_STRING) ? "STRING" :
           (symbol->details.variable.dataType == DATA_TYPE_BOOLEAN) ? "BOOLEAN" :
           (symbol->details.variable.dataType == DATA_TYPE_FLOAT) ? "FLOAT" :
           (symbol->details.variable.dataType == DATA_TYPE_OBJECT) ? "OBJECT" : "NONE",
           symbol->details.variable.isParameter ? "PARAM" : "LOCAL",
           symbol->details.variable.value ? symbol->details.variable.value : "NULL");
}

// Helper function to print details of a function symbol
void printFunctionSymbol(Symbol* symbol) {
    printf("| %-15s | FUNCTION  | %-10s | Params: %-3d       |\n",
           symbol->name,
           (symbol->details.function.returnType == DATA_TYPE_VOID) ? "VOID" :
           (symbol->details.function.returnType == DATA_TYPE_INT) ? "INT" :
           (symbol->details.function.returnType == DATA_TYPE_FLOAT) ? "FLOAT" :
           (symbol->details.function.returnType == DATA_TYPE_BOOLEAN) ? "BOOL" :
           (symbol->details.function.returnType == DATA_TYPE_STRING) ? "STRING" :
           symbol->details.function.paramCount);
}

// Helper function to print details of a keyword symbol
void printKeywordSymbol(Symbol* symbol) {
    printf("| %-15s | KEYWORD   | %-25s            |\n",
           symbol->name,
           symbol->details.keyword.keywordValue ? symbol->details.keyword.keywordValue : "NULL");
}

// Helper function to print details of a constant symbol
void printConstantSymbol(Symbol* symbol) {
    printf("| %-15s | CONSTANT  | %-10s | %-10s | %-10s |\n",
           symbol->name,
           (symbol->details.constant.dataType == DATA_TYPE_INT) ? "INT" :
           (symbol->details.constant.dataType == DATA_TYPE_STRING) ? "STRING" :
           (symbol->details.constant.dataType == DATA_TYPE_BOOLEAN) ? "BOOLEAN" :
           (symbol->details.constant.dataType == DATA_TYPE_FLOAT) ? "FLOAT" :
           (symbol->details.constant.dataType == DATA_TYPE_OBJECT) ? "OBJECT" : "NONE",
           symbol->details.constant.isParameter ? "PARAM" : "LOCAL",
           symbol->details.constant.value ? symbol->details.variable.value : "NULL");
}

// Main function to print the entire symbol table
void printSymbolTable(SymbolTable* table) {
    printf("\n");
    printf("+-----------------+-----------+-------------------------------+\n");
    printf("| Name            | Category  | Details                       |\n");
    printf("+-----------------+-----------+-------------------------------+\n");

    Symbol* current;

    // Print variables
    current = table->variables;
    while (current) {
        printVariableSymbol(current);
        current = current->next;
    }

    // Print constants
    current = table->constants;
    while (current) {
        printConstantSymbol(current);
        current = current->next;
    }

    // Print functions
    current = table->functions;
    while (current) {
        printFunctionSymbol(current);
        current = current->next;
    }

    // Print keywords
    current = table->keywords;
    while (current) {
        printKeywordSymbol(current);
        current = current->next;
    }

    printf("+-----------------+-----------+-------------------------------+\n");
    printf("Total Symbols: %d\n", table->totalSize);
    printf("\n");
}
