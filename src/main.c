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

int main(int argc, char **argv)
{
    ERROR_CODE error_code = ERROR_CODE_OK;

    // kontrola argumentů
    ERROR_CODE helpStatus = checkArgs(argc, argv);
    if(helpStatus == ERROR_CODE_HELP)
        return error_code = ERROR_CODE_HELP;
    else if(helpStatus == ERROR_CODE_INTERNAL)
        return error_code = ERROR_CODE_INTERNAL;


    tSymtable table;
    symTableInit(&table);
//    tToken token;
//    while ( (token = getNextToken()).type != sEndOfFile ) {
//        if (token.type == sIdentificator) {
//            symTableInsertVariable(&table, token.atr.value, createDataVariable(token.atr.value, sInteger) );
//            Print_tree(table.root);
//        }
//    }
//
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

    return error_code;
}