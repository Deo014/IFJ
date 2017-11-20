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
#include "instList.h"
#include "symtable.h"
#include "string.h"
#include "bintree.h"
#include "expression.h"
#include "parser.h"


tSymtable glSymTable; // globalni tabulka symbolu
tDLListInstruction instList; // globalni list vygenerovanych instrukci (instrukcni paska)

int main(int argc, char **argv)
{
    freopen("test1.txt", "r", stdin);
    ERROR_CODE result_code = ERROR_CODE_OK; // vysledny kod programu (pokud preklad probehne v poradku, hodnota bude ERROR_CODE_OK)

    /*----------kontrola poctu argumentu----------*/
    if (argc > 1) {
        fprintf(stderr, "Program \"%s\" nesmi byt volan s parametry.\n", *argv);
        return ERROR_CODE_INTERNAL;
    }

    /*----------inicializace----------*/
    symTableInit(&glSymTable); // globalni tabulka symbolu
    DLInitList(&instList);  // instrukcni paska

    /* tu kukaj ako jede symtable */
    string jedna;
    stringInit(&jedna);
    stringAddChar(&jedna, 'K');

    string dva;
    stringInit(&dva);
    stringAddChar(&dva, 'L');

    string tri;
    stringInit(&tri);
    stringAddChar(&tri, 'A');


    symTableInsertFunction(&glSymTable, jedna, createDataFunction(sInteger, true, false, "ssid" )); /* prvni parametr misto sInteger muzem zmenit na 'i', stejne jak to je v poslednim parametru */
    symTableInsertFunction(&glSymTable, dva, createDataFunction(sString, true, false, "sd" ));
    symTableInsertVariable(&glSymTable, tri, createDataVariable(sDouble));

    Print_tree(glSymTable.root);
    /* tu prestan kukat */


    /*----------Syntakticka analyza, Semanticka analyza, Generovani 3AK----------*/
    result_code = parse(&glSymTable, &instList);


    /*----------vypsani instrukcni pasky na stdout----------*/
    if (result_code == ERROR_CODE_OK) // instrukcni paska se vypise na stdout pouze pokud preklad probehl v poradku
        printInstructionList(&instList);

    /*----------uvolneni alokovane pameti----------*/
    //symTableDispose(&glSymTable); // globalni tabulka symbolu
    DLDisposeList(&instList); // insturkcni paska


    return result_code;
}