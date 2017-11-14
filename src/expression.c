/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   expression.c
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */
#include "expression.h"
#include "scanner.h"
#include "stack.h"

tToken next_exp_token; //Převzatý token od scanneru
tStack *first_terminal; //Nejvyšší terminál na stacku
int operation;

const char precedenceTable[PT_SIZE][PT_SIZE] = {
//           *     /     \     +     -     =    <>     <    <=     >    >=     (     )     ID    $
/*  *  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' },
/*  /  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' },
/*  \  */ { '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' },
/*  +  */ { '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' },
/*  -  */ { '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' },
/*  =  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' },
/*  <> */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' },
/*  <  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' },
/*  <= */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' },
/*  >  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' },
/*  >= */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' },
/*  (  */ { '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , '<' , '_' },
/*  )  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '>' },
/*  ID */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '>' },
/*  $  */ { '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '_' , '<' , '_' },
};

//Funkce volaná parserem
ERROR_CODE expression(){

    ptrStack expression_stack;
    ERROR_CODE result = expressionAnalysis(&expression_stack);
    SDispose(&expression_stack);
    return result;

}

//Hlavní funkce analyzující výraz
ERROR_CODE expressionAnalysis( ptrStack *expression_stack){

    ERROR_CODE error_type = ERROR_CODE_OK;
        //Inicializace stacku
        initExpressionStack(expression_stack);


        //bool do_analysis = true;
        char sign;
        next_exp_token = getNextToken();
        //Analýza výrazu
        while (1){

            if(((Exp_element*)(first_terminal->value))->pt_index == eDollar && convertTokenToIndex(next_exp_token.type) == eDollar) {
                printf("\nOK");
                break;
            }

            sign = getSignFromTable();


            if(sign == '='){  //Uplatňujeme pravidlo pro redukci závorek
                SPush(expression_stack,newElementToStack(next_exp_token.atr,convertTokenToIndex(next_exp_token.type),next_exp_token.type));

                if((error_type = reducePars(expression_stack)) != ERROR_CODE_OK) {
                    printf("\nSpatne");
                    return error_type;
                }
                printf("%d,",operation);
                next_exp_token = getNextToken();
            }
            else if (sign == '<') {     //Pushujeme na zábosobník

                if((error_type = shiftToStack(expression_stack)) != ERROR_CODE_OK) {
                    printf("\nSpatne");
                    return error_type;
                }
                else
                next_exp_token = getNextToken();
            }
            else if(sign == '>'){       //Uplatňujeme pravidla pro redukci binárních operátorů

                if((error_type = useRule(expression_stack)) != ERROR_CODE_OK){
                    printf("\nSpatne");
                    return error_type;
                }
                printf("%d,",operation);
            }
            else {      //Pokud nastane nepovolený stav
                printf("\nSpatne");
                return ERROR_CODE_SYN;
            }

        }
    return error_type;
}

//Inicializace stacku vlozi jako prvni znak dollar
ERROR_CODE initExpressionStack(ptrStack *expression_stack){

    if(expression_stack != NULL) {
        SInit(expression_stack);

        Exp_element *new_element;

        //Vytvořím string "$"
        string dollar;
        stringInit(&dollar);
        stringAddChar(&dollar, '$');

        //Jestli byl alokován a vytvořen nový prvek, vloží se na stack
        if ((new_element = newElementToStack(dollar, eDollar, sStart)) != NULL) {
            SPush(expression_stack, new_element);
            first_terminal = expression_stack->top_of_stack;
            return ERROR_CODE_OK;
        }
        else
            return ERROR_CODE_SYN;

    }
    else
        return ERROR_CODE_SYN;

}

//Funkce shiftuje na stack tokeny ze vstupu
ERROR_CODE shiftToStack(ptrStack *expression_stack){
    if (expression_stack != NULL) {
        ((Exp_element*) (first_terminal->value))->handle = true;
        Exp_element* new_element = newElementToStack(next_exp_token.atr, convertTokenToIndex(next_exp_token.type), next_exp_token.type);

        if(new_element != NULL) {
            SPush(expression_stack,new_element);
            first_terminal = expression_stack->top_of_stack;
            return ERROR_CODE_OK;
        }
        else
            return ERROR_CODE_SYN;
    }
    else
        return ERROR_CODE_SYN;
}

//Funkce uplatňuje pravidla pro redukci terminálů
ERROR_CODE useRule(ptrStack *expression_stack){
    ERROR_CODE error_type = ERROR_CODE_OK;

    if(expression_stack != NULL) {
        //int first_terminal = getRule(expression_stack);
        tStack *stack_item = expression_stack->top_of_stack;
        operation = 0;

        switch (((Exp_element *) (first_terminal->value))->pt_index) {

            //Řeší redukci pro ID (operand)
            case eOperand:
                ((Exp_element *) (stack_item->value))->terminal = false;
                ((Exp_element *) (stack_item->value))->handle = false;
                ((Exp_element *) (stack_item->left->value))->handle = false;
                first_terminal = (stack_item->left);
                operation = eOperand;
                break;

                //Řeší redukci násobení
            case eMultiply:

                if((error_type = reduceBinary(expression_stack, eMultiply)) != ERROR_CODE_OK)
                    return error_type;

                operation = eMultiply;
                break;
                //Řeší redukci dělení
            case eDivideD:

                if((error_type = reduceBinary(expression_stack, eDivideD)) != ERROR_CODE_OK)
                    return error_type;

                operation = eDivideD;
                break;

                //Řeší redukci celočíselného dělení
            case eDivideI:

                if((error_type = reduceBinary(expression_stack, eDivideI)) != ERROR_CODE_OK)
                        return error_type;
                operation = eDivideI;
                break;

                //Řeší redukci sčítání
            case ePlus:

                if((error_type = reduceBinary(expression_stack, ePlus)) != ERROR_CODE_OK)
                    return error_type;
                operation = ePlus;
                break;

                //Řeší redukci odčítání
            case eMinus:

                if((error_type = reduceBinary(expression_stack, eMinus)) != ERROR_CODE_OK)
                    return error_type;
                operation = eMinus;
                break;

                //Řeší redukci rovnosti
            case eEqual:

                if((error_type = reduceBinary(expression_stack, eEqual)) != ERROR_CODE_OK)
                    return error_type;
                operation = eEqual;
                break;

                //Řeší redukci nerovnosti
            case eNotEqual:
                if((error_type = reduceBinary(expression_stack, eNotEqual)) != ERROR_CODE_OK)
                    return error_type;
                operation = eNotEqual;
                break;

                //Řeší redukci menšítka
            case eLess:
                if((error_type = reduceBinary(expression_stack, eLessEqual)) != ERROR_CODE_OK)
                    return error_type;
                operation = eLess;
                break;

                //Řeší redukci většítka
            case eMore:
                if((error_type = reduceBinary(expression_stack, eLessEqual)) != ERROR_CODE_OK)
                    return error_type;
                operation = eMore;
                break;

                //Řeší redukci menší nebo rovno
            case eLessEqual:
                if((error_type = reduceBinary(expression_stack, eLessEqual)) != ERROR_CODE_OK)
                    return error_type;
                operation = eLessEqual;
                break;

                //Řeší redukci větší nebo rovno
            case eMoreEqual:
                if((error_type = reduceBinary(expression_stack, eMoreEqual)) != ERROR_CODE_OK)
                    return error_type;
                operation = eMoreEqual;
                break;

        }
        return error_type;
    }
    else
        return ERROR_CODE_SYN;
}


//Funkce pro redukci binárních operátorů
ERROR_CODE reduceBinary(ptrStack *expression_stack,int operator){
    if(expression_stack != NULL) {

        tStack *stack_item = (expression_stack->top_of_stack);
        int count = 0;
        //int operand_type_l;
        //int operand_type_r;

        //Kontrola syntaxe, zda binární operátor má své dva neterminály
        while (((Exp_element *) (stack_item->value))->handle != true) {

            //Pokud není čtvrtý znak v pořadí handle, je to chyba
            if (count > 3) {
                return ERROR_CODE_SYN;
            }

            switch (count) {
                case 0:
                    //První prvek musí být neterminál
                    if (((Exp_element *) (stack_item->value))->terminal != false)
                        return ERROR_CODE_SYN;
                    //operand_type_r = ((Exp_element *) (stack_item->value))->token_type;
                    break;
                case 1:
                    //Druhý operátor, tudíž i temrinal
                    if (((Exp_element *) (stack_item->value))->terminal != true &&
                        ((Exp_element *) (stack_item->value))->pt_index != operator)
                        return ERROR_CODE_SYN;
                    break;
                case 2:

                    //Třetí musí být neterminál
                    if (((Exp_element *) (stack_item->value))->terminal != false)
                        return ERROR_CODE_SYN;
                    //operand_type_l = ((Exp_element *) (stack_item->value))->token_type;
                    break;
            }
            count++;
            stack_item = stack_item->left;
        }

        //Pokud je binární operace správně, popne pravý neterminál a operátor ze stacku
        SPop(expression_stack);
        SPop(expression_stack);

        //Další první terminál je operátor před neterminálem
        first_terminal = expression_stack->top_of_stack->left;
        ((Exp_element *) (first_terminal->value))->handle = false;

        return ERROR_CODE_OK;
    }
    else
        return ERROR_CODE_SYN;
}


//Funkce pro redukci závorek
ERROR_CODE reducePars(ptrStack *expression_stack){


    if(expression_stack != NULL){

        tStack *stack_item = expression_stack->top_of_stack;
        tStack *tmp = expression_stack->top_of_stack->left; //Dočasné úložiště pro neterminál mezi závorkami

        if(((Exp_element*)(stack_item->left->value))->terminal == false &&
           ((Exp_element*)(stack_item->left->left->value))->pt_index == eLeftPar){

            //Ze zásobníku se popnou 3 prvku
            for (int i = 0; i < 3; ++i) {
                    SPop(expression_stack);
                }

            //Zpět pushneme uložený neterminál
            SPush(expression_stack,((Exp_element*) (tmp->value)));

            //Přenastavení prvního terminálu na následující znak na zásobníku a deaktivování handlu
            if(((Exp_element*)(expression_stack->top_of_stack->left->value))->terminal == true) {
                first_terminal = expression_stack->top_of_stack->left;
                ((Exp_element*) (first_terminal->value))->handle = false;
                operation = eRightPar;
            }
            else
                return ERROR_CODE_SYN;
        }
        else
            return ERROR_CODE_SYN;

        return ERROR_CODE_OK;
    }
    else
        return ERROR_CODE_SYN;
}
/*
//Funkce kontroluje sémantiku
ERROR_CODE checkSemantic( int operand_type_l,int operator, int operand_type_r){
    if((operator == eMultiply) || (operator == eDivideD) || (operator == eMinus)){
        if(operand_type_l == sString || operand_type_r == sString)
            return ERROR_CODE_SEM;
    }
    else if(operator == eDivideI){
        if (operand_type_l == sDouble || operand_type_r == sDouble)
            return ERROR_CODE_SEM;
    }
    else if(operator == ePlus){
        if((operand_type_l == sString && operand_type_r != sString) ||
           (operand_type_r == sString && operand_type_l != sString) ){
            return ERROR_CODE_SEM;
        }
    }
    else if((operator == sAssignment) || (operator == sNotEqual) || (operator == sLess) ||
            (operator == sLessEqual) ||(operator == sMore) || (operator == sMoreEqueal)){
        if((operand_type_l == sString && operand_type_r != sString) ||
           (operand_type_r == sString && operand_type_l != sString) ){
            return ERROR_CODE_SEM;
        }
    }
    return ERROR_CODE_OK;
}

int changeOperandType(ptrStack *expression_stack, int op_to){

}*/

//Funkce vytvaří nový element pro vložení na stack
Exp_element *newElementToStack(string value, int pt_index, int token_type){

    //Nový element na stack
    Exp_element *new_element = malloc(sizeof(struct exp_element));

    //Inicializace noveho elementu
    if(new_element != NULL){
        new_element->value = value;
        new_element->token_type = token_type;
        new_element->pt_index = pt_index;
        new_element->terminal = true;
        new_element->handle = false;
        return new_element;
    }
    else
        return NULL;
}

//Funkce získává na základě znaku na stacku a vstupu znaménko z PT
char getSignFromTable(){

    int table_input_index = convertTokenToIndex(next_exp_token.type);
    int table_stack_index = ((Exp_element*)(first_terminal->value))->pt_index;


    if(table_input_index != eOther) { //Pokud naleži index jednomu ze znaků
        return precedenceTable[table_stack_index][table_input_index];

    }
    else
        return '_';
}


//Funkce konvertuje hodnotu typu tokenu na index v PT
int convertTokenToIndex(int token_num){
    switch (token_num) {
        case sIdentificator:
            return eOperand;

        case sInteger:
            return eOperand;

        case sDouble:
            return eOperand;

        case sString:
            return eOperand;

        case sPlus:
            return ePlus;

        case sMinus:
            return eMinus;

        case sMultiply:
            return eMultiply;

        case sDivideD:
            return eDivideD;

        case sDivideI:
            return eDivideI;

        case sAssignment:
            return eEqual;

        case sNotEqual:
            return eNotEqual;

        case sLess:
            return eLess;

        case sLessEqual:
            return eLessEqual;

        case sMore:
            return eMore;

        case sMoreEqueal:
            return eMoreEqual;

        case sLeftPar:
            return eLeftPar;

        case sRightPar:
            return eRightPar;

        case sEndOfLine:
            return eDollar;
    }
    return eOther;
}


