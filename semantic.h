#include <stdbool.h>
#include "Table_symbole.h"


typedef struct expression expression;
struct expression{
DataType type;
char stringValue[255];
int integerValue;
double floatValue;
bool booleanValue;
};

// typedef struct variable variable;
// struct variable{

//         struct Symbol* symbol;
// };

void getStringValue(expression exp, char* valeur);