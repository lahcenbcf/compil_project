#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quadruplets.h"

quadruplet * createQuadruplet(char op[20], char op1[20], char op2[20], char r[20], int pos) {
    quadruplet *quad = (quadruplet *)malloc(sizeof(quadruplet));
    strcpy(quad->operation, op);
    strcpy(quad->operand_1, op1);
    strcpy(quad->operand_2, op2);
    strcpy(quad->result, r);
    quad->qc = pos;
    quad->next = NULL;
    return quad;
}

void insertQuadruplet(quadruplet **head, char op[], char op1[], char op2[], char r[], int pos) {
    if (*head == NULL)
    {
        *head = createQuadruplet(op, op1, op2, r, pos); 
    }
    else {
        quadruplet *quad = (quadruplet *)malloc(sizeof(quadruplet));
        strcpy(quad->operation, op);
        strcpy(quad->operand_1, op1);
        strcpy(quad->operand_2, op2);
        strcpy(quad->result, r);
        quad->qc = pos;
        quad->next = *head;
        *head = quad;
    }
}

void updateQuadruplet(quadruplet * quad, char br[20], int pos) {
    quadruplet *q = quad;
    if (q == NULL)
    {
        return;
    }
    while (q != NULL)
    {
        if (q->qc == pos)
        {
            strcpy(q->operand_1, br);
            return;
        }
        q = q->next;
    }
    
    
}

void addQuadruplet(quadruplet ** head, quadruplet * quad, int pos) {
    if (quad == NULL)
    {
        return;
    }
    if (head != NULL)
    {
        quad->qc = pos;
        quad->next = *head;
    }
    *head = quad;
}

void printAllQuadruplets(quadruplet * head) {
    quadruplet * quad;
    quad = head;
    printf("\n ------------------------ AFFICHAGE QUADRUPLETS --------------------------\n");
    printf("-------------------------------------------------- \n \n");
    if (quad == NULL) {
        printf("\t \t LISTE DE QUADRUPLETS VIDE. \n \n");
    }
    while (quad != NULL)
    {
        printf("\t %d - ( %s , %s , %s , %s ) \n",quad->qc,quad->operation,quad->operand_1,quad->operand_2,quad->result);
        quad = quad->next;
    }
    printf("-------------------------------------------------- \n \n");
}
