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

void symTableInit(tSymtable*);

void symTableInsertVariable(tSymtable*, char*, tDataVariable*);
tDataVariable *createDataVariable(char*, int);

void symTableInsertFunction(tSymtable*, char*, tDataFunction*);
tDataFunction *createDataFunction(char*, int, bool, bool);

tBSTNodePtr symTableSearch(tSymtable*, char*);

void symTableDispose(tSymtable*);
void symTableDelete(tSymtable*, char*);
#endif //IFJ_SYMTABLE_H
