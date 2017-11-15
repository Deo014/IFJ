/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   expression.h
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#ifndef IFJ_EXPRESSION_H
#define IFJ_EXPRESSION_H

#define PT_SIZE 15

#include "scanner.h"
#include "stack.h"
#include "error_code.h"
#include "string.h"
#include "symtable.h"

/*Datový typ enum, popisující všechny možné znaky ve výrazu*/
typedef enum {

    eMultiply,
    eDivideD,
    eDivideI,
    ePlus,
    eMinus,

    eEqual,
    eNotEqual,
    eLess,
    eLessEqual,
    eMore,
    eMoreEqual,

    eLeftPar,
    eRightPar,

    eOperand,
    eDollar,
    eOther

}eItem;

//Struktura reprezentující (ne)terminál na zásobníku
typedef struct exp_element{
    string value;   //Hodnota prvku
    int pt_index;   //Index (typ) prvku v rámci precedenční tabulky
    int token_type; //Typ tokenu (globálně)
    bool terminal;  //Určuje, zda je prvek terminál
    bool handle;    //Handle pro pomoc při redukci stacku
}Exp_element;


ERROR_CODE expression(int);
ERROR_CODE expressionAnalysis(ptrStack*);
ERROR_CODE initExpressionStack(ptrStack*);
ERROR_CODE shiftToStack(ptrStack*);
ERROR_CODE useRule(ptrStack*);
ERROR_CODE reduceBinary(ptrStack*,int);
ERROR_CODE reducePars(ptrStack*);
ERROR_CODE checkSemAConv(Exp_element*, int, Exp_element*);
Exp_element *newElementToStack(string, int, int);
char getSignFromTable();
int convertTokenToIndex(int);
ERROR_CODE changeOperandType(tStack*,int);

#endif //IFJ_EXPRESSION_H
