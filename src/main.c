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

tInstrOperand operand1; tInstrOperand operand2; tInstrOperand operand3;
string TMP;

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
    string test_string1; stringInit(&test_string1); stringAddChar(&test_string1, 'a');
    string test_string2; stringInit(&test_string2); stringAddChar(&test_string2, '4');
    string test_string3; stringInit(&test_string3); stringAddChar(&test_string3, 'b');

    operand1.token_type = sKeyWord; operand1.value.value = "Scope"; operand1.isLabel = true; operand1.isScope = true;
    writeInstructionOneOperand(&instList, I_LABEL, operand1);
    writeInstructionNoOperand(&instList, I_CREATEFRAME);
    writeInstructionNoOperand(&instList, I_PUSHFRAME);

    operand1.token_type = sKeyWord; operand1.value.value = "nejakylabel"; operand1.isLabel = true; operand1.isScope = false;
    writeInstructionOneOperand(&instList, I_LABEL, operand1);

    operand2.token_type = sDouble; operand2.value = test_string2;operand2.isLabel = false; operand2.isScope = false;
    operand3.token_type = sIdentificator; operand3.value = test_string3; operand3.isLabel = false; operand3.isScope = false;

    operand1.token_type = sIdentificator; operand1.value = test_string1; operand1.isLabel = false; operand1.isScope = false; operand1.isTMP = false;
    writeInstructionOneOperand(&instList, I_DEFVAR, operand1);
    operand1.value = test_string3;
    writeInstructionOneOperand(&instList, I_WRITE, operand1);
    operand2.token_type = sInteger; operand2.value.value = "4";
    writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);
    operand1.isTMP = true;
    writeInstructionThreeOperands(&instList, I_ADD, operand1, operand2, operand3);

    /* TMP */ operand1.token_type = sIdentificator; operand1.isTMP = true; operand1.isLabel = false; operand1.isScope = false; operand1.frame = F_LF;
    operand2.value.value = ""; operand2.token_type = sIdentificator; operand2.isTMP = true; operand2.isLabel = false; operand2.isScope = false; operand2.frame = F_LF;
    operand3.value.value = ""; operand3.token_type = sIdentificator; operand3.isTMP = true; operand3.isLabel = false; operand3.isScope = false; operand3.frame = F_LF;
    writeInstructionOneOperand(&instList, I_DEFVAR, operand1);
    writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);
    writeInstructionThreeOperands(&instList, I_ADD, operand1, operand2, operand3);
    operand1.value = test_string1; operand1.isTMP = false;
    writeInstructionThreeOperands(&instList, I_ADD, operand1, operand2, operand3);

    //if (result_code == ERROR_CODE_OK) // instrukcni paska se vypise na stdout pouze pokud preklad probehl v poradku
        printInstructionList(&instList);

    /*----------uvolneni alokovane pameti----------*/
    symTableDispose(&glSymTable); // globalni tabulka symbolu
    DLDisposeList(&instList); // insturkcni paska


    return result_code;
}