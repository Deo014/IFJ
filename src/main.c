/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   main.c
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#include <stdio.h>
//#include <stdlib.h>
#include "error_code.h"
#include "init.h"
#include "string.h"
#include "instList.h"

int main(int argc, char **argv)
{
    ERROR_CODE error_code = ERROR_CODE_OK;
    //stringInit(&token.s); // inicializace tokenu

    // kontrola argumentů
    ERROR_CODE helpStatus = checkArgs(argc, argv);
    if(helpStatus == ERROR_CODE_HELP)
        return error_code = ERROR_CODE_HELP;
    else if(helpStatus == ERROR_CODE_INTERNAL)
        return error_code = ERROR_CODE_INTERNAL;


    // otestovani vkladani instrukci do instrukcniho seznamu--SMAZAT
    tDLListInstruction instList;
    DLInitList(&instList);

    generateInstruction(&instList, 1, NULL, NULL, NULL);
    generateInstruction(&instList, 2, NULL, NULL, NULL);
    generateInstruction(&instList, 3, NULL, NULL, NULL);

    printInstructionList(&instList);

    return error_code;
}