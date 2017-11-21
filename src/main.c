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
#include "scanner.h"


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


    /* testovaci kod zacatek*/
    //                        funkce
    string ret;
    stringInit(&ret);
    stringAddChar(&ret, 'a');

    symTableInsertFunction(&glSymTable,ret); // zalozeni nove polozku pro funkci v symtable

    tBSTNodePtr node = symTableSearch(&glSymTable, ret); // vyhledani ukazatele na polozku
    tDataFunction* data = (tDataFunction*)(node->Data);

    stringAddChar(&(data->parameters), 'd'); // pripsani znaku do parametru
    data->returnDataType = sDouble; // navratovy typ funkce je double



    string ret3;
    stringInit(&ret3);
    stringAddChar(&ret3, 'd');

    symTableInsertVariable(&glSymTable, ret3); // zalozeni nove polozku pro promenno v symtable

    tBSTNodePtr node3 = symTableSearch(&glSymTable, ret3); // vyhledani ukazatele na polozku
    tDataVariable* data3 = (tDataVariable*)(node3->Data);

    data3->dataType = sDouble;

    string ret4;
    stringInit(&ret4);
    stringAddChar(&ret4, 's');

    symTableInsertVariable(&glSymTable, ret4); // zalozeni nove polozku pro promenno v symtable

    tBSTNodePtr node4 = symTableSearch(&glSymTable, ret4); // vyhledani ukazatele na polozku
    tDataVariable* data4 = (tDataVariable*)(node4->Data);

    data4->dataType = sString;

    //                        promenna
    string ret2;
    stringInit(&ret2);
    stringAddChar(&ret2, 'b');

    symTableInsertVariable(&glSymTable, ret2); // zalozeni nove polozku pro promenno v symtable

    tBSTNodePtr node2 = symTableSearch(&glSymTable, ret2); // vyhledani ukazatele na polozku
    tDataVariable* data2 = (tDataVariable*)(node2->Data);

    data2->dataType = sInteger; // datovy typ promenne je integer


    Print_tree(glSymTable.root);

    result_code = expression(getNextToken(),sDouble);

    /* testovaci kod konec*/


    /*----------Syntakticka analyza, Semanticka analyza, Generovani 3AK----------*/
    //result_code = parse();


    /*----------vypsani instrukcni pasky na stdout----------*/
    if (result_code == ERROR_CODE_OK) // instrukcni paska se vypise na stdout pouze pokud preklad probehl v poradku
        printInstructionList(&instList);

    /*----------uvolneni alokovane pameti----------*/
    //symTableDispose(&glSymTable); // globalni tabulka symbolu
    DLDisposeList(&instList); // insturkcni paska


    return result_code;
}