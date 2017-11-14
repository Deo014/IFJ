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
#include "symtable.h"
#include "bintree.h"
#include "expression.h"

tSymtable glSymTable;


int main(int argc, char **argv)
{
    ERROR_CODE error_code = ERROR_CODE_OK;

    // kontrola argumentů
    ERROR_CODE helpStatus = checkArgs(argc, argv);
    if(helpStatus == ERROR_CODE_HELP)
        return error_code = ERROR_CODE_HELP;
    else if(helpStatus == ERROR_CODE_INTERNAL)
        return error_code = ERROR_CODE_INTERNAL;

    symTableInit(&glSymTable);


//    tToken token;
//    while ( (token = getNextToken()).type != sEndOfFile ) {
//        if (token.type == sIdentificator) {
//            symTableInsertVariable(&table, token.atr.value, createDataVariable(token.atr.value, sInteger) );
//            Print_tree(table.root);
//        }
//    }
//
    symTableInsertVariable(&glSymTable, "klic5", createDataVariable("acko", sInteger) );
    symTableInsertFunction(&glSymTable, "klic3", createDataFunction("becko",sDouble, false, false));
    symTableInsertVariable(&glSymTable, "klic4", createDataVariable("ccko", sInteger) );
    symTableInsertVariable(&glSymTable, "klic7", createDataVariable("ccko", sInteger) );
    symTableInsertVariable(&glSymTable, "klic6", createDataVariable("ccko", sInteger) );
    symTableInsertVariable(&glSymTable, "klic8", createDataVariable("ccko", sInteger) );

    symTableDelete(&glSymTable, "klic7");
    symTableDelete(&glSymTable, "klic5");


    tBSTNodePtr node = symTableSearch(&glSymTable, "klic3");

    printf("%s %d %d\n", ((tDataFunction*)node->Data)->name, ((tDataFunction*)node->Data)->return_data_type, ((tDataFunction*)node->Data)->declared);

    Print_tree(glSymTable.root);

    int er = expression(sInteger);

    switch (er){
        case ERROR_CODE_SEM_COMP:
            printf("\nSemanticka chyba");
            break;
        case ERROR_CODE_SYN:
            printf("\nSyntakticka chyba");
            break;
        case ERROR_CODE_OK:
            printf("\nVše OK");
            break;
    }

    return error_code;
}