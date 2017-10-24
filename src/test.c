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
#include <string.h>
//#include "symtable.h"
#include "error_code.h"

ERROR_CODE code_error = ERROR_CODE_OK;

void printHelp();

int main(int argc, char **argv)
{
    FILE *pFile;
    char filename[200];
    char *newline;

    if(argc == 1) {
        if (fgets(filename, sizeof(filename), stdin) == NULL) {
            fprintf(stderr, "Input error: ERROR CODE 99\n");
            return code_error = ERROR_CODE_INTERNAL;
        }
        newline = strchr(filename, '\n');
        if (newline)
            *newline = '\0';
    }else if(argc == 2){
        filename = argv[1];
    }else{
        printHelp();
        return code_error;
    }

    if ((pFile = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Soubor '%s' se nepodarilo otevrit: ERROR CODE 99\n", filename);
        return code_error = ERROR_CODE_INTERNAL;
    }

    printf("sfasdfasfasffa\n");
    fclose(pFile);
    return code_error;
}

void printHelp(){
    printf("help help help help\n");
}