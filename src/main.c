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

    symTableInsertVariable(&table, 'a', createDataVariable("acko", sInteger) );
    symTableInsertVariable(&table, 'b', createDataVariable("becko", sInteger) );
    symTableInsertVariable(&table, 'c', createDataVariable("ccko", sInteger) );

    tBSTNodePtr node;
    node = symTableSearch(&table, 'c');

    printf("%s %d\n", ((tDataVariable*)node->Data)->name, ((tDataVariable*)node->Data)->data_type);


    return error_code;
}