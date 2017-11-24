#include <stdio.h>
#include <stdbool.h>
#include "../src/string.h"
#include "../src/inst_list.c"

tInstrOperand operand1; tInstrOperand operand2; tInstrOperand operand3;
tDLListInstruction instList1, instList2, instList3, instList4;

void writeLenght(tInstrOperand tmp_operand1, tInstrOperand tmp_operand2){
    writeInstructionTwoOperands(&instList1, I_STRLEN, operand1, operand2);
    return;
}

void writeAsc(tInstrOperand retezec, tInstrOperand index){
    writeInstructionNoOperand(&instList2, I_CREATEFRAME);
    operand1 = initOperand(operand1, "@%ascretval", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList2, I_DEFVAR, operand1);
    operand1 = initOperand(operand1, "s", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList2, I_DEFVAR, operand1);
    operand1 = initOperand(operand1, "i", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList2, I_DEFVAR, operand1);
    operand1 = initOperand(operand1, "s", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, retezec.value.value, sIdentificator, F_LF, false, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList2, I_MOVE, operand1, operand2);
    operand1 = initOperand(operand1, "i", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, index.value.value, sInteger, F_LF, false, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList2, I_MOVE, operand1, operand2);
    writeInstructionNoOperand(&instList2, I_PUSHFRAME);
    operand1 = initOperand(operand1, "s_len", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList2, I_DEFVAR, operand1);
    operand1 = initOperand(operand1, "s_len", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "s", sInteger, F_LF, false, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList2, I_STRLEN, operand1, operand2);
    operand1 = initOperand(operand1, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "i", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "0", sInteger, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList2, I_LT, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "%asc_error", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    operand2 = initOperand(operand2, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "true", 42, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList2, I_JUMPIFEQ, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "i", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "s_len", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList2, I_GT, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "%asc_error", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    operand2 = initOperand(operand2, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "true", 42, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList2, I_JUMPIFEQ, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "%ascretval", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "s", sIdentificator, F_LF, true, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "i", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList2, I_STRI2INT, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "%asc_end", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    writeInstructionOneOperand(&instList2, I_JUMP, operand1);
    operand1 = initOperand(operand1, "%asc_error", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    writeInstructionOneOperand(&instList2, I_LABEL, operand1);
    operand1 = initOperand(operand1, "%ascretval", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "0", sInteger, F_LF, false, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList2, I_MOVE, operand1, operand2);
    operand1 = initOperand(operand1, "%asc_end", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    writeInstructionOneOperand(&instList2, I_LABEL, operand1);
    writeInstructionNoOperand(&instList2, I_POPFRAME);
    return;
}

void writeChr(tInstrOperand hodnota){
    writeInstructionNoOperand(&instList3, I_CREATEFRAME);
    operand1 = initOperand(operand1, "@%chrretval", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList3, I_DEFVAR, operand1);
    operand1 = initOperand(operand1, "i", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList3, I_DEFVAR, operand1);
    operand1 = initOperand(operand1, "i", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, hodnota.value.value, sInteger, F_LF, false, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList3, I_MOVE, operand1, operand2);
    writeInstructionNoOperand(&instList3, I_PUSHFRAME);
    operand1 = initOperand(operand1, "@%chrretval", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "i", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList3, I_INT2CHAR, operand1, operand2);
    writeInstructionNoOperand(&instList3, I_POPFRAME);
    return;
}

void writeSubstr(tInstrOperand retezec, tInstrOperand index, tInstrOperand shift){
    writeInstructionNoOperand(&instList4, I_CREATEFRAME);
    operand1 = initOperand(operand1, "@%substrretval", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList4, I_DEFVAR, operand1);
    operand1 = initOperand(operand1, "@%substrretval", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "", sString, F_LF, false, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList4, I_MOVE, operand1, operand2);
    operand1 = initOperand(operand1, "substr_string", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList4, I_DEFVAR, operand1);
    operand1 = initOperand(operand1, "substr_len", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList4, I_DEFVAR, operand1);
    operand1 = initOperand(operand1, "substr_index", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList4, I_DEFVAR, operand1);
    operand1 = initOperand(operand1, "substr_shift", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList4, I_DEFVAR, operand1);
    operand1 = initOperand(operand1, "substr_char", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    writeInstructionOneOperand(&instList4, I_DEFVAR, operand1);
    operand1 = initOperand(operand1, "substr_string", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, retezec.value.value, sIdentificator, F_LF, false, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList4, I_MOVE, operand1, operand2);
    operand1 = initOperand(operand1, "substr_index", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, index.value.value, sInteger, F_LF, false, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList4, I_MOVE, operand1, operand2);
    operand1 = initOperand(operand1, "substr_shift", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, shift.value.value, sInteger, F_LF, false, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList4, I_MOVE, operand1, operand2);
    operand1 = initOperand(operand1, "substr_len", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "substr_string", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList4, I_STRLEN, operand1, operand2);
    operand1 = initOperand(operand1, "substr_len", sIdentificator, F_DEFAULT, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "substr_len", sIdentificator, F_DEFAULT, false, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "substr_index", sIdentificator, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList4, I_SUB, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "substr_index", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "1", sInteger, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList4, I_LT, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "%emptystring", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    operand2 = initOperand(operand2, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "true", 42, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList4, I_JUMPIFEQ, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "%emptystring", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    operand2 = initOperand(operand2, "substr_string", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "", sString, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList4, I_JUMPIFEQ, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "substr_shift", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "0", sInteger, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList4, I_LT, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "%error_shift", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    operand2 = initOperand(operand2, "", sIdentificator, F_TF, true, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "true", 42, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList4, I_JUMPIFEQ, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "substr_shift", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "substr_len", sIdentificator, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList4, I_GT, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "%error_shift", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    operand2 = initOperand(operand2, "", sIdentificator, F_TF, true, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "true", 42, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList4, I_JUMPIFEQ, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "%endsubstrwithshift0", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    operand2 = initOperand(operand2, "substr_shift", sIdentificator, F_TF, true, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "0", sInteger, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList4, I_JUMPIFEQ, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "%substr", sIdentificator, F_TF, false, true, false, I_DEFAULT);
    writeInstructionOneOperand(&instList4, I_JUMP, operand1);
    operand1 = initOperand(operand1, "%error_shift", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    writeInstructionOneOperand(&instList4, I_LABEL, operand1);
    operand1 = initOperand(operand1, "substr_shift", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "substr_len", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "substr_index", sIdentificator, F_TF, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList4, I_SUB, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "%substr", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    writeInstructionOneOperand(&instList4, I_LABEL, operand1);
    writeInstructionNoOperand(&instList4, I_PUSHFRAME);
    operand1 = initOperand(operand1, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "\\00", sString, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList4, I_MOVE, operand1, operand2);
    operand1 = initOperand(operand1, "%do_substr", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    writeInstructionOneOperand(&instList4, I_LABEL, operand1);
    operand1 = initOperand(operand1, "substr_char", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "substr_string", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "substr_index", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList4, I_GETCHAR, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "substr_index", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "substr_index", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "1", sInteger, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList4, I_ADD, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "substr_char", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList4, I_CONCAT, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "substr_shift", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "substr_shift", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "1", sInteger, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList4, I_SUB, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "%do_substr", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    operand2 = initOperand(operand2, "substr_shift", sIdentificator, F_LF, true, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "0", sInteger, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionThreeOperands(&instList4, I_JUMPIFNEQ, operand1, operand2, operand3);
    operand1 = initOperand(operand1, "%substrretval", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList4, I_MOVE, operand1, operand2);
    operand1 = initOperand(operand1, "%endsubstr", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    writeInstructionOneOperand(&instList4, I_JUMP, operand1);
    operand1 = initOperand(operand1, "%emptystring", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    writeInstructionOneOperand(&instList4, I_LABEL, operand1);
    operand1 = initOperand(operand1, "%substrretval", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "", sString, F_DEFAULT, false, false, false, I_DEFAULT);
    writeInstructionTwoOperands(&instList4, I_MOVE, operand1, operand2);
    operand1 = initOperand(operand1, "%endsubstr", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    writeInstructionOneOperand(&instList4, I_LABEL, operand1);
    writeInstructionNoOperand(&instList4, I_POPFRAME);
    operand1 = initOperand(operand1, "%endsubstrwithshift0", sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
    writeInstructionOneOperand(&instList4, I_LABEL, operand1);
    return;
}

int main(){
    DLInitList(&instList1); DLInitList(&instList2); DLInitList(&instList3); DLInitList(&instList4);

    printf("Vypsani Lenght(cil, retezec)\n--------------------------\n");
    operand1 = initOperand(operand1, "A", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "aaa", sString, F_DEFAULT, false, false, false, I_DEFAULT);
    writeLenght(operand1, operand2);
    printInstructionList(&instList1);
    printf("\n");

    printf("Vypsani Asc(retezec, index)\n--------------------------\n");
    operand1 = initOperand(operand1, "s1", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "5", sInteger, F_DEFAULT, false, false, false, I_DEFAULT);
    writeAsc(operand1, operand2);
    printInstructionList(&instList2);
    printf("\n");

    printf("Vypsani Chr(hodnota)\n--------------------------\n");
    operand1 = initOperand(operand1, "97", sInteger, F_LF, false, false, false, I_DEFAULT);
    writeChr(operand1);
    printInstructionList(&instList3);
    printf("\n");

    printf("Vypsani Substr(retezec, index, shift)\n--------------------------\n");
    operand1 = initOperand(operand1, "s2", sIdentificator, F_LF, false, false, false, I_DEFAULT);
    operand2 = initOperand(operand2, "10", sInteger, F_DEFAULT, false, false, false, I_DEFAULT);
    operand3 = initOperand(operand3, "4", sInteger, F_DEFAULT, false, false, false, I_DEFAULT);
    writeSubstr(operand1, operand2, operand3);
    printInstructionList(&instList4);
    printf("\n");

    return 0;
}

