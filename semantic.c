#include "semantic.h"
#include "Table_symbole.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Function to convert an expression's value to a string
void getStringValue(expression exp, char* valeur) {
    switch (exp.type) {
        case DATA_TYPE_INT:
            sprintf(valeur, "%d", exp.integerValue); // Convert integer to string
            break;
        case DATA_TYPE_FLOAT:
            sprintf(valeur, "%.4f", exp.floatValue); // Convert float to string (4 decimal places)
            break;
        case DATA_TYPE_STRING:
            sprintf(valeur, "%s", exp.stringValue); // Copy string value
            break;
        case DATA_TYPE_BOOLEAN:
            sprintf(valeur, "%s", exp.booleanValue ? "true" : "false"); // Convert boolean to "true" or "false"
            break;
        default:
            strcpy(valeur, "unknown"); // Handle unknown types (e.g., DATA_TYPE_OBJECT, DATA_TYPE_ARRAY)
            break;
    }
}