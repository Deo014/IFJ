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
#include "string.h"
#include "instList.h"

int main(int argc, char **argv)
{
    ERROR_CODE error_code = ERROR_CODE_OK;
    //stringInit(&token.s); // inicializace tokenu

    // kontrola argumentů
    ERROR_CODE helpStatus = checkArgs(argc, argv);
    if(helpStatus == ERROR_CODE_HELP)
        return error_code = ERROR_CODE_HELP;
    else if(helpStatus == ERROR_CODE_INTERNAL)
        return error_code = ERROR_CODE_INTERNAL;


    // otestovani vkladani instrukci do instrukcniho seznamu--SMAZAT
    tDLListInstruction instList;
    DLInitList(&instList);

    /*
    // práce s rámci, volání funkcí
    generateInstruction(&instList, I_MOVE, NULL, NULL, NULL);
    generateInstruction(&instList, I_CREATEFRAME, NULL, NULL, NULL);
    generateInstruction(&instList, I_PUSHFRAME, NULL, NULL, NULL);
    generateInstruction(&instList, I_POPFRAME, NULL, NULL, NULL);
    generateInstruction(&instList, I_DEFVAR, NULL, NULL, NULL);
    generateInstruction(&instList, I_CALL, NULL, NULL, NULL);
    generateInstruction(&instList, I_RETURN, NULL, NULL, NULL);
    // práce s datovým zásobníkem
    generateInstruction(&instList, I_PUSHS, NULL, NULL, NULL);
    generateInstruction(&instList, I_POPS, NULL, NULL, NULL);
    generateInstruction(&instList, I_CLEARS, NULL, NULL, NULL);
    // Aritmetické, relační, booleovské a konverzní instrukce
    generateInstruction(&instList, I_ADD, NULL, NULL, NULL);
    generateInstruction(&instList, I_SUB, NULL, NULL, NULL);
    generateInstruction(&instList, I_MUL, NULL, NULL, NULL);
    generateInstruction(&instList, I_DIV, NULL, NULL, NULL);
    generateInstruction(&instList, I_ADDS, NULL, NULL, NULL);
    generateInstruction(&instList, I_SUBS, NULL, NULL, NULL);
    generateInstruction(&instList, I_MULS, NULL, NULL, NULL);
    generateInstruction(&instList, I_DIVS, NULL, NULL, NULL);
    generateInstruction(&instList, I_LT, NULL, NULL, NULL);
    generateInstruction(&instList, I_GT, NULL, NULL, NULL);
    generateInstruction(&instList, I_EQ, NULL, NULL, NULL);
    generateInstruction(&instList, I_LTS, NULL, NULL, NULL);
    generateInstruction(&instList, I_GTS, NULL, NULL, NULL);
    generateInstruction(&instList, I_EQS, NULL, NULL, NULL);
    generateInstruction(&instList, I_AND, NULL, NULL, NULL);
    generateInstruction(&instList, I_OR, NULL, NULL, NULL);
    generateInstruction(&instList, I_NOT, NULL, NULL, NULL);
    generateInstruction(&instList, I_ANDS, NULL, NULL, NULL);
    generateInstruction(&instList, I_ORS, NULL, NULL, NULL);
    generateInstruction(&instList, I_NOTS, NULL, NULL, NULL);
    generateInstruction(&instList, I_INT2FLOAT, NULL, NULL, NULL);
    generateInstruction(&instList, I_FLOAT2INT, NULL, NULL, NULL);
    generateInstruction(&instList, I_FLOAT2R2EINT, NULL, NULL, NULL);
    generateInstruction(&instList, I_FLOAT2R2OINT, NULL, NULL, NULL);
    generateInstruction(&instList, I_INT2CHAR, NULL, NULL, NULL);
    generateInstruction(&instList, I_STRI2INT, NULL, NULL, NULL);
    generateInstruction(&instList, I_INT2FLOATS, NULL, NULL, NULL);
    generateInstruction(&instList, I_FLOAT2INTS, NULL, NULL, NULL);
    generateInstruction(&instList, I_FLOAT2R2EINTS, NULL, NULL, NULL);
    generateInstruction(&instList, I_FLOAT2R2OINTS, NULL, NULL, NULL);
    generateInstruction(&instList, I_INT2CHARS, NULL, NULL, NULL);
    generateInstruction(&instList, I_STRI2INTS, NULL, NULL, NULL);
    // Vstupně-výstupní instrukce
    generateInstruction(&instList, I_READ, NULL, NULL, NULL);
    generateInstruction(&instList, I_WRITE, NULL, NULL, NULL);
    // Práce s řetězci
    generateInstruction(&instList, I_CONCAT, NULL, NULL, NULL);
    generateInstruction(&instList, I_STRLEN, NULL, NULL, NULL);
    generateInstruction(&instList, I_GETCHAR, NULL, NULL, NULL);
    generateInstruction(&instList, I_SETCHAR, NULL, NULL, NULL);
    // Práce s typy
    generateInstruction(&instList, I_TYPE, NULL, NULL, NULL);
    // Instrukce pro řízení toku programu
    generateInstruction(&instList, I_LABEL, NULL, NULL, NULL);
    generateInstruction(&instList, I_JUMP, NULL, NULL, NULL);
    generateInstruction(&instList, I_JUMPIFEQ, NULL, NULL, NULL);
    generateInstruction(&instList, I_JUMPIFNEQ, NULL, NULL, NULL);
    generateInstruction(&instList, I_JUMPIFEQS, NULL, NULL, NULL);
    generateInstruction(&instList, I_JUMPIFNEQS, NULL, NULL, NULL);
    // Ladící instrukce
    generateInstruction(&instList, I_BREAK, NULL, NULL, NULL);
    generateInstruction(&instList, I_DPRINT, NULL, NULL, NULL);
    */

    printInstructionList(&instList);

    return error_code;
}