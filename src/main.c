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

    //generateInstruction(&instList, I_DEFVAR, "variable", NULL, NULL);
    /*----------vypsani instrukcni pasky na stdout----------*/

    //*******************************************
    Instr_element test_element1; Instr_element test_element2; Instr_element test_element3; Instr_element test_element4;
    string test_string1; stringInit(&test_string1); stringAddChar(&test_string1, 'a'); test_element1.frame = F_LF;
    string test_string2; stringInit(&test_string2); stringAddChar(&test_string2, '4');
    string test_string3; stringInit(&test_string3); stringAddChar(&test_string3, 'b'); test_element3.frame = F_TF;
    string test_string4; stringInit(&test_string4); test_element4.value.value = "Scope";
    test_element1.token_type = sIdentificator; test_element1.value = test_string1; test_element1.isLabel = false; test_element1.isScope = false;
    test_element2.token_type = sDouble; test_element2.value = test_string2;test_element2.isLabel = false; test_element2.isScope = false;
    test_element3.token_type = sIdentificator; test_element3.value = test_string3; test_element3.isLabel = false; test_element3.isScope = false;
    test_element4.token_type = sKeyWord; test_element4.value = test_string4; test_element4.isLabel = true; test_element4.isScope = true;

    writeInstructionOneOperand(&instList, I_LABEL, test_element4);
    writeInstructionNoOperand(&instList, I_PUSHFRAME);
    writeInstructionOneOperand(&instList, I_DEFVAR, test_element1);
    writeInstructionOneOperand(&instList, I_WRITE, test_element2);
    writeInstructionTwoOperands(&instList, I_MOVE, test_element1, test_element2);
    test_element4.value.value = "jmeno"; test_element4.isLabel = true; test_element4.isScope = false;
    writeInstructionOneOperand(&instList, I_LABEL, test_element4);
    writeInstructionTwoOperands(&instList, I_MOVE, test_element1, test_element3);
    writeInstructionThreeOperands(&instList, I_ADD, test_element1, test_element2, test_element3);

    //generateInstruction(&instList, I_DEFVAR, &test_element1, NULL, NULL);


    //if (result_code == ERROR_CODE_OK) // instrukcni paska se vypise na stdout pouze pokud preklad probehl v poradku
        printInstructionList(&instList);

    /*----------uvolneni alokovane pameti----------*/
    symTableDispose(&glSymTable); // globalni tabulka symbolu
    DLDisposeList(&instList); // insturkcni paska


    return result_code;
}