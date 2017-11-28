
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
#include "stack.h"
#include "string.h"
#include "scanner.h"

tToken next_exp_token; //Převzatý token od scanneru
tToken fun_or_var;      //Uchovává proměnnou a čeká, jestli se bude řešit funkce nebo proměnná
tStack *first_terminal; //Nejvyšší terminál na stacku


extern tSymtable glSymTable;            //GL tabulka symbolů
extern tSymtable table;                 //Lokální tabulka
extern tDLListInstruction instList;     //List instrukcí
extern bool inFunctionBody;             //Indikátor, že se kontroluje tělo funkce
extern tToken varToSet;
extern tToken tmpToken;

//str_element result_element;

//int operation;
int operation_type_global;  //Typ výsledné proměnné //Typ výsledku
bool exp_function;          //Pokud se řeší funkce je true
//bool shift_saved_token = false;
//bool exprEnd = false;
int parameter_index = 0;    //Index kontrolovaného parametru
char *params;               //Typy parametrů kontrolované funkce
int param_length = 0;       //Počet parametrů kontr. funkce

const char precedenceTable[PT_SIZE][PT_SIZE] = {
//           *     /     \     +     -     =    <>     <    <=     >    >=     (     )     ID    F     ,     $
/*  *  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  /  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  \  */ { '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  +  */ { '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  -  */ { '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  =  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  <> */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  <  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  <= */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  >  */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  >= */ { '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '_' , '_' , '>' },
/*  (  */ { '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , '<' , '_' , '<' , '_' },
/*  )  */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '_' , '_' , '>' },
/*  ID */ { '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '_' , '>' , '_' , '_' , '>' , '>' },
/*  F  */ { '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '_' , '_' , '_' , '_' , '_' },
/*  ,  */ { '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '_' , '<' , '<' , '_' , '<' , '_' },
/*  $  */ { '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '_' , '<' , '<' , '_' , '_' },
};

//Funkce volaná parserem
ERROR_CODE expression(tToken first_token,int operation_type){
    ERROR_CODE result;
    ptrStack expression_stack;
    operation_type_global = operation_type;
    //Pokud je první token EOF nebo EOL, je to chyba
    if (first_token.type == sEndOfLine || first_token.type == sEndOfFile) {
        return ERROR_CODE_SYN;
    } else {
        result = expressionAnalysis(&expression_stack, first_token);
        exp_function = false;
        // exprEnd = false;
        //shift_saved_token = false;
        parameter_index = 0;
        SDispose(&expression_stack);
    }
    return result;
}


//Hlavní funkce analyzující výraz
ERROR_CODE expressionAnalysis(ptrStack *expression_stack,tToken first_token){

    ERROR_CODE error_type;
    //Inicializace stacku
    initExpressionStack(expression_stack);


    char sign;

    fun_or_var = first_token;
    next_exp_token = first_token;
    //Analýza výrazu
    while (1){

        //Jestli je na vstupu token s lex. errorem, končím
        if(next_exp_token.type == sLexError){
            return ERROR_CODE_LEX;
        }



        //Jestli máme na vrcholu stacku dollar a na vstupu ukončující vstup, je to OK
        if(((Exp_element*)(first_terminal->value))->pt_index == eDollar && convertTokenToIndex(next_exp_token.type) == eDollar) {
            //Pokud se neřeší podmínka nebo print
            if (operation_type_global != -1) {
                //Zkontroluje se typ výsledku
                if ((error_type = checkResultType(expression_stack)) != ERROR_CODE_OK)
                    return error_type;
            }
            else {
                if (((Exp_element *) expression_stack->top_of_stack->value)->pt_index == eDollar)
                    return ERROR_CODE_SYN;
            }
            return ERROR_CODE_OK;
        }

        if(strcmp(next_exp_token.atr.value,"then") != 0  && next_exp_token.type == sKeyWord){
            return ERROR_CODE_SYN;
        }

        //Zjistíme se známénko z tabulky
        sign = getSignFromTable();


        if(sign == '=') {  //Uplatňujeme pravidlo pro redukci závorek
            SPush(expression_stack, newElementToStack(next_exp_token.atr, convertTokenToIndex(next_exp_token.type),
                                                      next_exp_token.type));

            if (exp_function == true) {
                if ((error_type = reduceFunction(expression_stack)) != ERROR_CODE_OK) {
                    return error_type;
                }
            } else {
                if ((error_type = reducePars(expression_stack)) != ERROR_CODE_OK) {
                    return error_type;
                }


            }
            //printf("%d,",operation);
            next_exp_token = getNextToken();
        }
        else if (sign == '<') {     //Pushujeme na zábosobník

            if((error_type = shiftToStack(expression_stack)) != ERROR_CODE_OK) {
                return error_type;
            }
            next_exp_token = getNextToken();

        }
        else if(sign == '>'){       //Uplatňujeme pravidla pro redukci binárních operátorů

            if((error_type = useRule(expression_stack)) != ERROR_CODE_OK){
                return error_type;
            }
            //printf("%d,",operation);
        }
        else {      //Pokud nastane nepovolený stav
            if (((Exp_element *) expression_stack->top_of_stack->value)->pt_index == eFunction)
                return ERROR_CODE_SEM;
            else
                return ERROR_CODE_SYN;
        }

    }
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
    ERROR_CODE error_type;


    if (expression_stack != NULL) {
        ((Exp_element*) (first_terminal->value))->handle = true;
        Exp_element* new_element = newElementToStack(next_exp_token.atr, convertTokenToIndex(next_exp_token.type), next_exp_token.type);

        if(new_element != NULL) {

            //Jestli je vkládaný prvek proměnná, zjistíme, jakou má v tabulce symbolů typ
            if(sIdentificator == new_element->token_type){
                tBSTNodePtr element_id = symTableSearch(&glSymTable,new_element->value);

                if(element_id != NULL) {
                    if(inFunctionBody) {
                        // fun_or_var = getNextToken();
                        if ((symTableSearch(&table, new_element->value) != NULL)) {
                            return ERROR_CODE_SEM;
                            /*
                            element_id = symTableSearch(&table,new_element->value);

                            if(convertTokenToIndex(fun_or_var.type) < eDollar) {
                                next_exp_token = fun_or_var;
                                shift_saved_token = true;
                            }
                            else {shift_saved_token = true;
                                next_exp_token = fun_or_var;
                                exprEnd = true;
                            }*/
                        } /*else {
                            next_exp_token = fun_or_var;
                            shift_saved_token = true;
                        }*/
                    }
                }
                //Pokud jsme nenašli v GL tabulce identifikator
                else {

                    //Pokud se nachazime v těle funkce, prohledame lokalni tabulku
                    if(inFunctionBody){
                        element_id = symTableSearch(&table,new_element->value);

                        //Pokud se i tak nic nenašlo nebo pokud našlo, ale není to proměnna, je to chyba
                        if(element_id == NULL || element_id->nodeDataType != ndtVariable){
                            return ERROR_CODE_SEM;
                        }
                    }//Pokud neřešíme tělo funkce, nenašli jsme identifikator -> chyba
                    else
                        return ERROR_CODE_SEM;
                }

                //Pokud se jedná o proměnnou
                if (element_id->nodeDataType == ndtVariable) {
                    tDataVariable *variable = ((tDataVariable *) (element_id->Data));
                    if(!exp_function) {
                        //A podle toho nastavíme typ prvku vkládanému na stack
                        switch (variable->dataType) {
                            case sInteger:
                                new_element->token_type = sInteger;
                                break;
                            case sDouble:
                                new_element->token_type = sDouble;
                                break;
                            case sString:
                                new_element->token_type = sString;
                                break;
                        }
                    }
                    else{
                        //Pokud řešíme funkci, jedná se o parametr, který se zkontroluje
                        if((error_type = checkParams(new_element,variable->dataType)) != ERROR_CODE_OK)
                            return error_type;
                    }
                }
                    //Pokud se jedná o funkci
                else if(element_id->nodeDataType == ndtFunction) {
                    tDataFunction *function = ((tDataFunction*) (element_id->Data));
                    if ((function->returnDataType != sString && sString == operation_type_global) ||
                        (function->returnDataType == sString && sString != operation_type_global))
                        return ERROR_CODE_SEM_COMP;
                    else {
                        //A podle toho nastavíme typ prvku vkládanému na stack
                        switch (function->returnDataType) {
                            case sInteger:
                                new_element->token_type = sInteger;
                                break;
                            case sDouble:
                                new_element->token_type = sDouble;
                                break;
                            case sString:
                                new_element->token_type = sString;
                                break;
                        }


                        new_element->pt_index = eFunction;      //V tabulce budeme hledat funkci
                        exp_function = true;                    //Řešíme funkci
                        params = function->parameters.value;    //params drží parametry funkce

                        param_length = (int)strlen(function->parameters.value);     //Počet parametrů
                    }


                }

            }
                //Kontrola parametru, pokud parametr není proměnná
            else if(exp_function && (sString == new_element->token_type || sDouble == new_element->token_type || sInteger == new_element->token_type)){
                if((error_type = checkParams(new_element,new_element->token_type)) != ERROR_CODE_OK)
                    return error_type;
            }


            //Pushne se znak na vrchol stacku, který je zároveň nejvyšší terminál
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
        tStack *stack_item = expression_stack->top_of_stack;
        //operation = 0;

        //Zjistí se, které pravidlo se uplatní pro redukci
        switch (((Exp_element *) (first_terminal->value))->pt_index) {

            //Řeší redukci pro ID (operand)
            case eOperand:
                //Aktuální el. nastavíme jako neterminál a u dalšího prvku, který je nejbližší terminál, zrušíme zarážku
                ((Exp_element *) (stack_item->value))->terminal = false;
                ((Exp_element *) (stack_item->value))->handle = false;
                ((Exp_element *) (stack_item->left->value))->handle = false;
                first_terminal = (stack_item->left);
                //if(operation_type_global != -1) {
                    operand1 = initOperand(operand1, "", sIdentificator, F_LF, true, false, false,
                                           I_DEFAULT);
                    operand2 = initOperand(operand2, ((Exp_element *) (stack_item->value))->value.value,
                                           ((Exp_element *) (stack_item->value))->token_type, F_DEFAULT, false, false,
                                           false, I_DEFAULT);
                    writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);
                //}

                //operation = eOperand;
                return ERROR_CODE_OK;

                //Řeší redukci násobení
            case eMultiply:
                if((error_type = checkBinary(expression_stack, eMultiply)) != ERROR_CODE_OK)
                    return error_type;
                //operation = eMultiply;
                break;
                //Řeší redukci dělení
            case eDivideD:

                if((error_type = checkBinary(expression_stack, eDivideD)) != ERROR_CODE_OK)
                    return error_type;
                //operation = eDivideD;
                break;

                //Řeší redukci celočíselného dělení
            case eDivideI:

                if((error_type = checkBinary(expression_stack, eDivideI)) != ERROR_CODE_OK)
                    return error_type;
                //operation = eDivideI;
                break;

                //Řeší redukci sčítání
            case ePlus:
                if((error_type = checkBinary(expression_stack, ePlus)) != ERROR_CODE_OK)
                    return error_type;
                //##
                operand1 = initOperand(operand1, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
                operand2 = initOperand(operand2, ((Exp_element *) (stack_item->left->left->value))->value.value, ((Exp_element *) (stack_item->left->left->value))->token_type, F_DEFAULT, false, false, false, I_DEFAULT);
                operand3 = initOperand(operand3, ((Exp_element *) (stack_item->value))->value.value, sIdentificator, F_DEFAULT, false, false, false, I_DEFAULT);
                writeInstructionThreeOperands(&instList, I_ADD, operand1, operand2, operand3);
                //operation = ePlus;
                break;

                //Řeší redukci odčítání
            case eMinus:
                if((error_type = checkBinary(expression_stack, eMinus)) != ERROR_CODE_OK)
                    return error_type;
                //generateInstruction(&instList,I_SUB,)
                //operation = eMinus;
                break;

                //Řeší redukci rovnosti
            case eEqual:

                if((error_type = checkBinary(expression_stack, eEqual)) != ERROR_CODE_OK)
                    return error_type;
                //operation = eEqual;
                break;

                //Řeší redukci nerovnosti
            case eNotEqual:
                if((error_type = checkBinary(expression_stack, eNotEqual)) != ERROR_CODE_OK)
                    return error_type;
                //operation = eNotEqual;
                break;

                //Řeší redukci menšítka
            case eLess:
                if((error_type = checkBinary(expression_stack, eLess)) != ERROR_CODE_OK)
                    return error_type;
                //operation = eLess;
                break;

                //Řeší redukci většítka
            case eMore:
                if((error_type = checkBinary(expression_stack, eMore)) != ERROR_CODE_OK)
                    return error_type;
                //operation = eMore;
                break;

                //Řeší redukci menší nebo rovno
            case eLessEqual:
                if((error_type = checkBinary(expression_stack, eLessEqual)) != ERROR_CODE_OK)
                    return error_type;
                //operation = eLessEqual;
                break;

                //Řeší redukci větší nebo rovno
            case eMoreEqual:
                if((error_type = checkBinary(expression_stack, eMoreEqual)) != ERROR_CODE_OK)
                    return error_type;
                //operation = eMoreEqual;
                break;

            case eRightPar:
                if((error_type = reduceFunction(expression_stack)) != ERROR_CODE_OK)
                    return error_type;
                //operation = eFunction;
                return ERROR_CODE_OK;

        }

        //Pokud je binární operace správně, popne pravý neterminál a operátor ze stacku
        SPop(expression_stack);
        SPop(expression_stack);

        //Další první terminál je operátor před neterminálem
        first_terminal = expression_stack->top_of_stack->left;
        ((Exp_element *) (first_terminal->value))->handle = false;
        return error_type;
    }
    else
        return ERROR_CODE_SYN;
}


//Funkce pro kontrolu binárních operátorů
ERROR_CODE checkBinary(ptrStack *expression_stack, int operator){
    if(expression_stack != NULL) {
        if (!exp_function) {
            tStack *stack_item = (expression_stack->top_of_stack);

            //Uložíme si levý a pravý operátor operace pro kontrolu sémantiky
            Exp_element *r_operator = ((Exp_element *) (stack_item->value));

            //Případ, kdy jsou na vstupu dva operatory po sobě
            if (r_operator->pt_index != eOperand)
                return ERROR_CODE_SYN;

            Exp_element *l_operator = ((Exp_element *) (stack_item->left->left->value));

            //Pokud není splněná podmínka: OPERAND OPERATOR OPERAND, jedná se o syntaktickou chybu
            if ((r_operator->terminal != false || eOperand != convertTokenToIndex(r_operator->token_type)) ||
                (((Exp_element *) (stack_item->left->value))->terminal != true ||
                 ((Exp_element *) (stack_item->left->value))->pt_index != operator) ||
                (l_operator->terminal != false || eOperand != convertTokenToIndex(l_operator->token_type)))
                return ERROR_CODE_SYN;

            //Zkontroluje se sémantika operace
            if (checkSemAConv(l_operator, operator, r_operator) != ERROR_CODE_OK)
                return ERROR_CODE_SEM_COMP;

            return ERROR_CODE_OK;
        }
        else
            return ERROR_CODE_SYN;
    }
    else
        return ERROR_CODE_SYN;
}


//Funkce pro redukci závorek
ERROR_CODE reducePars(ptrStack *expression_stack){


    if(expression_stack != NULL){

        tStack *stack_item = expression_stack->top_of_stack;
        tStack *tmp = expression_stack->top_of_stack->left; //Dočasné úložiště pro neterminál mezi závorkami

        if (((Exp_element *) (stack_item->left->value))->terminal == false &&
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
                //operation = eRightPar;
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

//Redukce funkce
ERROR_CODE reduceFunction(ptrStack *expression_stack){
    //Pokud nesedí počet parametrů je to chyba
    if(param_length != parameter_index)
        return ERROR_CODE_SEM_COMP;
    else {
        Exp_element *del_element = ((Exp_element *) (expression_stack->top_of_stack->value));

        //Jelikož kontroluji pouze funkci, popuju dokud nenarazím na dollar
        while (del_element->pt_index != eFunction) {

            SPop(expression_stack);
            del_element = ((Exp_element *) (expression_stack->top_of_stack->value));
        }
        //operation = eFunction;
        ((Exp_element*)expression_stack->top_of_stack->value)->handle = false;
        ((Exp_element*)expression_stack->top_of_stack->value)->terminal = false;
        first_terminal = expression_stack->top_of_stack->left;
        return ERROR_CODE_OK;
    }

}

//Funkce kontroluj parametry funkce
ERROR_CODE checkParams(Exp_element *element,int variable){

    //Jestli má ještě stále co kontrolovat
    if(params[parameter_index] != '\0') {
        switch (params[parameter_index]) {
            case 'i':
                if (variable != sInteger) {
                    if(variable == sDouble){
                        element->token_type = sInteger;
                    }
                    else
                        return ERROR_CODE_SEM_COMP;
                }
                break;
            case 'd':
                if (variable != sDouble) {
                    if (variable == sInteger) {
                        element->token_type = sDouble;
                    } else
                        return ERROR_CODE_SEM_COMP;
                }

                break;
            case 's':
                if (variable != sString)
                    return ERROR_CODE_SEM_COMP;
                break;
        }
        //Zvedne se index pro kontrolu dalšího parametru
        parameter_index++;
    }
    else
        return ERROR_CODE_SEM_COMP;

    return ERROR_CODE_OK;

}


//Funkce kontroluje sémantiku a případně konvertuje typ operatoru
ERROR_CODE checkSemAConv( Exp_element *operand_type_l,int operator, Exp_element *operand_type_r){

    /*
    //Pokud má být výsledek string a jeden z operátorů není string, chyba
    if((operand_type_l->token_type != sString || operand_type_r->token_type != sString) && operation_type_global == sString)
        return ERROR_CODE_SEM_COMP;
*/
    //Jestli se provadi operace '\'
    if(operator == eDivideI){
        //Pokud není ani jeden z operandů string, oba se přetypují na int
        if (operand_type_l->token_type != sString && operand_type_r->token_type != sString){
            operand_type_l->token_type = sInteger;
            operand_type_r->token_type = sInteger;
        }
        else
            return ERROR_CODE_SEM_COMP;
    }

        //Jestli se provádí operace +, která může i konkatenovat dva stringy, nebo operace s log. operatorem
    else if (operator == ePlus || (operator > eMinus && operator <= eMoreEqual)) {

        //Pokud je jeden z operandů string a druhý nikoliv, je to sem. chyba
        if ((operand_type_l->token_type == sString && operand_type_r->token_type != sString) ||
            (operand_type_l->token_type != sString && operand_type_r->token_type == sString)){
            return ERROR_CODE_SEM_COMP;
        }
            //Jestli je jeden z operandů double, druhý se přetypuje na double
        else if(operand_type_l->token_type == sDouble || operand_type_r->token_type == sDouble) {
            operand_type_l->token_type = sDouble;
            operand_type_r->token_type = sDouble;
        }
        else if(operand_type_l->token_type == sString|| operand_type_r->token_type == sString) {
        }
        //Jinak máme v operaci dva stringy...
    }

        //Jestli se provádí operace *,/,-
    else if(operator == eMultiply || operator == eMinus || operator == eDivideD){

        //Jestli je jeden z operandů string, je to sem. chyba
        if (operand_type_l->token_type == sString || operand_type_r->token_type == sString){
            return ERROR_CODE_SEM_COMP;
        }
            //Jestli je jeden z operandů double, druhý se přetypuje na double
        else if(operand_type_l->token_type == sDouble || operand_type_r->token_type == sDouble) {
            operand_type_l->token_type = sDouble;
            operand_type_r->token_type = sDouble;
        }
        //Jinak necháváme původní typ...
    }
    else{
        return ERROR_CODE_SEM_COMP;
    }

    return ERROR_CODE_OK;
}

/* OPRAVIT*/
ERROR_CODE checkResultType(ptrStack *expression_stack){
    if(operation_type_global == sDouble && ((Exp_element*)expression_stack->top_of_stack->value)->token_type != sDouble){
        /*TODO přetypovat proměnnou výsledku*/
    }
    else if(operation_type_global == sInteger && ((Exp_element*)expression_stack->top_of_stack->value)->token_type != sInteger){
        /*TODO přetypovat proměnnou výsledku*/
    }

    if(( ((Exp_element*)expression_stack->top_of_stack->value)->token_type != sString && sString == operation_type_global) ||
            (((Exp_element*)expression_stack->top_of_stack->value)->token_type == sString && sString != operation_type_global))
        return ERROR_CODE_SEM_COMP;
    tmpToken.type=((Exp_element*)expression_stack->top_of_stack->value)->token_type;
    return ERROR_CODE_OK;
}

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

    //Pokud naleži index jednomu z povolených znaků
    if(table_input_index != eOther) {
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

        case sMoreEqual:
            return eMoreEqual;

        case sLeftPar:
            return eLeftPar;

        case sRightPar:
            return eRightPar;

        case sKeyWord:
            return eDollar;

        case sEndOfLine:
            return eDollar;

        case sSemicolon:
            return eDollar;

        case sComma:
            return eComma;
    }
    return eOther;
}
