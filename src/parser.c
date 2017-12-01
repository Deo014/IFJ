/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   parser.c - syntaktický a sémantický analyzátor
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */
#include "parser.h"
#include "instlist.h"
#include "string.h"
#include "scanner.h"
#include <stdlib.h>
#include <ctype.h>

extern tDLListInstruction instList;
extern tSymtable glSymTable;
extern tSymtable table;
tToken aktualni_token;
tBSTNodePtr node;
tBSTNodePtr glNode;
extern tToken next_exp_token;
//Pomocna promenna pro semantickou analyzu
bool comingFromDefinition;
//V tele programu se nastavi na true
bool inScope;
//Promenna, ktera povoluje zavolat adjustTokenType po navratu z vyrazu aby se spravil tokenType
bool exprAdjust = false;
//Pomocna promenna, pro zjisteni, zda vsechny deklarovane funkce byly i definovane
int allDeclaredAreDefined = ERROR_CODE_OK;
//Promenna pro uchovani cisla parametru pri zapisu nebo kontrole
int paramIndex;
//Pomocna promenna, ktera znaci ze funkce byla deklarovana nyni
bool declRecently = false;
//Pomocne parametry pro zapis vice parametru do tabulky
string paramName;
//pomocna promenna pro zapis funkce do tabluky
string functionName;
//Promenna, ktera urcuje, kolik parametru se zapise do tabulky pri prechodu z globalni
int paramsToDeclare;
//Pomocna promenna znacici ze se nachazime v tele funkce
bool inFunctionBody = false;
//Promenna, ktera rika expressionu, jakeho typu by mel byt vyhodnocovany vyraz
int expectedValue;
tToken varToSet;
tToken tmpToken;
string labelIf;
string labelElse;
string labelEndIf;
string labelWhile;
string labelLoop;
string test;

int zanoreniCelkemIf = 0;
int labelAktualniIf = 0;
int zanoreniAktualniIf = 0;

int zanoreniCelkemWhile = 0;
int labelAktualniWhile = 0;
int zanoreniAktualniWhile = 0;

//Pomocna funkce, ktera z obsahu atributu tokenu klicovych slov priradi cislo k pouziti ve switchi
int adjustTokenType(tToken tok) {
    //Upravujeme pouze pokud to mame povoleno nebo ze scanneru prisel stav klicove slovo
    if ((tok.type == 3) || (exprAdjust == true)) {
        exprAdjust = false;
        if (tok.type != 0) {
            if (strcmp(tok.atr.value, "end") == 0)
                return sEnd;
            if (strcmp(tok.atr.value, "scope") == 0)
                return sScope;
            if (strcmp(tok.atr.value, "declare") == 0)
                return sDeclare;
            if (strcmp(tok.atr.value, "function") == 0)
                return sFuntion;
            if (strcmp(tok.atr.value, "as") == 0)
                return sAs;
            if (strcmp(tok.atr.value, "dim") == 0)
                return sDim;
            if (strcmp(tok.atr.value, "print") == 0)
                return sPrint;
            if (strcmp(tok.atr.value, "input") == 0)
                return sInput;
            if (strcmp(tok.atr.value, "if") == 0)
                return sIf;
            if (strcmp(tok.atr.value, "then") == 0)
                return sThen;
            if (strcmp(tok.atr.value, "else") == 0)
                return sElse;
            if (strcmp(tok.atr.value, "do") == 0)
                return sDo;
            if (strcmp(tok.atr.value, "while") == 0)
                return sWhile;
            if (strcmp(tok.atr.value, "loop") == 0)
                return sLoop;
            if (strcmp(tok.atr.value, "return") == 0)
                return sReturn;
            if (strcmp(tok.atr.value, "integer") == 0)
                return tInteger;
            if (strcmp(tok.atr.value, "double") == 0)
                return tDouble;
            if (strcmp(tok.atr.value, "string") == 0)
                return tString;
        }
    }
    return tok.type;
}


void zmenLabel(string actualLabel, char *type, bool inc, int kolik) {
    int value;
    string tmp;
    stringInit(&tmp);
    stringAddChars(&tmp, actualLabel.value);
    char *p;
    p = actualLabel.value;
    while (*p) {
        if (isdigit(*p)) {
            value = strtol(p, &p, 10);
            if (inc)
                value = kolik;
            else
                value = kolik;
        } else {
            p++;
        }
    }

    char *cislo;
    cislo = malloc(10);
    sprintf(cislo, "%d", value);
    stringClear(&actualLabel);

    stringAddChars(&actualLabel, type);
    stringAddChars(&actualLabel, cislo);

    free(cislo);

}


//Funkce projde tabulku a zkontroluje jestli byly vsechny deklarovane funkce i definovane
void checkDefinitionsOfDeclarations(tBSTNodePtr TempTree) {
    if (TempTree != NULL) {
        checkDefinitionsOfDeclarations(TempTree->RPtr);
        if (TempTree->nodeDataType == ndtFunction) {
            //Pokud nejaka funkce ma zaznam a nebyla deklarovana definovana pred vstupem do scopu je to semanticka chyba
            if (!((((tDataFunction *) TempTree->Data)->defined) && (((tDataFunction *) TempTree->Data)->declared)))
                allDeclaredAreDefined = ERROR_CODE_SEM;
        }
        checkDefinitionsOfDeclarations(TempTree->LPtr);
    }
}

//Funkce nacte dalsi token a upravi jeho typ pokud ze scanneru prisel stav klicove slovo
int dalsiToken() {
    aktualni_token = getNextToken();
    if (aktualni_token.type != sLexError) {
        aktualni_token.type = adjustTokenType(aktualni_token);
        return ERROR_CODE_OK;
    } else
        return ERROR_CODE_LEX;
}

int parse() {
    stringInit(&paramName);
    stringInit(&functionName);
    stringInit(&labelIf);
    stringInit(&labelElse);
    stringInit(&labelEndIf);
    stringInit(&labelWhile);
    stringInit(&labelLoop);
    stringInit(&test);
    int result;
    //Nacteni prvniho tokenu
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    if (aktualni_token.type == sLexError)
        result = ERROR_CODE_LEX;
    else
        //Prvni token je v poradku, volame prvni pravidlo
        result = Program();
    stringClear(&paramName);
    stringClear(&functionName);
    stringClear(&labelIf);
    stringClear(&labelElse);
    stringClear(&labelEndIf);
    stringClear(&labelWhile);
    stringClear(&labelLoop);
    stringClear(&test);
    return result;
}

//Funkce na "pozirani" EOLU
int Line() {
    int result;
    switch (aktualni_token.type) {
        case sEndOfLine:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Line();
        default:
            return ERROR_CODE_OK;

    }

    return result;
}

int Program() {
    //<Program> -> <Deklarace_funkci_definice_funkci><Telo_programu><EOF>
    int result;
    switch (aktualni_token.type) {
        //Jeste nez bude prvni deklarace, definice, nebo scope mohou byt prazdne radky
        case sEndOfLine:
            result = Line();
            if (result != ERROR_CODE_OK) return result;
        case sDeclare:
        case sFuntion:
        case sScope:
            //Pred scopem muzou byt deklarace a definice funkci
            result = Deklarace_fci_definice_fci();
            if (result != ERROR_CODE_OK) return result;
            //Vstup do tela programu
            result = Telo_programu();
            if (result != ERROR_CODE_OK) return result;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEndOfFile) return ERROR_CODE_SYN;
            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Deklarace_fci_definice_fci() {
    int result;
    switch (aktualni_token.type) {
        //<Deklarace_funkci_definice_funkci> -> < Nekolik_Deklaraci_fce > <Deklarace_funkci_definice_funkci>
        case sDeclare:
            result = Nekolik_deklaraci_fce();
            if (result != ERROR_CODE_OK) return result;
            result = Deklarace_fci_definice_fci();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
            //<Deklarace_funkci_definice_funkci> -> < Nekolik_Definici_fce > <Deklarace_funkci_definice_funkci>
        case sFuntion:
            result = Nekolik_definici_fce();
            if (result != ERROR_CODE_OK) return result;
            result = Deklarace_fci_definice_fci();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
            //<Deklarace_funkci_definice_funkci> -> e
        case sScope:
            return ERROR_CODE_OK;

    }
    return ERROR_CODE_SYN;
}

int Nekolik_deklaraci_fce() {
    int result;
    switch (aktualni_token.type) {
        //<Nekolik_Deklaraci_fce> -> <Deklarace_fce> <Nekolik_Deklaraci_fce>
        case sDeclare:
            result = Deklarace_fce();
            if (result != ERROR_CODE_OK) return result;

            return Nekolik_deklaraci_fce();
            //<Nekolik_Deklaraci_fce> -> e
        case sFuntion:
        case sScope:
            return ERROR_CODE_OK;
    }

    return ERROR_CODE_SYN;
}

int Nekolik_definici_fce() {
    int result;
    switch (aktualni_token.type) {
        //<Nekolik_Definici_fce> -> < Definice_fce><Nekolik_Definici_fce>
        case sFuntion:
            result = Definice_fce();
            if (result != ERROR_CODE_OK) return result;
            return Nekolik_definici_fce();
            //<Nekolik_Definici_fce> -> e
        case sDeclare:
        case sScope:
            return ERROR_CODE_OK;
    }

    return ERROR_CODE_SYN;
}

int Telo_programu() {
    //<Telo_programu> -> <Scope><Deklarace_promennych_a _prikazy><End><Scope>
    int result;
    switch (aktualni_token.type) {
        case sScope:
            //Pred vstupem do tela programu zkontrolujeme jestli vsechny deklarovane funkce byly i definovany
            checkDefinitionsOfDeclarations(glSymTable.root);
            if ((allDeclaredAreDefined) != ERROR_CODE_OK)
                return ERROR_CODE_SEM;
            //Pomocnou promennou nastavime ze jsme ve scopu
            inScope = true;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            // Generování $$scope
            operand1 = initOperand(operand1, "", sKeyWord, F_DEFAULT, false, false, true, I_DEFAULT);
            writeInstructionOneOperand(&instList, I_LABEL, operand1);
            writeInstructionNoOperand(&instList, I_CREATEFRAME);
            writeInstructionNoOperand(&instList, I_PUSHFRAME);
            //Ve scope budou dekalrace promennych a prikazy
            result = Deklarace_prom_a_prikazy();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEnd) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sScope) return ERROR_CODE_SYN;
            // generování $endscope
            writeInstructionNoOperand(&instList, I_POPFRAME);
            operand1 = initOperand(operand1, "%endscope", sKeyWord, F_DEFAULT, false, true, false, I_DEFAULT);
            writeInstructionOneOperand(&instList, I_LABEL, operand1);
            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Deklarace_fce() {
    //<Deklarace_fce> -> <Declare> <Hlavicka_fce><EOL>
    int result;
    switch (aktualni_token.type) {
        case sDeclare:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sFuntion) return ERROR_CODE_SYN;
            //Pomocnou promennou si nastavime ze jdeme z deklarace ne z definice
            comingFromDefinition = false;
            //A jdeme do hlavicky fce
            result = Hlavicka_fce();
            if (result != ERROR_CODE_OK) return result;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Definice_fce() {
    //<Definice_fce> -> <Hlavicka_fce><EOL><Telo_fce><End><Function><EOL>
    int result;
    switch (aktualni_token.type) {
        case sFuntion:
            //Pomocnou promennou si nastavime ze jdeme z definice ne z deklarace
            comingFromDefinition = 1;
            //A jdeme do hlavicky
            result = Hlavicka_fce();
            operand1 = initOperand(operand1, functionName.value, sKeyWord, F_DEFAULT, false, true, false, I_DEFAULT);
            writeInstructionOneOperand(&instList, I_LABEL, operand1);
            writeInstructionNoOperand(&instList, I_PUSHFRAME);
            //operand1 = initOperand(operand1, "%retval", sIdentificator, F_LF, false, true, false, I_DEFAULT);
            //writeInstructionOneOperand(&instList, I_DEFVAR, operand1);
            if (result != ERROR_CODE_OK) return result;
            //Vlozime funkci do globalni tabulky symbolu
            if ((symTableSearch(&glSymTable, functionName)) != NULL) {
                node = symTableSearch(&glSymTable, functionName);
                //Pokud uz tam je, zkontrolujeme pocet parametru s deklaraci
                if (((tDataFunction *) node->Data)->parameters.length != paramIndex)
                    //Pocet parametru v definici neni stejny jako v deklaraci, takze error
                    return ERROR_CODE_SEM;
            }
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            //Jdeme do tela funkce
            result = Telo_funkce();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEnd) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sFuntion) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Hlavicka_fce() {
    //<Hlavicka_fce> -> <Function><Id><<(><Parametry><)><As><Typ>
    int result;
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    if (aktualni_token.type != sIdentificator) return ERROR_CODE_SYN;
    //Jedna se o definici nebo deklaraci?
    if (comingFromDefinition == true) {
        //Pokus o definici fce
        //Podivame se jestli je uz fce v tabulce
        //Uchovame si v promenne jmeno definovane funkce
        functionName = aktualni_token.atr;
        if ((symTableSearch(&glSymTable, aktualni_token.atr)) != NULL) {
            //V tabulce uz je o teto funkci nejaky zaznam
            node = symTableSearch(&glSymTable, aktualni_token.atr);
            if (((tDataFunction *) node->Data)->defined == true) {
                //Uz byla definovana, pokus o redefinici -> error
                return ERROR_CODE_SEM;
            } else {
                //Byla deklarovana, nastavime ze uz bude i definovana
                ((tDataFunction *) node->Data)->defined = true;
            }
        } else {
            //Neni v tabulce, a jdu z definice, vlozim a reknu ze je od ted definovana a deklarovana
            symTableInsertFunction(&glSymTable, aktualni_token.atr);
            node = symTableSearch(&glSymTable, aktualni_token.atr);
            ((tDataFunction *) node->Data)->declared = true;
            //Kvuli kontrole parametru si ulozim do promenne ze byla ted deklarovana/definovana
            declRecently = true;
            ((tDataFunction *) node->Data)->defined = true;

        }
    } else {
        //Comingfromdefinition==0, takze jsem tu z deklarace
        //Neco o funkci tam je, coz u deklarace nelze
        if ((symTableSearch(&glSymTable, aktualni_token.atr)) != NULL) return ERROR_CODE_SEM;
        else {
            //Jdu z deklarace a fce je deklarovana poprve
            symTableInsertFunction(&glSymTable, aktualni_token.atr);
            node = symTableSearch(&glSymTable, aktualni_token.atr);
            ((tDataFunction *) node->Data)->declared = true;
            declRecently = true;
            //Jdu z deklarace, takze to, ze je definovana nastavim na false
            ((tDataFunction *) node->Data)->defined = false;
        }

    }
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    if (aktualni_token.type != sLeftPar)return ERROR_CODE_SYN;
    //Vynuluju index parametru pro kontrolu nebo vkladani
    paramIndex = 0;
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    //Jdu do parametru
    result = Parametry();
    if (result != ERROR_CODE_OK)return result;
    if (aktualni_token.type != sRightPar)return ERROR_CODE_SYN;
    if (dalsiToken() != ERROR_CODE_OK)return ERROR_CODE_LEX;
    if (aktualni_token.type != sAs)return ERROR_CODE_SYN;
    if (dalsiToken() != ERROR_CODE_OK)return ERROR_CODE_LEX;
    result = Typ();
    if (result != ERROR_CODE_OK)return result;
    //Jeste se musi zaznamenat navratovy typ fce
    if (comingFromDefinition == 1) {
        if (declRecently != true) {
            //Jdu z definice a funkce byla kdysi deklarovana, takze zkontoluju parametry v definici s temi v drivejsi deklaraci
            switch (aktualni_token.type) {
                case tInteger:
                    if ((((tDataFunction *) node->Data)->returnDataType) != sInteger)
                        //Parametr nesouhlasi, takze chyba
                        return ERROR_CODE_SEM;
                    break;

                case tDouble:
                    if ((((tDataFunction *) node->Data)->returnDataType) != sDouble)
                        //Parametr nesouhlasi, takze chyba
                        return ERROR_CODE_SEM;
                    break;

                case tString:
                    if ((((tDataFunction *) node->Data)->returnDataType) != sString)
                        //Parametr nesouhlasi, takze chyba
                        return ERROR_CODE_SEM;
                    break;

            }

        } else {
            //Funkci prave definuji, takze musim zaznamenat i jeji navratovy typ
            switch (aktualni_token.type) {
                case tInteger:
                    ((tDataFunction *) node->Data)->returnDataType = sInteger;
                    break;
                case tDouble:
                    ((tDataFunction *) node->Data)->returnDataType = sDouble;
                    break;
                case tString:
                    ((tDataFunction *) node->Data)->returnDataType = sString;
                    break;
            }

        }
    } else {
        //Jdu z deklarace, tudiz funkce jeste nebyla definovana, takze rovnou zaznamenam jeji navratovy typ
        switch (aktualni_token.type) {
            case tInteger:
                ((tDataFunction *) node->Data)->returnDataType = sInteger;
                break;
            case tDouble:
                ((tDataFunction *) node->Data)->returnDataType = sDouble;
                break;
            case tString:
                ((tDataFunction *) node->Data)->returnDataType = sString;
                break;
        }
    }
//"Vynuluju" promennou, ktera rika ze jsem funkci ted definoval/deklaroval
    declRecently = false;
    return ERROR_CODE_OK;

}

int Typ() {
    switch (aktualni_token.type) {
        //<Typ> -> <Integer>
        //<Typ> -> <Double>
        //<Typ> -> <String>
        case tInteger:
        case tDouble:
        case tString:
            //Pokud je token integer,double nebo string, je to ok
            return ERROR_CODE_OK;
    }
    //Je to neco jineho, takze chyba
    return ERROR_CODE_SYN;
}

int Parametry() {
    int result;
    switch (aktualni_token.type) {
        //<Parametry> -> <Id><As><Typ><Dalsi_parametry>
        case sIdentificator:
            paramName = aktualni_token.atr;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sAs) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Typ();
            if (result != ERROR_CODE_OK) return result;
            if (comingFromDefinition == 0) {
                //Jdu z deklarace, parametry nekontroluju jen zapisu, protoze zadny zaznam o funkci jeste neni
                switch (aktualni_token.type) {
                    case tInteger:
                        stringAddChar(&((tDataFunction *) node->Data)->parameters, 'i');
                        //Zkontroluji jestli se nektery z predchozich parametru nejmenuje stejne jako ten, co ted vkladam
                        for (int i = 0; i < paramIndex; i++) {
                            if (stringCompare(&((tDataFunction *) node->Data)->paramName[i], &paramName) == true)
                                return ERROR_CODE_SEM;

                        }
                        //Kontrola, jestli se parametr nejmenuje stejne jako nejaka funkce
                        if (symTableSearch(&glSymTable, paramName) != NULL)
                            return ERROR_CODE_SEM;
                        //Jmeno je ok, vlozim ho
                        ((tDataFunction *) node->Data)->paramName[paramIndex] = paramName;
                        break;
                    case tDouble:
                        stringAddChar(&((tDataFunction *) node->Data)->parameters, 'd');
                        //Zkontroluji jestli se nektery z predchozich parametru nejmenuje stejne jako ten, co ted vkladam
                        for (int i = 0; i < paramIndex; i++) {
                            if (stringCompare(&((tDataFunction *) node->Data)->paramName[i], &paramName) == true)
                                return ERROR_CODE_SEM;

                        }
                        //Kontrola, jestli se parametr nejmenuje stejne jako nejaka funkce
                        if (symTableSearch(&glSymTable, paramName) != NULL)
                            return ERROR_CODE_SEM;
                        ((tDataFunction *) node->Data)->paramName[paramIndex] = paramName;
                        break;
                    case tString:
                        stringAddChar(&((tDataFunction *) node->Data)->parameters, 's');
                        //Zkontroluji jestli se nektery z predchozich parametru nejmenuje stejne jako ten, co ted vkladam
                        for (int i = 0; i < paramIndex; i++) {
                            if (stringCompare(&((tDataFunction *) node->Data)->paramName[i], &paramName) == true)
                                return ERROR_CODE_SEM;

                        }
                        //Kontrola, jestli se parametr nejmenuje stejne jako nejaka funkce
                        if (symTableSearch(&glSymTable, paramName) != NULL)
                            return ERROR_CODE_SEM;
                        ((tDataFunction *) node->Data)->paramName[paramIndex] = paramName;
                        break;
                }
            } else {

                //Jdu z definice a funkce byla deklarovana, musim zkontrolovat, jestli souhlasi typy parametru
                if (!declRecently) {
                    switch (aktualni_token.type) {
                        case tInteger:
                            //  stringAddChar(,'a');
                            if (((tDataFunction *) node->Data)->parameters.value[paramIndex] != 'i')

                                return ERROR_CODE_SEM;
                            ((tDataFunction *) node->Data)->paramName[paramIndex] = paramName;
                            break;
                        case tDouble:
                            if (((tDataFunction *) node->Data)->parameters.value[paramIndex] != 'd')
                                return ERROR_CODE_SEM;
                            ((tDataFunction *) node->Data)->paramName[paramIndex] = paramName;
                            break;
                        case tString:
                            if (((tDataFunction *) node->Data)->parameters.value[paramIndex] != 's')
                                return ERROR_CODE_SEM;
                            ((tDataFunction *) node->Data)->paramName[paramIndex] = paramName;
                            break;
                    }


                } else {
                    switch (aktualni_token.type) {
                        case tInteger:
                            //  stringAddChar(,'a');
                            stringAddChar(&((tDataFunction *) node->Data)->parameters, 'i');
                            //Zkontroluji jestli se nektery z predchozich parametru nejmenuje stejne jako ten, co ted vkladam
                            for (int i = 0; i < paramIndex; i++) {
                                if (stringCompare(&((tDataFunction *) node->Data)->paramName[i], &paramName) == true)
                                    return ERROR_CODE_SEM;

                            }
                            //Kontrola, jestli se parametr nejmenuje stejne jako nejaka funkce
                            if (symTableSearch(&glSymTable, paramName) != NULL)
                                return ERROR_CODE_SEM;
                            ((tDataFunction *) node->Data)->paramName[paramIndex] = paramName;
                            break;
                        case tDouble:
                            stringAddChar(&((tDataFunction *) node->Data)->parameters, 'd');
                            //Zkontroluji jestli se nektery z predchozich parametru nejmenuje stejne jako ten, co ted vkladam
                            for (int i = 0; i < paramIndex; i++) {
                                if (stringCompare(&((tDataFunction *) node->Data)->paramName[i], &paramName) == true)
                                    return ERROR_CODE_SEM;

                            }
                            //Kontrola, jestli se parametr nejmenuje stejne jako nejaka funkce
                            if (symTableSearch(&glSymTable, paramName) != NULL)
                                return ERROR_CODE_SEM;
                            ((tDataFunction *) node->Data)->paramName[paramIndex] = paramName;
                            break;
                        case tString:
                            stringAddChar(&((tDataFunction *) node->Data)->parameters, 's');
                            //Zkontroluji jestli se nektery z predchozich parametru nejmenuje stejne jako ten, co ted vkladam
                            for (int i = 0; i < paramIndex; i++) {
                                if (stringCompare(&((tDataFunction *) node->Data)->paramName[i], &paramName) == true)
                                    return ERROR_CODE_SEM;

                            }
                            //Kontrola, jestli se parametr nejmenuje stejne jako nejaka funkce
                            if (symTableSearch(&glSymTable, paramName) != NULL)
                                return ERROR_CODE_SEM;
                            ((tDataFunction *) node->Data)->paramName[paramIndex] = paramName;
                            break;
                    }
                }
            }
            //Pri pristim pruchodu se bude resit dalsi parametr v poradi, takze inkrementurju jeho index
            paramIndex++;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Dalsi_parametry();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
            //<Parametry> -> e
        case sRightPar:
            //Prava zavorka, takze jsou vsechny parametry zpracovane, nuluju index
            paramIndex = 0;
            return ERROR_CODE_OK;

    }
    return ERROR_CODE_SYN;
}

int Dalsi_parametry() {
    int result;
    switch (aktualni_token.type) {
        //<Dalsi_parametry> -> <,><Parametry><Dalsi_parametry>
        case sComma:
            //Carka, takze bude dalsi parametr
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Parametry();
            if (result != ERROR_CODE_OK) return result;
            return Dalsi_parametry();
            //<Dalsi_parametry> -> e
        case sRightPar:
            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Telo_funkce() {
    //<Telo_fce> -> <Deklarace_promennych_a _prikazy>
    int result;
    //Jdu do tela funkce, vytvorim lokalni tabulku symbolu
    symTableInit(&table);
    //V globalni tabulce si najdu funkci do ktere vstupuju
    glNode = symTableSearch(&glSymTable, functionName);
//Podivam se, kolik ma funkce do ktere vstupuju parametru v globalni tabulce
    paramsToDeclare = ((tDataFunction *) glNode->Data)->parameters.length;
    for (int i = 0; i < paramsToDeclare; i++) {
        //Jmena parametru, ktere ma funkce v globalni tabulce zkopiruju jako lokalni promenne do tabluky od funkce
        symTableInsertVariable(&table, ((tDataFunction *) glNode->Data)->paramName[i]);
        node = symTableSearch(&table, ((tDataFunction *) glNode->Data)->paramName[i]);
        switch (((tDataFunction *) glNode->Data)->parameters.value[i]) {
            case 'i':
                ((tDataVariable *) node->Data)->dataType = sInteger;
                break;
            case 'd':
                ((tDataVariable *) node->Data)->dataType = sDouble;
                break;
            case 's':
                ((tDataVariable *) node->Data)->dataType = sString;
                break;
        }


    }
    inFunctionBody = true;
    result = Deklarace_prom_a_prikazy();
    if (result != ERROR_CODE_OK) return result;
    //Opoustim telo funkce, zmenim promennou ktera to indikuje
    inFunctionBody = false;
    return ERROR_CODE_OK;
}

int Deklarace_promennych() {
    int result;
    switch (aktualni_token.type) {
        //< Deklarace _promennych> ->< Deklarace_promenne> <Deklarace_promennych>
        case sDim:
            result = Deklarace_promenne();
            if (result != ERROR_CODE_OK) return result;
            return Deklarace_promennych();
            //< Deklarace _promennych> -> e
        case sScope:
        case sEnd:
        case sIdentificator:
        case sEndOfLine:
        case sPrint:
        case sInput:
        case sIf:
        case sDo:
        case sReturn:
            return ERROR_CODE_OK;
    }

    return ERROR_CODE_SYN;
}

int Prikazy() {
    int result;
    switch (aktualni_token.type) {
        //<Prikazy> -> <Prikaz><Prikazy>
        case sPrint:
        case sInput:
        case sIf:
        case sDo:
        case sIdentificator:
        case sReturn:
            result = Prikaz();
            if (result != ERROR_CODE_OK) return result;
            return Prikazy();
            //<Prikazy> -> e
        case sEndOfLine:
        case sElse:
        case sLoop:
        case sEnd:
        case sDim:
        case sScope:
            return ERROR_CODE_OK;
    }

    return ERROR_CODE_SYN;
}

int Prikaz() {
    int result = 0;
    switch (aktualni_token.type) {
        //<Prikaz> -> <Print><Vyraz><;><Dalsi_vyrazy><EOL>
        case sPrint:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            //Tisknout muzu jakykoliv typ vyrazu bez kontroly, nastavim promennou na -1 jako signal expressionu,
            //ze ho nemusi kontrolovat
            expectedValue = -1;
            result = Vyraz();
            //print
            operand1 = initOperand(operand1, "", sIdentificator, F_LF, true, false, false, I_DEFAULT);
            writeInstructionOneOperand(&instList, I_WRITE, operand1);

            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sSemicolon) return ERROR_CODE_SYN;
            if (result != ERROR_CODE_OK) return result;
            result = Dalsi_vyrazy();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            break;
            //<Prikaz> -> <Input><Id><EOL>
        case sInput:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sIdentificator) return ERROR_CODE_SYN;
            //Promenna musi byt v tabulce symbolu abychom do ni mohli neco prirazovat
            if (!inScope) {
                //Nejsem ve scope, takze se divam do lokalni tabulky
                if (!((symTableSearch(&table, aktualni_token.atr)) != NULL)) return ERROR_CODE_SEM;
                node = symTableSearch(&table, aktualni_token.atr);

            } else {
                //Jsem ve scope, divam se do globalni tabulky
                if (!((symTableSearch(&glSymTable, aktualni_token.atr)) != NULL)) return ERROR_CODE_SEM;
                node = symTableSearch(&glSymTable, aktualni_token.atr);
            }

            //Overeni ze klic co jsme nasli je promenna a ne funkce, protze do funkce nic prirazovat nechci
            if (node->nodeDataType != ndtVariable)
                return ERROR_CODE_SEM;

            operand1 = initOperand(operand1, "?\\032", sString, F_LF, false, false, false, F_DEFAULT);
            writeInstructionOneOperand(&instList, I_WRITE, operand1);
 
            operand1 = initOperand(operand1, aktualni_token.atr.value, sIdentificator, F_LF, false, false, false,
                                   F_DEFAULT);
            operand2 = initOperand(operand2, "", sIdentificator, F_DEFAULT, false, false, false,
                                   ((tDataVariable *) node->Data)->dataType);
            writeInstructionTwoOperands(&instList, I_READ, operand1, operand2);

/*
            if (expectedValue == sInteger) {
                operand1 = initOperand(operand1, varToSet.atr.value, sIdentificator, F_LF, false, false, false,
                                       I_DEFAULT);
                operand2 = initOperand(operand2, "0", sInteger, F_DEFAULT, false, false, false, I_DEFAULT);
                writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);
            } else if (expectedValue == sDouble) {
                operand1 = initOperand(operand1, varToSet.atr.value, sIdentificator, F_LF, false, false, false,
                                       I_DEFAULT);
                operand2 = initOperand(operand2, "0", sDouble, F_DEFAULT, false, false, false, I_DEFAULT);
                writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);
            } else if (expectedValue == sString) {
                operand1 = initOperand(operand1, varToSet.atr.value, sIdentificator, F_LF, false, false, false,
                                       I_DEFAULT);
                operand2 = initOperand(operand2, "\0", sString, F_DEFAULT, false, false, false, I_DEFAULT);
                writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);
            }

*/
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            break;
            //<Prikaz> -> <If><Vyraz><Then><EOL><Prikazy><Else><EOL><Prikazy><End><If><EOL>
        case sIf:
            if (zanoreniAktualniIf == 0) {
                labelAktualniIf += zanoreniCelkemIf;
                zmenLabel(labelIf, "if", true, zanoreniCelkemIf);
                zmenLabel(labelElse, "else", true, zanoreniCelkemIf);
                zmenLabel(labelEndIf, "endif", true, zanoreniCelkemIf);
            }
            zanoreniAktualniIf++;
            labelAktualniIf++;
            zanoreniCelkemIf++;
            zmenLabel(labelIf, "if", true, labelAktualniIf);
            zmenLabel(labelElse, "else", true, labelAktualniIf);
            zmenLabel(labelEndIf, "endif", true, labelAktualniIf);






            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;

            //Pokud je aktualni token integer, nastavim promennou na integer
            // vypsání unikátního ifu
            operand1 = initOperand(operand1, labelIf.value, sKeyWord, F_DEFAULT, false, true, false, I_DEFAULT);
            writeInstructionOneOperand(&instList, I_LABEL, operand1);
            if (aktualni_token.type == sInteger)
                expectedValue = sInteger;
                //Pokud je aktualni token double, nastavim promennou na double
            else if (aktualni_token.type == sDouble)
                expectedValue = sDouble;
            else
                //Pokud to neni ani integer ani double nebudu to kontrolovat
                expectedValue = -1;
            result = Vyraz();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sThen) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;


            // výraz je false, skočí na else
            operand1 = initOperand(operand1, labelElse.value, sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
            operand2 = initOperand(operand2, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
            operand3 = initOperand(operand3, "false", 42, F_TF, false, false, false, I_DEFAULT);
            writeInstructionThreeOperands(&instList, I_JUMPIFEQ, operand1, operand2, operand3);
            result = Prikazy();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sElse) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;

            // výraz je true, provedly se příkazy za then, else se přeskočí



            operand1 = initOperand(operand1, labelEndIf.value, sIdentificator, F_DEFAULT, false, true, false,
                                   I_DEFAULT);
            writeInstructionOneOperand(&instList, I_JUMP, operand1);
            operand1 = initOperand(operand1, labelElse.value, sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
            writeInstructionOneOperand(&instList, I_LABEL, operand1);
            result = Prikazy();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEnd) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sIf) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;

            // vygeneruje labl endifu
            operand1 = initOperand(operand1, labelEndIf.value, sIdentificator, F_DEFAULT, false, true, false,
                                   I_DEFAULT);
            writeInstructionOneOperand(&instList, I_LABEL, operand1);
            labelAktualniIf--;
            zanoreniAktualniIf--;
            zmenLabel(labelIf, "if", true, labelAktualniIf);
            zmenLabel(labelElse, "else", true, labelAktualniIf);
            zmenLabel(labelEndIf, "endif", true, labelAktualniIf);
            break;
            //<Prikaz> -> <Do><While><Vyraz><EOL><Prikazy><Loop><EOL>
        case sDo:
            if (zanoreniAktualniWhile == 0) {
                labelAktualniWhile += zanoreniCelkemWhile;
                zmenLabel(labelWhile, "while", true, zanoreniCelkemWhile);
                zmenLabel(labelLoop, "loop", true, zanoreniCelkemWhile);
            }
            zanoreniAktualniWhile++;
            labelAktualniWhile++;
            zanoreniCelkemWhile++;
            zmenLabel(labelWhile, "while", true, labelAktualniWhile);
            zmenLabel(labelLoop, "loop", true, labelAktualniWhile);

            // vygenerování labelu dowhile
            operand1 = initOperand(operand1, labelWhile.value, sIdentificator, F_DEFAULT, false, true, false,
                                   I_DEFAULT);
            writeInstructionOneOperand(&instList, I_LABEL, operand1);
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sWhile) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            //Pokud je aktualni token integer, nastavim promennou na integer
            if (aktualni_token.type == sInteger)
                expectedValue = sInteger;
                //Pokud je aktualni token double, nastavim promennou na double
            else if (aktualni_token.type == sDouble)
                expectedValue = sDouble;
            else
                expectedValue = -1;

            result = Vyraz();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            // Vygenerování podmíněné skoku na konec cyklu
            operand1 = initOperand(operand1, labelLoop.value, sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
            operand2 = initOperand(operand2, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
            operand3 = initOperand(operand3, "false", 42, F_TF, false, false, false, I_DEFAULT);
            writeInstructionThreeOperands(&instList, I_JUMPIFEQ, operand1, operand2, operand3);

            result = Prikazy();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sLoop) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            // vygenerování labelu loop
            operand1 = initOperand(operand1, labelWhile.value, sIdentificator, F_DEFAULT, false, true, false,
                                   I_DEFAULT);
            writeInstructionOneOperand(&instList, I_JUMP, operand1);
            operand1 = initOperand(operand1, labelLoop.value, sIdentificator, F_DEFAULT, false, true, false, I_DEFAULT);
            writeInstructionOneOperand(&instList, I_LABEL, operand1);

            labelAktualniWhile--;
            zanoreniAktualniWhile--;
            zmenLabel(labelWhile, "while", true, labelAktualniWhile);
            zmenLabel(labelLoop, "loop", true, labelAktualniWhile);
            break;
            //<Prikaz> -> <Id><=><Vyraz><EOL>
        case sIdentificator:
            if (!inScope) {
                //Budu prirazovat do promenne a jsem ve funkci, podivam se jestli je v lokalni tabulce
                if (!((symTableSearch(&table, aktualni_token.atr)) != NULL)) return ERROR_CODE_SEM;
                node = symTableSearch(&table, aktualni_token.atr);
                //Nastavim odpovidajici datovy typ aby mohl expression zkontrolovat jestli to lze po vyhodnoceni
                //vyrazu nastavit
                expectedValue = ((tDataVariable *) node->Data)->dataType;
                varToSet = aktualni_token;

            } else {
                //Budu prirazovat do promenne a jsem ve scopu, podivam se jestli je v globalni tabulce
                if (!((symTableSearch(&glSymTable, aktualni_token.atr)) != NULL)) return ERROR_CODE_SEM;
                node = symTableSearch(&glSymTable, aktualni_token.atr);
                //Nastavim odpovidajici datovy typ aby mohl expression zkontrolovat jestli to lze po vyhodnoceni
                //vyrazu nastavit
                expectedValue = ((tDataVariable *) node->Data)->dataType;
                varToSet = aktualni_token;
            }
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sAssignment) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;

            result = Vyraz();
            // z tmp do varToSet
            operand1 = initOperand(operand1, varToSet.atr.value, varToSet.type, F_LF, false, false, false, I_DEFAULT);
            operand2 = initOperand(operand2, "", sIdentificator, F_LF, true, false, false, I_DEFAULT);
            writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            break;
            //<Prikaz> -> <Return><Vyraz><EOL>
        case sReturn:


            if (inFunctionBody == false)
                return ERROR_CODE_SYN;
            //V hlavnim tele scope nemuze return byt
            if (inScope == true)
                return ERROR_CODE_SEM;
//Podivam se do globalni tabulky na typ funkce ze ktere chci provest return at vim jakeho typu ma byt vyraz za returnem
            if (((symTableSearch(&glSymTable, functionName)) == NULL)) return ERROR_CODE_SEM;
            node = symTableSearch(&glSymTable, functionName);
            //Podle typu funkce nastavim pro expression promennou at vi s cim porovnavat vyhodnoceny typ vyrazu a pripadne
            //muze zahlasit ze to nelze
            expectedValue = ((tDataFunction *) node->Data)->returnDataType;


            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Vyraz();
            if (result != ERROR_CODE_OK) return result;
            //operand1 = initOperand(operand1, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
            //operand1 = initOperand(operand1, "", sIdentificator, F_DEFAULT, true, false, false, I_DEFAULT);
            //writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);
            writeInstructionNoOperand(&instList, I_RETURN);
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            break;
    }
    return result;

}

int Deklarace_promenne() {
    //<Deklarace_promenne> -> <Dim><Id><As><Typ><Prirazeni_hodnoty>
    int result;
    switch (aktualni_token.type) {
        case sDim:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sIdentificator) return ERROR_CODE_SYN;


            exprAdjust = true;
            varToSet = aktualni_token;
            varToSet.type = adjustTokenType(varToSet);

            if (inScope) {
                //Jsem ve scopu a chci vkladat promennou, podivam se jestli uz neni v globalni tabulce
                if ((symTableSearch(&glSymTable, aktualni_token.atr)) != NULL) return ERROR_CODE_SEM;
                //Nebyla, vlozime ju
                symTableInsertVariable(&glSymTable, aktualni_token.atr);
                node = symTableSearch(&glSymTable, aktualni_token.atr);
            } else {
                //Jsem ve funkci a chci vkladat promennou, podivam se jestli uz neni v lokalni tabulce
                if ((symTableSearch(&table, aktualni_token.atr)) != NULL) return ERROR_CODE_SEM;
                //Nebyla, vlozime ju
                symTableInsertVariable(&table, aktualni_token.atr);
                node = symTableSearch(&table, aktualni_token.atr);
            }
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sAs) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Typ();
            if (result != ERROR_CODE_OK) return result;
            //Nastavim typ promenne
            switch (aktualni_token.type) {
                case tInteger:
                    ((tDataVariable *) node->Data)->dataType = sInteger;
                    break;
                case tDouble:
                    ((tDataVariable *) node->Data)->dataType = sDouble;
                    break;
                case tString:
                    ((tDataVariable *) node->Data)->dataType = sString;
                    break;
            }
            // Vypsání vytvoření proměnné
            operand1 = initOperand(operand1, varToSet.atr.value, sIdentificator, F_LF, false, false, false, I_DEFAULT);
            writeInstructionOneOperand(&instList, I_DEFVAR, operand1);

            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            //Rovnou si ulozim jeji typ i do promenne pro expression, kdyby se do ni hned prirazovalo
            expectedValue = ((tDataVariable *) node->Data)->dataType;
            switch (aktualni_token.type) {
                case sAssignment:
                    result = Prirazeni_hodnoty();
                    if (result != ERROR_CODE_OK) return result;
                    result = Line();
                    if (result != ERROR_CODE_OK) return result;
                    return ERROR_CODE_OK;
                case sEndOfLine:
                    result = Line();
                    if (expectedValue == sInteger) {
                        operand1 = initOperand(operand1, varToSet.atr.value, sIdentificator, F_LF, false, false, false,
                                               I_DEFAULT);
                        operand2 = initOperand(operand2, "0", sInteger, F_DEFAULT, false, false, false, I_DEFAULT);
                        writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);
                    } else if (expectedValue == sDouble) {
                        operand1 = initOperand(operand1, varToSet.atr.value, sIdentificator, F_LF, false, false, false,
                                               I_DEFAULT);
                        operand2 = initOperand(operand2, "0", sDouble, F_DEFAULT, false, false, false, I_DEFAULT);
                        writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);
                    } else if (expectedValue == sString) {
                        operand1 = initOperand(operand1, varToSet.atr.value, sIdentificator, F_LF, false, false, false,
                                               I_DEFAULT);
                        operand2 = initOperand(operand2, "\0", sString, F_DEFAULT, false, false, false, I_DEFAULT);
                        writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);
                    }


                    if (result != ERROR_CODE_OK) return result;
                    return ERROR_CODE_OK;

            }
            return ERROR_CODE_SYN;

    }

    return ERROR_CODE_SYN;
}

int Deklarace_prom_a_prikazy() {
    int result;
    switch (aktualni_token.type) {
        //< Deklarace_promennych_a _prikazy >-> <Prikazy><Deklarace_promennych_a _prikazy>
        case sPrint:
        case sInput:
        case sIf:
        case sDo:
        case sIdentificator:
        case sReturn:
            result = Prikazy();
            if (result != ERROR_CODE_OK) return result;
            result = Deklarace_prom_a_prikazy();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
            //< Deklarace_promennych_a _prikazy> -> <Deklarace_promennych> <Deklarace_promennych_a _prikazy>
        case sDim:
            result = Deklarace_promennych();
            if (result != ERROR_CODE_OK) return result;
            result = Deklarace_prom_a_prikazy();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
            //<Deklarace_promennych_a _prikazy> -> e
        case sEnd:
        case sScope:
            return ERROR_CODE_OK;

    }
    return ERROR_CODE_SYN;
}

int Prirazeni_hodnoty() {
    int result;
    switch (aktualni_token.type) {
        //<Prirazeni_hodnoty> -><=><Vyraz>
        case sAssignment:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Vyraz();
            operand1 = initOperand(operand1, varToSet.atr.value, varToSet.type, F_LF, false, false, false, I_DEFAULT);
            operand2 = initOperand(operand2, "", sIdentificator, F_LF, true, false, false, I_DEFAULT);
            writeInstructionTwoOperands(&instList, I_MOVE, operand1, operand2);
            return result;
            //<Prirazeni_hodnoty> -> e
        case sEndOfLine:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            return ERROR_CODE_OK;

    }
    return ERROR_CODE_SYN;
}


int Dalsi_vyrazy() {
    int result;
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    switch (aktualni_token.type) {
        //<Dalsi_vyrazy> -> <Vyraz> <;><Dalsi_vyrazy>
        case sMinus:
        case sIdentificator:
        case sInteger:
        case sDouble:
        case sString:
            result = Vyraz();
            if (result != ERROR_CODE_OK) return result;
            operand1 = initOperand(operand1, "", sIdentificator, F_LF, true, false, false, I_DEFAULT);
            writeInstructionOneOperand(&instList, I_WRITE, operand1);
            result = Dalsi_vyrazy();
            if (result != ERROR_CODE_OK) return result;
            //<Dalsi_vyrazy> -> e
        case sEndOfLine:
        case sSemicolon:

            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Vyraz() {
    int result;
    //Zavolam funkci, ktera vyhodnoti vyraz s aktualnim tokenem a hodnotou, do ktere se vyraz pripadne bude ukladat
    result = expression(aktualni_token, expectedValue);
    //Nastavim si promennou, ktera docasne povoli upravu typu tokenu, kdyby expression pouzival jine typy
    exprAdjust = true;
    //Do aktualniho tokenu si nactu ten, se kterym naposledy exprssion pracoval
    aktualni_token = next_exp_token;
    //Pokud je to potreba upravim jeho typ
    aktualni_token.type = adjustTokenType(aktualni_token);
    return result;
}