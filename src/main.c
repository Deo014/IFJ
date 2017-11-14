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
#include "expression.h"

tSymtable glSymTable; // globalni tabulka symbolu
tDLListInstruction instList; // globalni list vygenerovanych instrukci (instrukcni paska)

int main(int argc, char **argv)
{
    ERROR_CODE result_code = ERROR_CODE_OK; // vysledny kod programu (pokud preklad probehne v poradku, hodnota bude ERROR_CODE_OK)

    /*----------kontrola poctu argumentu----------*/
    if (argc > 1) {
        fprintf(stderr, "Program \"%s\" nesmi byt volan s parametry.\n", *argv);
        return ERROR_CODE_INTERNAL;
    }

    /*----------inicializace----------*/
    symTableInit(&glSymTable); // globalni tabulka symbolu
    DLInitList(&instList);  // instrukcni paska

/*
    tToken token;
    while ( (token = getNextToken()).type != sEndOfFile ) {
        if (token.type == sIdentificator) {
            symTableInsertVariable(&table, token.atr.value, createDataVariable(token.atr.value, sInteger) );
            Print_tree(table.root);
        }
    }

    symTableInsertVariable(&table, "klic5", createDataVariable("acko", sInteger) );
    symTableInsertFunction(&table, "klic3", createDataFunction("becko",sDouble, false, false));
    symTableInsertVariable(&table, "klic4", createDataVariable("ccko", sInteger) );
    symTableInsertVariable(&table, "klic7", createDataVariable("ccko", sInteger) );
    symTableInsertVariable(&table, "klic6", createDataVariable("ccko", sInteger) );
    symTableInsertVariable(&table, "klic8", createDataVariable("ccko", sInteger) );

    symTableDelete(&table, "klic7");
    symTableDelete(&table, "klic5");


    tBSTNodePtr node = symTableSearch(&table, "klic3");

    printf("%s %d %d\n", ((tDataFunction*)node->Data)->name, ((tDataFunction*)node->Data)->return_data_type, ((tDataFunction*)node->Data)->declared);

    Print_tree(table.root);
*/
    /*----------Syntakticka analyza, Semanticka analyza, Generovani 3AK----------*/
    //result_code = parse();

    /*----------vypsani instrukcni pasky na stdout----------*/
    if (result_code == ERROR_CODE_OK) // instrukcni paska se vypise na stdout pouze pokud preklad probehl v poradku
        printInstructionList(&instList);

    /*----------uvolneni alokovane pameti----------*/
    symTableDispose(&glSymTable); // globalni tabulka symbolu
    DLDisposeList(&instList); // insturkcni paska

    return result_code;
}