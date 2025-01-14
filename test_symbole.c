#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Table_symbole.h"
#include "Table_symbole.c"

// Fonction utilitaire pour afficher un symbole
void logSymbol(Symbol* symbol) {
    if (!symbol) {
        printf("[LOG] Symbol is NULL.\n");
        return;
    }
    printf("[LOG] Symbol: \n");
    printf("  Name: %s\n", symbol->name);
    printf("  Category: %d\n", symbol->category);

    if (symbol->category == VARIABLE_SYMBOL ) {
        printf("  Type: VARIABLE\n");
        printf("  DataType: %d\n", symbol->details.variable.dataType);
        printf("  IsParameter: %s\n", symbol->details.variable.isParameter ? "true" : "false");
        printf("  Value: %s\n", symbol->details.variable.value);
    } else if (symbol->category == FUNCTION_SYMBOL) {
        printf("  Type: FUNCTION\n");
        printf("  ReturnType: %d\n", symbol->details.function.returnType);
        printf("  ParamCount: %d\n", symbol->details.function.paramCount);
    } else if (symbol->category == CONSTANT_SYMBOL) {
        printf("  Type: CONSTANT\n");
        printf("  Value: %s\n", symbol->details.constant.value);
    } else if (symbol->category == KEYWORD_SYMBOL) {
        printf("  Type: KEYWORD\n");
        printf("  Keyword Value: %s\n", symbol->details.keyword.keywordValue);
    }
    if (symbol->scope) {
        printf("  Scope: %s (ID: %d)\n", symbol->scope->name, symbol->scope->scopeId);
    }
}

int main() {
    printf("[TEST] Initializing tests for symbol table...\n");

    // Step 1: Create a global scope
    printf("[TEST] Creating global scope...\n");
    Scope* globalScope = createScope("global", 1, NULL);
    if (globalScope) {
        printf("[LOG] Global scope created: %s (ID: %d)\n", globalScope->name, globalScope->scopeId);
    } else {
        fprintf(stderr, "[ERROR] Failed to create global scope.\n");
        return EXIT_FAILURE;
    }

    // Step 2: Initialize the symbol table
    printf("[TEST] Initializing symbol table...\n");
    SymbolTable table = {NULL, NULL, NULL, NULL, 0};

    // Step 3: Add a variable to the global scope
    printf("[TEST] Adding variable to the global scope...\n");
    Symbol* varSymbol = createSymbol("x", VARIABLE_SYMBOL, globalScope, false, DATA_TYPE_INT, DATA_TYPE_VOID, "10");
    insertSymbol(&table, varSymbol);
    logSymbol(varSymbol);

    // Step 4: Add a constant to the global scope
    printf("[TEST] Adding constant to the global scope...\n");
    Symbol* constSymbol = createSymbol("PI", CONSTANT_SYMBOL, globalScope, false, DATA_TYPE_FLOAT, DATA_TYPE_VOID, "3.14");
    insertSymbol(&table, constSymbol);
    logSymbol(constSymbol);

    // Step 5: Add a function to the global scope
    printf("[TEST] Adding function to the global scope...\n");
    Symbol* funcSymbol = createSymbol("add", FUNCTION_SYMBOL, globalScope, false, DATA_TYPE_INT, DATA_TYPE_INT, NULL);
    funcSymbol->details.function.paramCount = 2;
    insertSymbol(&table, funcSymbol);
    logSymbol(funcSymbol);

    // Step 6: Add a keyword
    printf("[TEST] Adding keyword to the table...\n");

    Symbol* keywordSymbol = createSymbol("if", KEYWORD_SYMBOL, globalScope, false, DATA_TYPE_VOID, DATA_TYPE_VOID, "if");
    insertSymbol(&table, keywordSymbol);
    logSymbol(keywordSymbol);

    // Step 7: Find symbols in the table
    printf("[TEST] Searching for symbols in the global scope...\n");
    Symbol* foundVar = findSymbol(&table, "x", globalScope);
    printf("[LOG] Found variable: \n");
    logSymbol(foundVar);

    Symbol* foundConst = findSymbol(&table, "PI", globalScope);
    printf("[LOG] Found constant: \n");
    logSymbol(foundConst);

    Symbol* foundFunc = findSymbol(&table, "add", globalScope);
    printf("[LOG] Found function: \n");
    logSymbol(foundFunc);

    Symbol* foundKeyword = findSymbol(&table, "if", globalScope);
    printf("[LOG] Found keyword: \n");
    logSymbol(foundKeyword);

    // Step 8: Test inserting duplicate symbol
    printf("[TEST] Trying to add a duplicate symbol...\n");
    insertSymbolIfNotExists(&table, "x", VARIABLE_SYMBOL, globalScope);

    // Step 9: Create a child scope
    printf("[TEST] Creating a child scope...\n");
    Scope* childScope = createScope("function_scope", 2, globalScope);
    if (childScope) {
        printf("[LOG] Child scope created: %s (ID: %d, Parent: %s)\n", childScope->name, childScope->scopeId, childScope->parentScope->name);
    } else {
        fprintf(stderr, "[ERROR] Failed to create child scope.\n");
        return EXIT_FAILURE;
    }

    // Step 10: Add a variable to the child scope
    printf("[TEST] Adding variable to the child scope...\n");
    Symbol* childVarSymbol = createSymbol("y", VARIABLE_SYMBOL, childScope, false, DATA_TYPE_FLOAT, DATA_TYPE_VOID, "20.5");
    insertSymbol(&table, childVarSymbol);
    logSymbol(childVarSymbol);

    // Step 11: Search for a symbol in the parent scope from the child scope
    printf("[TEST] Searching for parent scope variable from child scope...\n");
    Symbol* parentVar = findSymbol(&table, "x", childScope);
    printf("[LOG] Found parent variable: \n");
    logSymbol(parentVar);

     // Step 12: Clean up memory
    printf("[TEST] logging table size...\n");
    printf("total size of the table is %d \n",table.totalSize) ;

    // Step 13: Clean up memory
    printf("[TEST] Cleaning up allocated memory...\n");
    // Free symbols and scopes here (if necessary).

    printf("[TEST] All tests completed successfully.\n");
    printSymbolTable(&table);
    return 0;

}
