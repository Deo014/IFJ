/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   symbtable.h - Implementace tabulky symbolů
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#include "bintree.h"
#include <stdbool.h>
#include "string.h"

#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

typedef struct variable {
    char *name;
    int data_type; // sInteger, sDouble, sString
} tDataVariable;

typedef struct function {
    char *name;
    int return_data_type; // sInteger, sDouble, sString
    bool declared;
    bool defined;
    /* TODO seznam parametru */
} tDataFunction;

typedef struct symtable {
    tBSTNodePtr root;
} tSymtable;

/* --------------------inicialiace symtable-------------------- */
void symTableInit(tSymtable*);


void symtableInsert(tSymtable* Table, string Key, void* dataPtr);

/* --------------------vlozeni dat o funkci do symtable--------------------*/
void symTableInsertFunction(tSymtable*, string, tDataFunction*);
tDataFunction *createDataFunction(string, int, bool, bool);

/* --------------------vlozeni dat o promenne do symtable--------------------*/
void symTableInsertVariable(tSymtable*, string, tDataVariable*);
tDataVariable *createDataVariable(string, int);

/* --------------------vyhledani prvku v symtable--------------------*/
tBSTNodePtr symTableSearch(tSymtable*, string);

/* --------------------smazani prvku v symtable--------------------*/
void symTableDelete(tSymtable*, string);

/* --------------------smazani cele symtable--------------------*/
void symTableDispose(tSymtable*);
#endif //IFJ_SYMTABLE_H
