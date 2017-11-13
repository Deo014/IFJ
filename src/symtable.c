/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   symbtable.c - Implementace tabulky symbolů
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#include "symtable.h"
#include <stdlib.h>
#include <stdio.h>

// inicialiace symtable
void symTableInit(tSymtable* table) {
    BSTInit(&(table->root));
}


// vlozi do symtable zaznam s klicem Key a data dataPtr
void symTableInsertFunction(tSymtable* Table, char* Key, tDataFunction* dataPtr) {
    BSTInsert(&(Table->root), Key, dataPtr);
}
// alokuje novou datovou polozku pro data funkce a vrati ukazatel
tDataFunction *createDataFunction(char* name, int return_data_type, bool declared, bool defined) {
    tDataFunction * newitem;
    if ( (newitem = (tDataFunction*)malloc(sizeof(tDataFunction))) == NULL ) {
        return NULL;
    }
    newitem->name = name;
    newitem->return_data_type = return_data_type;
    newitem->declared = declared;
    newitem->defined = defined;
    return newitem;
}


// vlozi do symtable zaznam s klicem Key a data dataPtr
void symTableInsertVariable(tSymtable* Table, char* Key, tDataVariable* dataPtr) {
    BSTInsert(&(Table->root), Key, dataPtr);
}
// alokuje novou datovou polozku pro data promenne a vrati ukazatel
tDataVariable *createDataVariable(char* name, int data_type) {
    tDataVariable * newitem;
    if ( (newitem = (tDataVariable*)malloc(sizeof(tDataVariable))) == NULL ) {
        return NULL;
    }
    newitem->name = name;
    newitem->data_type = data_type;
    return newitem;
}


// vraci ukazatel na hledany uzel, pokud nenajde vraci NULL
tBSTNodePtr symTableSearch(tSymtable* Table, char* Key) {
    return BSTSearch(Table->root, Key);
}


void symTableDispose(tSymtable* Table) {
    BSTDispose(&(Table->root));
}

void symTableDelete(tSymtable* Table, char* Key) {
    BSTDelete(&(Table->root), Key);
}

