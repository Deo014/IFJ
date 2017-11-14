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
#include "symtable.h"
#include "string.h"
#include "instList.h"
#include "scanner.h"
#include <string.h> //doplnit funkce k nam

tSymtable *table;

tDLListInstruction *list;
tToken aktualni_token;
string attr;
int tokenType;

int Program();

int Telo_programu();

int Nekolik_deklaraci_fce();

int Nekolik_definici_fce();

int Deklarace_fce();

int Definice_fce();

//Pomocna funkce, ktera z obsahu atributu tokenu klicovych slov priradi cislo k pouziti ve switchi
int adjustTokenType(tToken tok) {
    if (tok.type == 3) {
        if (strcmp(tok.atr.value, "end") == 0)
            return sEnd;
        if (strcmp(tok.atr.value, "scope") == 1)
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
    }
    return tok.type;
}


int parse(tSymtable *symtable, tDLListInstruction *instrList) {
    int result;
    table = symtable;
    symTableInit(symtable);
    list = instrList;
    aktualni_token = getNextToken();
    //Uprava tokenu klicoveho slova na konkretni klicove slovo
    aktualni_token.type = adjustTokenType(aktualni_token);
    //pokud hned prvni token je chybny
    if (aktualni_token.type == sLexError)
        result = LEX_ERROR;
    else
        result = Program();
    return result;
}

int Program() {
//1<Program> -> <Nekolik_deklaraci_fce><Nekolik_definici_fce><Telo_programu>
    int result;
    switch (aktualni_token.type) {
        case sDeclare:
        case sFuntion:
        case sScope:

            result = Nekolik_deklaraci_fce();
            if (result != SYNTAX_OK) return result;
            result = Nekolik_definici_fce();
            if (result != SYNTAX_OK) return result;
            result = Telo_programu();
            if (result != SYNTAX_OK) return result;

            // POZOR! Nezapomenout testovat, zda nasleduje konec souboru.
            // Pri oponenuti teto veci by zde mohly pokracovat nejake nesmysly, ktere by se
            // v ramci syntakticke analyzy jiz nezpracovavaly a program by se tvaril, ze je OK
            if (aktualni_token.type != sEndOfFile) return SYNTAX_ERROR;

            // nagenerujeme instrukci konce programu
            //generateInstruction(list,)

            return SYNTAX_OK;
            break;
    }
    return SYNTAX_ERROR;
}

int Nekolik_deklaraci_fce() {
    int result;
    switch (aktualni_token.type)
        //Nekolik_deklaraci_fce-><Deklarace_fce><Nekolik_deklaraci_fce>
    {
        case sDeclare:
            // nejprve zavolame funkci Deklarace_fce
            result = Deklarace_fce();
            // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
            if (result != SYNTAX_OK) return result;
            // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce Nekolik_deklaraci_fce
            return Nekolik_deklaraci_fce();
    }

    return SYNTAX_ERROR;
}

int Nekolik_definici_fce() {
    int result;
    switch (aktualni_token.type)
        //Nekolik_definici_fce-><Definice_fce><Nekolik_definici_fce>
    {
        case sFuntion:
            // nejprve zavolame funkci Definice_fce
            result = Definice_fce();
            // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
            if (result != SYNTAX_OK) return result;
            // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce Nekolik_definici_fce
            return Nekolik_definici_fce();
    }

    return SYNTAX_ERROR;
}

int Telo_programu() {
    int result;
    switch (aktualni_token.type)
        //Nekolik_definici_fce-><Definice_fce><Nekolik_definici_fce>
    {
        case sScope:
            // nejprve zavolame funkci Definice_fce
            result = Deklarace_prom_a_prikazy();
            // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
            if (result != SYNTAX_OK) return result;
            // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce Nekolik_definici_fce
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sEnd) return SYNTAX_ERROR;
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sScope) return SYNTAX_ERROR;

            return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

int Deklarace_fce() {
    int result;

    switch (aktualni_token.type) {
        // pravidlo <declrList> -> "ID" ";" <declrList>
        case sDeclare:

            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);

            if (aktualni_token.type != sFuntion) return SYNTAX_ERROR;
            result = Hlavicka_fce();
            // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
            if (result != SYNTAX_OK) return result;
            return SYNTAX_OK;
    }
    // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
    return SYNTAX_ERROR;
}

int Definice_fce() {
    int result;
    switch (aktualni_token.type) {

        case sFuntion:
            result = Hlavicka_fce();
            if (result != SYNTAX_OK) return result;
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sEndOfLine) return SYNTAX_ERROR;
            result = Telo_funkce();
            if (result != SYNTAX_OK) return result;
            aktualni_token = getNextToken();
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sEnd) return SYNTAX_ERROR;
            aktualni_token = getNextToken();
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sFuntion) return SYNTAX_ERROR;
            return SYNTAX_OK;
    }
    // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
    return SYNTAX_ERROR;
}

int Hlavicka_fce() {
    int result;
    aktualni_token = getNextToken();
    //Uprava tokenu klicoveho slova na konkretni klicove slovo
    aktualni_token.type = adjustTokenType(aktualni_token);
    if (aktualni_token.type != sIdentificator) return SYNTAX_ERROR;
    aktualni_token = getNextToken();
    //Uprava tokenu klicoveho slova na konkretni klicove slovo
    aktualni_token.type = adjustTokenType(aktualni_token);
    if (aktualni_token.type != sLeftPar) return SYNTAX_ERROR;
    aktualni_token = getNextToken();
    //Uprava tokenu klicoveho slova na konkretni klicove slovo
    aktualni_token.type = adjustTokenType(aktualni_token);
    switch (aktualni_token.type) {

        case sIdentificator:
            //zkontrolujeme parametry
            result = Parametry();
            if (result != SYNTAX_OK) return result;

            break;
        case sRightPar:
            //Funkce nema parametry

            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sAs) return SYNTAX_ERROR;
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (Typ() != SYNTAX_ERROR) {
                //Zapsat do tabulky symbolu
            }
            return SYNTAX_OK;
    }
}

int Typ() {
    if (aktualni_token.type == sInteger || aktualni_token.type == sString || aktualni_token.type == sDouble)
        return SYNTAX_OK;
    else
        return SYNTAX_ERROR;
}

int Parametry() {
    int result;
    aktualni_token = getNextToken();
    //Uprava tokenu klicoveho slova na konkretni klicove slovo
    aktualni_token.type = adjustTokenType(aktualni_token);
    if (aktualni_token.type != sAs) return SYNTAX_ERROR;
    aktualni_token = getNextToken();
    //Uprava tokenu klicoveho slova na konkretni klicove slovo
    aktualni_token.type = adjustTokenType(aktualni_token);
    result = Typ();
    if (result != SYNTAX_OK) return result;

    aktualni_token = getNextToken();
    //Uprava tokenu klicoveho slova na konkretni klicove slovo
    aktualni_token.type = adjustTokenType(aktualni_token);
    result = Dalsi_parametry();
    if (result != SYNTAX_OK) return result;
    return result;
}

int Dalsi_parametry() {
    int result;
    switch (aktualni_token.type) {
        case sComma:
            result = Parametry();
            if (result != SYNTAX_OK) return result;
            return Dalsi_parametry();
    }
    return SYNTAX_ERROR;
}

int Telo_funkce() {
    int result;
    aktualni_token = getNextToken();
    //Uprava tokenu klicoveho slova na konkretni klicove slovo
    aktualni_token.type = adjustTokenType(aktualni_token);

    switch (aktualni_token.type) {
        case sDim:
        case sPrint:
        case sInput:
        case sIf:
        case sDo:
        case sIdentificator:
        case sReturn:
            result = Deklarace_promennych();
            if (result != SYNTAX_OK) return result;
            result = Prikazy();
            if (result != SYNTAX_OK) return result;
            return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

int Deklarace_promennych() {
    int result;
    switch (aktualni_token.type)
        //Nekolik_definici_fce-><Definice_fce><Nekolik_definici_fce>
    {
        case sDim:
            // nejprve zavolame funkci Definice_fce
            result = Deklarace_promenne();
            // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
            if (result != SYNTAX_OK) return result;
            // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce Nekolik_definici_fce
            return Deklarace_promennych();
    }

    return SYNTAX_ERROR;
}

int Prikazy() {
    int result;
    return result;
}

int Deklarace_promenne() {
    int result;
    switch (aktualni_token.type)
        //Nekolik_definici_fce-><Definice_fce><Nekolik_definici_fce>
    {
        case sDim:
            // nejprve zavolame funkci Definice_fce
            result = Deklarace_promenne();
            // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
            if (result != SYNTAX_OK) return result;
            // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce Nekolik_definici_fce
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sIdentificator) return SYNTAX_ERROR;
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sAs) return SYNTAX_ERROR;
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            result = Typ();
            if (result != SYNTAX_OK) return result;

            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sAssignment) return SYNTAX_ERROR;
            result = Prirazeni_hodnoty();
            // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
            if (result != SYNTAX_OK) return result;
    }

    return SYNTAX_ERROR;

    return result;
}