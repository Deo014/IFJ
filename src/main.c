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

    string str;
    stringInit(&str);
    stringAddChar(&str, 's');

    string inte;
    stringInit(&inte);
    stringAddChar(&inte, 'i');

    string dou;
    stringInit(&dou);
    stringAddChar(&dou, 'd');
    /* testovaci kod zacatek*/
/*
    tToken token;
    while ( (token = getNextToken()).type != sLexError ) {
        if (token.type == sIdentificator) {
            symTableInsertVariable(&glSymTable, token.atr, createDataVariable(token.atr, sInteger) );
            Print_tree(glSymTable.root);
        }
    }

//    symTableInsertFunction(&glSymTable, "klic1", createDataFunction("becko",sDouble, false, false));

//    symtableInsert(&glSymTable, "klic3", createDataVariable("ccko", sInteger));

    //Print_tree(glSymTable.root);
    //tBSTNodePtr node = symTableSearch(&glSymTable, "klic");
    //printf("%s %d %d\n", ((tDataFunction*)node->Data)->name, ((tDataFunction*)node->Data)->return_data_type, ((tDataFunction*)node->Data)->declared);

*/
    /* testovaci kod konec*/

    symTableInsertVariable(&glSymTable, str, createDataVariable(str, sString) );
    symTableInsertVariable(&glSymTable, inte, createDataVariable(inte, sInteger) );
    symTableInsertVariable(&glSymTable, dou, createDataVariable(dou, sDouble) );
    int er;
    /*
    bool kont = true;

    while (kont) {*/
         er = expression(sString);

        switch (er) {
            case ERROR_CODE_OK:
                printf("OK\n");
                break;
            case ERROR_CODE_SYN:
                printf("Syntaxe\n");
                break;
            case ERROR_CODE_SEM_COMP:
                printf("Semantika\n");
                break;
            case ERROR_CODE_LEX:
                printf("Lexikalní error");
                //kont = false;
                break;
        }
    //}

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