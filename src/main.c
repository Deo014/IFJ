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
#include "scanner.h"
#include "string.h"

int main(int argc, char **argv)
{
    ERROR_CODE error_code = ERROR_CODE_OK;
    tToken token; // promenna token
    //stringInit(&token.s); // inicializace tokenu

    // kontrola argumentů
    ERROR_CODE helpStatus = checkArgs(argc, argv);
    if(helpStatus == ERROR_CODE_HELP)
        return error_code = ERROR_CODE_HELP;
    else if(helpStatus == ERROR_CODE_INTERNAL)
        return error_code = ERROR_CODE_INTERNAL;



    while (1) {
        token = getNextToken();
        printf("%2d %s\n", token.type, token.atr.value);
        //printf("length:%d\n", token.atr.length);
        //printf("allocated:%d\n\n", token.atr.lengthAllocated);

        //if (token.type == 30 /* EOF */)
        //    break;
    }



    /*
    // inicializace paměti
    memInit();
    // získání filename z stdin
    getFilename();

    // otevření souboru pro čtení
    ERROR_CODE fileStatus = openFile(filename);
    if(fileStatus == ERROR_CODE_INTERNAL)
        return code_error = ERROR_CODE_INTERNAL;

    // Tady se vyvolá funkce pro zpracování souboru

    //code_error = nejakaFunkceZeScanneruNeboParseru();

    // uvolnění paměti
    memFree(filename);
     */
    return error_code;
}