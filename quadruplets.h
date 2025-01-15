//  La structure quadruplet sera utilisée comme une liste linéaire chainée
//  dont la tête sera le dernier quadruplet inséré

typedef struct quadruplet quadruplet;

struct quadruplet {
    char operation[20]; // L'opérateur
    char operand_1[20]; // La première opérande
    char operand_2[20]; // La deuxième opérande
    char result[20]; // La variable où le résultat sera stocké
    int qc; // Quadruplet Cursor
    struct quadruplet * next; //    L'adresse de la tête suivante de la liste linéaire chainée
};

//  Machine abstraite des quadruplets : --------------------------------------

quadruplet * createQuadruplet(char op[20], char op1[20], char op2[20], char r[20], int pos);
//  Création d'un quadruplet : operation, operand_1, operand_2, result, qc 
//  Le next de ce quadruplet sera NUL

void insertQuadruplet(quadruplet ** head, char op[], char op1[], char op2[], char r[], int pos);
//  Insertion d'un quadruplet dans une liste dont la tête est le quadruplet head

void updateQuadruplet(quadruplet * quad, char br[20], int pos);
//  Mise à jour de l'opérande 1 (Opérande du branchement) du quadruplet numéro QC

void addQuadruplet(quadruplet ** head, quadruplet * quad, int pos);
//  Ajout d'un quadruplet déjà crée dans une liste dont la tête est le quadruplet head

void printAllQuadruplets(quadruplet * head);
//  Affichage de tous les quadruplets en commençant par la tête head
