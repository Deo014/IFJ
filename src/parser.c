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

//Funkce nacte dalsi token a aktualizuje jeho typ
int dalsiToken() {
    aktualni_token = getNextToken();
    if (aktualni_token.type != sLexError) {
        aktualni_token.type = adjustTokenType(aktualni_token);
        return ERROR_CODE_OK;
    } else
        return ERROR_CODE_LEX;
}

int parse(tSymtable *symtable, tDLListInstruction *instrList) {
    //inicializace tabulky symbolů a instrukčního listu
    int result;
    table = symtable;
    symTableInit(symtable);
    list = instrList;
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    //pokud hned prvni token je chybny
    if (aktualni_token.type == sLexError)
        result = ERROR_CODE_LEX;
    else
        //Prvni token je v poradku, volame prvni pravidlo
        result = Program();
    return result;
}

int Program() {
    int result;
    switch (aktualni_token.type) {
        case sDeclare:
        case sFuntion:
        case sScope:
            result = Deklarace_fci_definice_fci();
            if (result != ERROR_CODE_OK) return result;
            result = Telo_programu();
            if (result != ERROR_CODE_OK) return result;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX; //posledni eol
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX; //eof
            if (aktualni_token.type != sEndOfFile) return ERROR_CODE_SYN;
            return ERROR_CODE_OK;
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
    {
        case sDeclare:
            result = Deklarace_fce();
            if (result != ERROR_CODE_OK) return result;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            return Nekolik_deklaraci_fce();
        case sFuntion:
            result = Nekolik_definici_fce();
            if (result != ERROR_CODE_OK) return result;
        case sScope:

            return ERROR_CODE_OK;
    }

    return ERROR_CODE_SYN;
}

int Nekolik_definici_fce() {
    int result;
    switch (aktualni_token.type)
    {
        case sFuntion:
            result = Definice_fce();
            if (result != ERROR_CODE_OK) return result;
            return Nekolik_definici_fce();
        case sDeclare:
            return ERROR_CODE_OK;
    }

    return ERROR_CODE_SYN;
}

int Telo_programu() {
    int result;

    switch (aktualni_token.type)
    {
        case sScope:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Deklarace_prom_a_prikazy();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEnd) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sScope) return ERROR_CODE_SYN;

            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Deklarace_fce() {
    int result;

    switch (aktualni_token.type) {
        case sDeclare:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sFuntion) return ERROR_CODE_SYN;
            result = Hlavicka_fce();
            if (result != ERROR_CODE_OK) return result;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Definice_fce() {
    int result;
    switch (aktualni_token.type) {

        case sFuntion:
            result = Hlavicka_fce();
            if (result != ERROR_CODE_OK) return result;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Telo_funkce();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEnd) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sFuntion) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Hlavicka_fce() {
    int result;
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    if (aktualni_token.type != sIdentificator) return ERROR_CODE_SYN;
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    if (aktualni_token.type != sLeftPar) return ERROR_CODE_SYN;
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    result = Parametry();
    if (result != ERROR_CODE_OK) return result;
    if (aktualni_token.type != sRightPar) return ERROR_CODE_SYN;
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    if (aktualni_token.type != sAs) return ERROR_CODE_SYN;
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
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
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sAs) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Typ();
            if (result != ERROR_CODE_OK) return result;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
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
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
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
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    result = Deklarace_prom_a_prikazy();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
}

int Deklarace_promennych() {
    int result;
    switch (aktualni_token.type)
    {
        case sDim:
            result = Deklarace_promenne();
            if (result != ERROR_CODE_OK) return result;
            return Deklarace_promennych();
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
    switch (aktualni_token.type)
    {
        case sPrint:
        case sInput:
        case sIf:
        case sDo:
        case sIdentificator:
        case sReturn:

            result = Prikaz();
            if (result != ERROR_CODE_OK) return result;
            return Prikazy();
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
        case sPrint:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Vyraz();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sSemicolon) return ERROR_CODE_SYN;
            result = Dalsi_vyrazy();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            break;
        case sInput:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sIdentificator) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            break;
        case sIf:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Vyraz();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sThen) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Prikazy();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sElse) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Prikazy();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEnd) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sIf) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            break;
        case sDo:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sWhile) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Vyraz();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Prikazy();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sLoop) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            break;
        case sIdentificator:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sAssignment) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Vyraz();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            break;
        case sReturn:
            //TODO Semantikou vyresit aby nemohl byt v hlavnim tele
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Vyraz();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            break;
    }
    return result;

}

int Deklarace_promenne() {
    int result;
    switch (aktualni_token.type)
    {
        case sDim:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sIdentificator) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sAs) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Typ();
            if (result != ERROR_CODE_OK) return result;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            switch (aktualni_token.type)
            {
                case sAssignment:
                    result = Prirazeni_hodnoty();
                    if (result != ERROR_CODE_OK) return result;
                    break;
                case sEndOfLine:
                    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;

            }
            return ERROR_CODE_OK;
    }

    return ERROR_CODE_SYN;
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
    int result;
    switch (aktualni_token.type) {
        case sAssignment:
            result = Vyraz();
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            return result;
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
        case sIdentificator:
        case sInteger:
        case sDouble:
        case sString:
            result = Vyraz();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sSemicolon) return ERROR_CODE_SYN;
            result = Dalsi_vyrazy();
            if (result != ERROR_CODE_OK) return result;
        case sEndOfLine:
        case sEnd:
            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Vyraz() {
    //simulace vyrazu jednoho cisla
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    int result = 0;
    return result;
}