/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   init.h - syntaktický a sémantický analyzátor
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#ifndef IFJ_INIT_H
#define IFJ_INIT_H

#include "error_code.h"

// deklarace fukncí
void printHelp();
ERROR_CODE memInit();
ERROR_CODE memFree();
ERROR_CODE checkArgs(int, char**);
ERROR_CODE getFilename();
ERROR_CODE openFile(char*);

#endif //IFJ_INIT_H
