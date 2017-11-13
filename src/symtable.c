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

void symTableInit(tSymtable* table) {
    BSTInit(&(table->root));
}

//void symTableInsertFunction(symTablePtr table, tFunctionData function, char K) {
//    BSTInsert(table, K, function);
//}

void symTableInsertVariable(tSymtable* table, char K, tDataVariable* variable) {
    BSTInsert(&(table->root), K, variable);
}

// alokuje novou datovou polozku pro data promenne a vrati ukazatel
tDataVariable *createDataVariable(char* name, char data_type) {
    tDataVariable * newitem;
    if ( (newitem = (tDataVariable*)malloc(sizeof(tDataVariable))) == NULL ) {
        return NULL;
    }
    newitem->name = name;
    newitem->data_type = data_type;
    return newitem;
}

tBSTNodePtr symTableSearch(tSymtable* table, char K) {
    return BSTSearch(table->root, K);
}