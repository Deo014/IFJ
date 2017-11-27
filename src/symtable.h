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

/* ----------------------------------------FUNKCE PRO PRACI SE SYMTABLE-----------------------------------------------*/

typedef struct variable {
    int dataType; /* sInteger, sDouble, sString */
} tDataVariable;

typedef struct function {
    int returnDataType; /* sInteger, sDouble, sString */
    bool declared;
    bool defined;
    string parameters;   /* parametry funkce: napr. isd (integer, string, double) */
    string paramName[10];
} tDataFunction;

typedef struct symtable {
    tBSTNodePtr root;
} tSymtable;

/*
 * Inicialiace symtable
 */
void symTableInit(tSymtable*);


/*
 * Vlozeni dat o funkci do symtable
 */
void symTableInsertFunction(tSymtable*, string);

/*
 * Vlozeni dat o promenne do symtable
 */
void symTableInsertVariable(tSymtable*, string);

/*
 * Vyhledani prvku v symtable
 */
tBSTNodePtr symTableSearch(tSymtable*, string);

/*
 * Smazani prvku ze symtable
 */
void symTableDelete(tSymtable*, string);

/*
 * Smazani cele symtable
 */
void symTableDispose(tSymtable*);



#endif //IFJ_SYMTABLE_H
