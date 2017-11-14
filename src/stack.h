/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   stack.h - deklarace zásobníku
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#ifndef IFJ_STACK_H
#define IFJ_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error_code.h"
#include "scanner.h"

// Pomocné proměnné pro účely testování


// Struktura prvku na zásobníku
typedef struct tstack_structure{
    struct tstack_structure *left;
    void *value;
} tStack;

// Struktura zásboníku
typedef struct {
    tStack *top_of_stack;
} ptrStack;

// Deklarace funkcí pro zásobník
void SInit(ptrStack *);
bool SPush(ptrStack *, void *);
bool SPop(ptrStack *);
//tToken STop(ptrStack *, void *);
tToken STop(ptrStack *);
bool SEmpty(ptrStack *);
bool SDispose(ptrStack *);

#endif //IFJ_STACK_H