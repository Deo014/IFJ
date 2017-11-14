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
tDLList *list;
tToken aktualni_token;
string attr;
int tokenType;

//Pomocna funkce, ktera z obsahu atributu tokenu klicovych slov priradi cislo k pouziti ve switchi
int adjustTokenType(tToken tok) {

    if (strcmp(tok.atr.value, "end") == 0)
        return 101;
    if (strcmp(tok.atr.value, "scope") == 1)
        return 102;
    if (strcmp(tok.atr.value, "declare") == 0)
        return 103;
    if (strcmp(tok.atr.value, "function") == 0)
        return 104;
    if (strcmp(tok.atr.value, "end") == 0)
        return 105;
    if (strcmp(tok.atr.value, "as") == 0)
        return 106;
    if (strcmp(tok.atr.value, "dim") == 0)
        return 107;
    if (strcmp(tok.atr.value, "print") == 0)
        return 108;
    if (strcmp(tok.atr.value, "input") == 0)
        return 109;
    if (strcmp(tok.atr.value, "if") == 0)
        return 110;
    if (strcmp(tok.atr.value, "then") == 0)
        return 111;
    if (strcmp(tok.atr.value, "else") == 0)
        return 112;
    if (strcmp(tok.atr.value, "do") == 0)
        return 113;
    if (strcmp(tok.atr.value, "while") == 0)
        return 114;
    if (strcmp(tok.atr.value, "loop") == 0)
        return 115;
    if (strcmp(tok.atr.value, "return") == 0)
        return 116;
    return -1;
}


int parse(tSymtable *symtable, tDLList *instrList) {
    int result;
    table = symtable;
    list = instrList;
    aktualni_token = getNextToken();
    //pokud hned prvni token je chybny
    if (aktualni_token.type == sLexError)
        result = LEX_ERROR;
    else
        result = Program();
    return result;
}

int Program() {
//1<Program> -> <Nekolik_deklaraci_fce><Nekolik_definici_fce><Telo_programu>
    //Pokud je token klicove slovo, zmenime ho na konkretni pomoci fce
    if (aktualni_token.type == 3) {
        aktualni_token.type = adjustTokenType(aktualni_token);
    }
    int result;
    switch (aktualni_token.type) {
        case ID:
        case LEFT_VINCULUM:
            // pravidlo <program> -> <declrList> <statList> <END_OF_FILE>
            result = Nekolik_deklaraci_fce();
            if (result != SYNTAX_OK) return result;
            result = statList();
            if (result != SYNTAX_OK) return result;

            // POZOR! Nezapomenout testovat, zda nasleduje konec souboru.
            // Pri oponenuti teto veci by zde mohly pokracovat nejake nesmysly, ktere by se
            // v ramci syntakticke analyzy jiz nezpracovavaly a program by se tvaril, ze je OK
            if (token != END_OF_FILE) return SYNTAX_ERROR;

            // nagenerujeme instrukci konce programu
            generateInstruction(I_STOP, NULL, NULL, NULL);

            return SYNTAX_OK;
            break;
    }
}

int Nekolik_deklaraci_fce() {
    int result;
    if (strcmp(aktualni_token.atr.value, "declare") == 0) {
        result = Deklarace_fce();
        if (result != SYNTAX_OK) return result;
        result = Nekolik_definici_fce();
        if (result != SYNTAX_OK) return result;
        return Nekolik_deklaraci_fce();
    }


    return SYNTAX_ERROR;
}

int Nekolik_definici_fce() {
    int result;


    return result;
}

int Telo_programu() {
    int result;


    return result;
}

int Deklarace_fce() {
    int result;


    return result;
}