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
#include "string.h"
#include <stdlib.h>
#include <stdio.h>

/* ----------------------------------------FUNKCE PRO PRACI SE SYMTABLE-----------------------------------------------*/

/*
 * Inicialiace symtable
 */
void symTableInit(tSymtable* Table) {
    BSTInit(&(Table->root));
}


/*
 * Vlozeni dat o funkci do symtable
 */
void symTableInsertFunction(tSymtable* Table, string Key) { // vlozi do symtable zaznam s klicem Key a data dataPtr
    // alokovani pameti pro data
    tDataFunction * dataPtr;
    if ( (dataPtr = (tDataFunction*)malloc(sizeof(tDataFunction))) == NULL ) {
        return;
    }
    // inicializace dat
    string parameters;
    stringInit(&parameters);
    dataPtr->returnDataType = -1;
    dataPtr->declared = false;
    dataPtr->defined = false;
    dataPtr->parameters = parameters;
    // vytvoreni nove polozky v symtable
    BSTInsert(&(Table->root), Key.value, dataPtr, ndtFunction);
}

/*
 * Vlozeni dat o promenne do symtable
 */
void symTableInsertVariable(tSymtable* Table, string Key) { // vlozi do symtable zaznam s klicem Key a data dataPtr
    // alokovani pameti pro data
    tDataVariable * dataPtr;
    if ( (dataPtr = (tDataVariable*)malloc(sizeof(tDataVariable))) == NULL ) {
        return;
    }
    dataPtr->dataType = -1;
    // vytvoreni nove polozky v symtable
    BSTInsert(&(Table->root), Key.value, dataPtr, ndtVariable);
}

/*
 * Vyhledani prvku v symtable
 */
tBSTNodePtr symTableSearch(tSymtable* Table, string Key) { // vraci ukazatel na hledany uzel, pokud nenajde vraci NULL
    return BSTSearch(Table->root, Key.value);
}

/*
 * Smazani prvku ze symtable
 */
void symTableDelete(tSymtable* Table, string Key) {
    BSTDelete(&(Table->root), Key.value);
}

/*
 * Smazani cele symtable
 */
void symTableDispose(tSymtable* Table) {
    BSTDispose(&(Table->root));
}