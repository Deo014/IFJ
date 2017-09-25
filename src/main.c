/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   main.c
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :
 * Varianta :
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#include <stdio.h>
#include <stdlib.h>
//#include "symtable.h"
#include "error_code.h"

ERROR_CODE code_error = ERROR_CODE_OK;
FILE *pFile;

int main(int argc, char **argv) {

    // kontrola správného počtu argumentů
    if(argc != 2){
        fprintf(stderr, "Nespravny pocet argumentu. ERROR CODE 99\n");
        return code_error = ERROR_CODE_INTERNAL;
    }

    // otevření souboru pro čtení
    if((pFile = fopen(argv[1], "r")) == NULL){
        fprintf(stderr,"Soubor '%s' se nepodarilo otevrit. ERROR CODE 99\n",argv[1]);
        return code_error = ERROR_CODE_INTERNAL;
    }

    fprintf(stdout,"Soubor '%s' byl otevren a nasledne zavren. ERROR CODE 0\n", argv[1]);

    fclose(pFile);
    return code_error;
}