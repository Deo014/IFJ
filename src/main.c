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
#include "init.c"
#include "scanner.c"

int main(int argc, char **argv)
{
    // kontrola argumentů
    ERROR_CODE helpStatus = checkArgs(argc, argv);
    if(helpStatus == ERROR_CODE_HELP)
        return code_error = ERROR_CODE_HELP;
    else if(helpStatus == ERROR_CODE_INTERNAL)
        return code_error = ERROR_CODE_INTERNAL;

    // inicializace paměti
    memInit();
    // získání filename z stdin
    getFilename();

    // otevření souboru pro čtení
    ERROR_CODE fileStatus = openFile(filename);
    if(fileStatus == ERROR_CODE_INTERNAL)
        return code_error = ERROR_CODE_INTERNAL;

    /*********************************************
    Tady se vyvolá funkce pro zpracování souboru
     ********************************************/
    code_error = nejakaFunkceZeScanneruNeboParseru();
    /*******************************************/

    // uvolnění paměti
    memFree();
    return code_error;
}