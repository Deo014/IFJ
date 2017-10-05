/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   init.h
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

#include <string.h>
#include "error_code.h"

ERROR_CODE code_error = ERROR_CODE_OK;
FILE *pFile;
char filename[200];
char *newline;

// deklarace fukncí
void printHelp();
void memInit();
void memFree();
ERROR_CODE checkArgs(int, char**);
ERROR_CODE getFilename();
ERROR_CODE openFile(char*);

// kontrola argumentů, metoda printHelp()
ERROR_CODE checkArgs(int argc, char **argv){
    if(argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0 ) ){
        printHelp();
        return ERROR_CODE_HELP;
    }else if(argc > 1){
        fprintf(stderr, "Chyba zpracovani argumentu: ERROR CODE X\n");
        return ERROR_CODE_INTERNAL;
    }
    return ERROR_CODE_OK;
}

// inicializace paměti
ERROR_CODE memInit(){
    bool errors = false;

    printf("Memory initialization.\n");

    if(errors)
        return ERROR_CODE_INTERNAL;
    else
        return ERROR_CODE_OK;
}

// získá filename z stdin, odstaní z něj '\n'
ERROR_CODE getFilename(){
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Input error: ERROR CODE 99\n");
        return ERROR_CODE_INTERNAL;
    }
    newline = strchr(filename, '\n');
    if (newline)
        *newline = '\0';
    return ERROR_CODE_OK;
}

// kontrola platnosti a otevření souboru
ERROR_CODE openFile(char *filename){
    if ((pFile = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Soubor '%s' se nepodarilo otevrit: ERROR CODE %d\n", filename, ERROR_CODE_INTERNAL);
        return ERROR_CODE_INTERNAL;
    }
    return ERROR_CODE_OK;
}

// uvolnění paměti
ERROR_CODE memFree(char *filename){
    bool errors = false;
    if(fclose(pFile) == EOF) {
        printf(stderr, "Soubor %s se nepodarilo uzavrit: ERROR CODE %d\n", filename, ERROR_CODE_INTERNAL);
        errors = true;
    }

    printf("\nMemory cleaning.\n");

    if(errors)
        return ERROR_CODE_INTERNAL;
    else
        return ERROR_CODE_OK;
}

// nápověda k programu
void printHelp(){
    printf("help help help help\n");
}

#endif //IFJ_INIT_H
