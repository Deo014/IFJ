/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   stack.c - implementace zásobníku
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#include "stack.h"
unsigned int deletedItems = 0;
unsigned int stackSize = 0;

// Inicializuje zásobník
void SInit (ptrStack* stack) {
    stack->top_of_stack = NULL;
}

// vrátí true pokud je zásobník prázdný
bool SEmpty (ptrStack* stack ) {
    return (stack->top_of_stack == NULL);
}

/*
// vrátí vrchní token na zásobníku
tToken STop (ptrStack* stack) {
    return (stack->top_of_stack->value);
}*/

// Uvolní vrchní prvek ze zásobníku
bool SPop (ptrStack* stack ) {
    tStack *tmp = NULL;
    if(stack->top_of_stack != NULL){
        tmp = stack->top_of_stack;
        stack->top_of_stack = stack->top_of_stack->left;
        free(tmp);
        stackSize--;
        return true;
    }
    return false;
}

// vytvoří nové paměťové místo na zásobníku a vloží do něj data
bool SPush (ptrStack* stack, void * value) {
    tStack *tmp = malloc(sizeof(struct tstack_structure));
    if(tmp != NULL) {
        tmp->value = value;
        tmp->left = stack->top_of_stack;
        stack->top_of_stack = tmp;
        stackSize++;
        return true;
    }
    return false;
}

// uvolnění postupně celý zásobník
bool SDispose(ptrStack *stack){
    while(stack->top_of_stack != NULL){
        SPop(stack);
        deletedItems++;
    }
    return true;
}