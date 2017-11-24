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
#include "inst_list.h"
#include "symtable.h"
#include "string.h"
#include "bintree.h"
#include "expression.h"
#include "scanner.h"

tInstrOperand operand1; tInstrOperand operand2; tInstrOperand operand3;

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

//    tToken token;
//    while ( (token = getNextToken()).type != sLexError ) {
//        if (token.type == sIdentificator) {
//            symTableInsertVariable(&glSymTable, token.atr);
//            Print_tree(glSymTable.root);
//        }
//    }

//    while ( (token = getNextToken()).type != sEndOfFile) {
//        printf("%4d %s\n", token.type, token.atr.value);
//    }

//    result_code = expression(getNextToken(),sDouble);

    /* testovaci kod konec*/


    /*----------Syntakticka analyza, Semanticka analyza, Generovani 3AK----------*/
    //result_code = parse();

    //generateInstruction(&instList, I_DEFVAR, "variable", NULL, NULL);
    /*----------vypsani instrukcni pasky na stdout----------*/

    //*******************************************
    string test_string1; stringInit(&test_string1); stringAddChar(&test_string1, 'a');
    string test_string2; stringInit(&test_string2); stringAddChar(&test_string2, '4');
    string test_string3; stringInit(&test_string3); stringAddChar(&test_string3, 'b');
    // operand,  value,  type,  frame,  isTMP,  isLabel,  isScope,  input, inputtype

    // Vypsání $$scope, CREATEFRAME a PUSHFRAME
    operand1 = initOperand(operand1, "", sKeyWord, F_DEFAULT, false, false, true, I_DEFAULT);
    writeInstructionOneOperand(&instList, I_LABEL, operand1);
    writeInstructionNoOperand(&instList, I_CREATEFRAME);
    writeInstructionNoOperand(&instList, I_PUSHFRAME);

    // vytvoření LABEL $nejakylabel
    operand1 = initOperand(operand1, "nejakylabel", sKeyWord, F_DEFAULT, false, true, false, I_DEFAULT);
    writeInstructionOneOperand(&instList, I_LABEL, operand1);

    // Nahrání double@4 do LF@b
    operand1 = initOperand(operand1, test_string3.value, sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, test_string2.value, sDouble, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);

    // Vytvoření LF@a
    operand1 = initOperand(operand1, test_string1.value, sIdentificator, F_LF, false, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList, I_DEFVAR, operand1);

    // Načtení hodnoty int do LF@a
    operand1 = initOperand(operand1, test_string1.value, sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "", sIdentificator, F_DEFAULT, false, false, false, INPUT_DOUBLE   );
    writeInstructionTwoOperands(&instList, I_READ, operand1, operand2);

    // Vypsání hodnoty LF@a
    operand1 = initOperand(operand1, test_string1.value, sIdentificator, F_LF, false, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList, I_WRITE, operand1);

    // Práce se zásobníkem hodnot
    operand1 = initOperand(operand1, test_string1.value, sIdentificator, F_LF, false, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList, I_PUSHS, operand1);
    writeInstructionOneOperand(&instList, I_POPS, operand1);

    // Zavolání funkce $func
    operand1 = initOperand(operand1, "func", sIdentificator, F_LF, false, true, false, I_DEFAULT);
    writeInstructionOneOperand(&instList, I_CALL, operand1);

    // Práce s doublem (float)
    operand1 = initOperand(operand1, test_string1.value, sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "4e2", sDouble, F_LF, false, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);

    // Vypsání stringu "nejaky text\n"
    operand1 = initOperand(operand1, "nejaky\\032text", sString, F_LF, false, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList, I_WRITE, operand1);

    // Nahrání prázdného řetězce do LF@a
    operand1 = initOperand(operand1, test_string1.value, sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "", sString, F_LF, false, false, false,  I_DEFAULT);
    writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);

    // Nahrání int@4 do LF@b
    operand1 = initOperand(operand1, test_string3.value, sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, test_string2.value, sInteger, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);

    // Sečtení int@4 a TF@b do globální proměnné GF@tmp
    operand1 = initOperand(operand1, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, test_string2.value, sInteger, F_DEFAULT, false, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, test_string3.value, sIdentificator, F_TF, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList, I_ADD, operand1, operand2, operand3);

    // Přeskočení na návěští $else pokud v GF@tmp je bool@false
    operand1 = initOperand(operand1, "else", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    operand2 = initOperand(operand2, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "false", 42, F_TF, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList, I_JUMPIFNEQ, operand1, operand2, operand3);

    // Práce s proměnou na globálním rámci GF@tmp
    operand1 = initOperand(operand1, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList, I_DEFVAR, operand1);
    writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);
    writeInstructionThreeOperands(&instList, I_ADD, operand1, operand2, operand3);

    // Vypsání konce programu
    writeInstructionNoOperand(&instList, I_POPFRAME);
    operand1 = initOperand(operand1, "endscope", sKeyWord, F_DEFAULT, false, true, false, I_DEFAULT);
    writeInstructionOneOperand(&instList, I_LABEL, operand1);

    //if (result_code == ERROR_CODE_OK) // instrukcni paska se vypise na stdout pouze pokud preklad probehl v poradku
        printInstructionList(&instList);

    /*----------uvolneni alokovane pameti----------*/
    //symTableDispose(&glSymTable); // globalni tabulka symbolu
    DLDisposeList(&instList); // insturkcni paska


    return result_code;
}