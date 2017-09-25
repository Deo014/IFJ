/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   error_code.h - Hlavničkový soubor s typy možných chyb
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :
 * Varianta :
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#ifndef IFJ_ERROR_CODE_H
#define IFJ_ERROR_CODE_H

#include <stdio.h>

typedef enum error_code{
    ERROR_CODE_OK = 0,  // chyba v rámci lexikální analýzy
    ERROR_CODE_SYN = 1, // chyba v rámci syntaktické analýzy
    ERROR_CODE_SEM_UNDEFINED  = 2, // nedefinová funkce/proměnná
    ERROR_CODE_SEM_TYPE = 4,    // sémantická chyba typové kompatibility v operacích
    ERROR_CODE_SEM_OTHER = 6,   // ostatní sémantické chyby
    ERROR_CODE_INTERNAL = 99    // interní chyba překladače neovlivněná vstupním programem

} ERROR_CODE;

#endif //IFJ_ERROR_CODE_H


