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
    tDLList instList;
    DLInitList(&instList);

    tInstr instruction;

    instruction.instType = 1;
    DLInsertLast(&instList, instruction);

    instruction.instType = 2;
    DLInsertLast(&instList, instruction);

    instruction.instType = 3;
    DLInsertLast(&instList, instruction);

    instruction.instType = 0;
    DLInsertFirst(&instList, instruction);


    DLFirst(&instList);
    while ( DLActive(&instList) ) {
        DLCopy(&instList, &instruction);
        printf("%d\n", instruction.instType);
        DLSucc(&instList);
    }

    return error_code;
}