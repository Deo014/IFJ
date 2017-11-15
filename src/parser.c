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
#include <stdio.h>
#include "parser.h"
#include "symtable.h"
#include "string.h"
#include "instList.h"
#include "scanner.h"
#include <string.h> //doplnit funkce k nam
#include "error_code.h"

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

int Deklarace_prom_a_prikazy();

int Hlavicka_fce();

int Telo_funkce();

int Parametry();

int Typ();

int Dalsi_parametry();

int Deklarace_promennych();

int Prikazy();

int Deklarace_promenne();

int Prirazeni_hodnoty();

int Deklarace_fci_definice_fci();

int Prikaz();



//Pomocna funkce, ktera z obsahu atributu tokenu klicovych slov priradi cislo k pouziti ve switchi
int adjustTokenType(tToken tok) {
    if (tok.type == 3) {
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
        result = ERROR_CODE_LEX;
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
            result = Deklarace_fci_definice_fci();
            if (result != ERROR_CODE_OK) return result;
            result = Telo_programu();
            if (result != ERROR_CODE_OK) return result;
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            // POZOR! Nezapomenout testovat, zda nasleduje konec souboru.
            // Pri oponenuti teto veci by zde mohly pokracovat nejake nesmysly, ktere by se
            // v ramci syntakticke analyzy jiz nezpracovavaly a program by se tvaril, ze je OK
            if (aktualni_token.type != sEndOfFile) return ERROR_CODE_SYN;

            // nagenerujeme instrukci konce programu
            //generateInstruction(list,)

            return ERROR_CODE_OK;
            break;
    }
    return ERROR_CODE_SYN;
}

int Deklarace_fci_definice_fci() {
    int result;
    switch (aktualni_token.type) {
        case sDeclare:

            result = Nekolik_deklaraci_fce();
            if (result != ERROR_CODE_OK) return result;
            result = Deklarace_fci_definice_fci();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
        case sFuntion:
            result = Nekolik_definici_fce();
            if (result != ERROR_CODE_OK) return result;
            result = Deklarace_fci_definice_fci();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
        case sScope:
            return ERROR_CODE_OK;

    }
    return ERROR_CODE_SYN;
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
            if (result != ERROR_CODE_OK) return result;
            // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce Nekolik_deklaraci_fce
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            return Nekolik_deklaraci_fce();
        case sFuntion:
            result = Nekolik_definici_fce();
            // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
            if (result != ERROR_CODE_OK) return result;
        case sScope:

            return ERROR_CODE_OK;
    }

    return ERROR_CODE_SYN;
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
            if (result != ERROR_CODE_OK) return result;
            // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce Nekolik_definici_fce
            return Nekolik_definici_fce();
        case sDeclare:
            return ERROR_CODE_OK;
    }

    return ERROR_CODE_SYN;
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
            if (result != ERROR_CODE_OK) return result;
            // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce Nekolik_definici_fce
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sEnd) return ERROR_CODE_SYN;
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sScope) return ERROR_CODE_SYN;

            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Deklarace_fce() {
    int result;

    switch (aktualni_token.type) {
        // pravidlo <declrList> -> "ID" ";" <declrList>
        case sDeclare:

            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);

            if (aktualni_token.type != sFuntion) return ERROR_CODE_SYN;
            result = Hlavicka_fce();
            // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
            if (result != ERROR_CODE_OK) return result;
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);

            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            return ERROR_CODE_OK;
    }
    // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
    return ERROR_CODE_SYN;
}

int Definice_fce() {
    int result;
    switch (aktualni_token.type) {

        case sFuntion:
            result = Hlavicka_fce();
            if (result != ERROR_CODE_OK) return result;
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Telo_funkce();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEnd) return ERROR_CODE_SYN;
            aktualni_token = getNextToken();
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sFuntion) return ERROR_CODE_SYN;
            aktualni_token = getNextToken();
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            aktualni_token = getNextToken();
            aktualni_token.type = adjustTokenType(aktualni_token);
            return ERROR_CODE_OK;
    }
    // pokud aktualni token je jiny nez vyse uvedene, jedna se o syntaktickou chybu
    return ERROR_CODE_SYN;
}

int Hlavicka_fce() {
    int result;
    aktualni_token = getNextToken();
    //Uprava tokenu klicoveho slova na konkretni klicove slovo
    aktualni_token.type = adjustTokenType(aktualni_token);
    if (aktualni_token.type != sIdentificator) return ERROR_CODE_SYN;
    aktualni_token = getNextToken();
    //Uprava tokenu klicoveho slova na konkretni klicove slovo
    aktualni_token.type = adjustTokenType(aktualni_token);
    if (aktualni_token.type != sLeftPar) return ERROR_CODE_SYN;
    aktualni_token = getNextToken();
    //Uprava tokenu klicoveho slova na konkretni klicove slovo
    aktualni_token.type = adjustTokenType(aktualni_token);
    result = Parametry();
    if (result != ERROR_CODE_OK) return result;

    if (aktualni_token.type != sRightPar) return ERROR_CODE_SYN;
    aktualni_token = getNextToken();
    //Uprava tokenu klicoveho slova na konkretni klicove slovo
    aktualni_token.type = adjustTokenType(aktualni_token);
    if (aktualni_token.type != sAs) return ERROR_CODE_SYN;

    aktualni_token = getNextToken();
    //Uprava tokenu klicoveho slova na konkretni klicove slovo
    aktualni_token.type = adjustTokenType(aktualni_token);
    result = Typ();
    if (result != ERROR_CODE_OK) return result;
    return ERROR_CODE_OK;
}

int Typ() {
    if (aktualni_token.type == tInteger || aktualni_token.type == tDouble || aktualni_token.type == tString)
        return ERROR_CODE_OK;
    else
        return ERROR_CODE_SYN;
}

int Parametry() {
    int result;

    switch (aktualni_token.type) {

        case sIdentificator:
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sAs) return ERROR_CODE_SYN;
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            result = Typ();
            if (result != ERROR_CODE_OK) return result;

            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            result = Dalsi_parametry();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
        case sRightPar:
            return ERROR_CODE_OK;

    }
    return ERROR_CODE_SYN;
}

int Dalsi_parametry() {
    int result;
    switch (aktualni_token.type) {
        case sComma:
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            result = Parametry();
            if (result != ERROR_CODE_OK) return result;
            return Dalsi_parametry();
        case sRightPar:
            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
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
            if (result != ERROR_CODE_OK) return result;
            result = Prikazy();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;

    }
    return ERROR_CODE_SYN;
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
            if (result != ERROR_CODE_OK) return result;
            // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce Nekolik_definici_fce
            return Deklarace_promennych();
        case sScope:
        case sEnd:
            return ERROR_CODE_OK;
    }

    return ERROR_CODE_SYN;
}

int Prikazy() {
    int result;
    switch (aktualni_token.type)
        //Nekolik_definici_fce-><Definice_fce><Nekolik_definici_fce>
    {
        case sDim:
        case sPrint:
        case sInput:
        case sIf:
        case sDo:
        case sIdentificator:
        case sReturn:
            // nejprve zavolame funkci Definice_fce
            result = Prikaz();
            // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
            if (result != ERROR_CODE_OK) return result;
            // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce Nekolik_definici_fce
            return Prikazy();
        case sElse:
        case sLoop:
        case sEnd:
            return ERROR_CODE_OK;
    }

    return ERROR_CODE_SYN;
}

int Prikaz() {
    int result = 0;
    switch (aktualni_token.type) {
        case sPrint:
            aktualni_token = getNextToken();
            aktualni_token.type = adjustTokenType(aktualni_token);
            //Tady se ocekava vyraz

            aktualni_token = getNextToken();
            aktualni_token.type = adjustTokenType(aktualni_token);
        case sInput:

        case sIf:

        case sDo:

        case sIdentificator:

        case sReturn:

            return result;
    }
    return result;

}


int Deklarace_promenne() {
    int result;
    switch (aktualni_token.type)
        //Nekolik_definici_fce-><Definice_fce><Nekolik_definici_fce>
    {
        case sDim:
            // pokud probehlo vse v poradku, hlasime vysledek, ktery dostaneme od funkce Nekolik_definici_fce
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sIdentificator) return ERROR_CODE_SYN;
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            if (aktualni_token.type != sAs) return ERROR_CODE_SYN;
            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);
            result = Typ();
            if (result != ERROR_CODE_OK) return result;

            aktualni_token = getNextToken();
            //Uprava tokenu klicoveho slova na konkretni klicove slovo
            aktualni_token.type = adjustTokenType(aktualni_token);

            switch (aktualni_token.type)
                //Nekolik_definici_fce-><Definice_fce><Nekolik_definici_fce>
            {
                case sAssignment:
                    // nejprve zavolame funkci Definice_fce
                    result = Prirazeni_hodnoty();
                    // pokud v ramci teto funkce nastala chyba, vracime jeji kod a nepokracujeme dal
                    if (result != ERROR_CODE_OK) return result;
                    break;
                case sEndOfLine:
                    aktualni_token = getNextToken();
                    //Uprava tokenu klicoveho slova na konkretni klicove slovo
                    aktualni_token.type = adjustTokenType(aktualni_token);

            }
            return ERROR_CODE_OK;

    }

    return ERROR_CODE_SYN;

    return result;
}

int Deklarace_prom_a_prikazy() {
    int result;
    switch (aktualni_token.type) {
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
        case sDim:
            result = Deklarace_promennych();
            if (result != ERROR_CODE_OK) return result;
            result = Deklarace_prom_a_prikazy();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
        case sEnd:
        case sScope:
            return ERROR_CODE_OK;

    }
    return ERROR_CODE_SYN;
}

int Prirazeni_hodnoty() {
    int result = 0;
    return result;
}
