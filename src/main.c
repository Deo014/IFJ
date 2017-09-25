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
    // code_error = nejakaFunkceZeScanneruNeboParseru();
    int chr;
    while ((chr = fgetc(pFile)) != EOF)
        fputc(chr, stdout);
    /********************************************
     * *****************************************/

    // uvolnění paměti
    memFree();
    return code_error;
}

// kontrola platnosti a otevření souboru
ERROR_CODE openFile(char *filename){
    if ((pFile = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Soubor '%s' se nepodarilo otevrit: ERROR CODE 99\n", filename);
        return code_error = ERROR_CODE_INTERNAL;
    }
    return code_error = ERROR_CODE_OK;
}

// získá filename z stdin, odstaní z něj '\n'
ERROR_CODE getFilename(){
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Input error: ERROR CODE 99\n");
        return code_error = ERROR_CODE_INTERNAL;
    }
    newline = strchr(filename, '\n');
    if (newline)
        *newline = '\0';
    return code_error = ERROR_CODE_OK;
}

// inicializace paměti
void memInit(){
    printf("Memory initialization.\n");
}

// kontrola argumentů, metoda printHelp()
ERROR_CODE checkArgs(int argc, char **argv){
    if(argc == 2 && (strcmp(argv[1], "-h") ==0 || strcmp(argv[1], "--help")==0 ) ){
        printHelp();
        return code_error = ERROR_CODE_HELP;
    }else if(argc > 1){
        fprintf(stderr, "Chyba zpracovani argumentu: ERROR CODE X\n");
        return code_error = ERROR_CODE_INTERNAL;
    }
    return code_error = ERROR_CODE_OK;
}

// uvolnění paměti
void memFree(){
    fclose(pFile);
    printf("\nMemmory cleaning.\n");
}

// nápověda k programu
void printHelp(){
    printf("help help help help\n");
}