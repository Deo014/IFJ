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
#include "instlist.h"
#include "symtable.h"
#include "parser.h"
#include "string.h"
#include "bintree.h"
#include "expression.h"
#include "scanner.h"


tSymtable glSymTable; // globalni tabulka symbolu
tSymtable table;
tDLListInstruction instList; // globalni list vygenerovanych instrukci (instrukcni paska)

int main(int argc, char **argv)
{
    char f1[] = "length";
    char f2[] = "substr";
    char f3[] = "asc";
    char f4[] = "chr";
    string length;
    string substr;
    string asc;
    string chr;
    string paramLength;
    string paramSubstr;
    string paramAsc;
    string paramChr;
    tBSTNodePtr node;
    stringInit(&paramLength);
    stringInit(&paramSubstr);
    stringInit(&paramAsc);
    stringInit(&paramChr);
    stringInit(&length);
    stringInit(&substr);
    stringInit(&asc);
    stringInit(&chr);
    //freopen("test1.txt", "r", stdin);
    ERROR_CODE result_code = ERROR_CODE_OK; // vysledny kod programu (pokud preklad probehne v poradku, hodnota bude ERROR_CODE_OK)

    /*----------kontrola poctu argumentu----------*/
    if (argc > 1) {
        fprintf(stderr, "Program \"%s\" nesmi byt volan s parametry.\n", *argv);
        return ERROR_CODE_INTERNAL;
    }

    /*----------inicializace----------*/
    symTableInit(&glSymTable); // globalni tabulka symbolu
    symTableInit(&table);

    stringAddChars(&length, f1);
    symTableInsertFunction(&glSymTable, length);
    node = symTableSearch(&glSymTable, length);
    ((tDataFunction *) node->Data)->declared = true;
    ((tDataFunction *) node->Data)->defined = true;
    stringAddChar(&((tDataFunction *) node->Data)->parameters, 's');
    stringAddChar(&paramLength, 's');
    ((tDataFunction *) node->Data)->paramName[0] = paramLength;
    stringClear(&paramLength);
    ((tDataFunction *) node->Data)->returnDataType = sInteger;


    stringAddChars(&substr, f2);
    symTableInsertFunction(&glSymTable, substr);
    node = symTableSearch(&glSymTable, substr);
    ((tDataFunction *) node->Data)->declared = true;
    ((tDataFunction *) node->Data)->defined = true;
    stringAddChar(&((tDataFunction *) node->Data)->parameters, 's');
    stringAddChar(&((tDataFunction *) node->Data)->parameters, 'i');
    stringAddChar(&((tDataFunction *) node->Data)->parameters, 'i');
    stringAddChar(&paramSubstr, 's');
    ((tDataFunction *) node->Data)->paramName[0] = paramSubstr;
    stringClear(&paramSubstr);
    stringAddChar(&paramSubstr, 'i');
    ((tDataFunction *) node->Data)->paramName[1] = paramSubstr;
    stringClear(&paramSubstr);
    stringAddChar(&paramSubstr, 'n');
    ((tDataFunction *) node->Data)->paramName[2] = paramSubstr;
    stringClear(&paramSubstr);
    ((tDataFunction *) node->Data)->returnDataType = sString;


    stringAddChars(&asc, f3);
    symTableInsertFunction(&glSymTable, asc);
    node = symTableSearch(&glSymTable, asc);
    ((tDataFunction *) node->Data)->declared = true;
    ((tDataFunction *) node->Data)->defined = true;
    stringAddChar(&((tDataFunction *) node->Data)->parameters, 's');
    stringAddChar(&((tDataFunction *) node->Data)->parameters, 'i');
    stringAddChar(&paramAsc, 's');
    ((tDataFunction *) node->Data)->paramName[0] = paramAsc;
    stringClear(&paramAsc);
    ((tDataFunction *) node->Data)->returnDataType = sInteger;

    stringAddChars(&chr, f4);
    symTableInsertFunction(&glSymTable, chr);
    node = symTableSearch(&glSymTable, chr);
    ((tDataFunction *) node->Data)->declared = true;
    ((tDataFunction *) node->Data)->defined = true;
    stringAddChar(&((tDataFunction *) node->Data)->parameters, 'i');
    stringAddChar(&paramChr, 'i');
    ((tDataFunction *) node->Data)->paramName[0] = paramChr;
    stringClear(&paramChr);
    ((tDataFunction *) node->Data)->returnDataType = sString;


    //DLInitList(&instList);  // instrukcni paska

    /*----------Syntakticka analyza, Semanticka analyza, Generovani 3AK----------*/
    result_code = parse();

    //generateInstruction(&instList, I_DEFVAR, "variable", NULL, NULL);
    /*----------vypsani instrukcni pasky na stdout----------*/

    //*******************************************
    /*
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
  */
    //generateInstruction(&instList, I_DEFVAR, &test_element1, NULL, NULL);


    if (result_code == ERROR_CODE_OK) // instrukcni paska se vypise na stdout pouze pokud preklad probehl v poradku
        printInstructionList(&instList);

    /*----------uvolneni alokovane pameti----------*/
    symTableDispose(&glSymTable); // globalni tabulka symbolu
    symTableDispose(&table);
    DLDisposeList(&instList); // insturkcni paska


    return result_code;
}