/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   error_code.h - Hlavničkový soubor s typy možných chyb
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#ifndef IFJ_ERROR_CODE_H
#define IFJ_ERROR_CODE_H

#include <stdio.h>

typedef enum error_code{
    ERROR_CODE_OK = 0,  // preklad probehl bez chyby
    ERROR_CODE_LEX = 1, // chyba v ramci lexikalni analyzy
    ERROR_CODE_SYN  = 2, // chyba v ramci syntakticke analyzy
    ERROR_CODE_SEM = 3, //semanticka chyba  sémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefinici funkce/proměnné, atd.
    ERROR_CODE_SEM_COMP = 4,    // semanticka chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech, příp. špatný počet či typ parametrů u volání funkce
    ERROR_CODE_SEM_OTHER = 6,   // ostatní sémantické chyby
    ERROR_CODE_HELP = 7,
    ERROR_CODE_INTERNAL = 99,    // interní chyba překladače neovlivněná vstupním programem (napr. chyba alokace pameti)

    ERROR_CODE_FALSE = 0,
    ERROR_CODE_TRUE = 100
} ERROR_CODE;

#endif //IFJ_ERROR_CODE_H


