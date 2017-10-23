/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   scanner.c - lexikální analyzátor
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#include "error_code.h"
#include "scanner.h"
#include "string.h"

tToken token; // token
tState state;

tToken getNextToken(){
    if (token.atr.value == NULL)
        stringInit(&token.atr); // inicializace retezce tokenu
    else
        stringClear(&token.atr); // smazani obsahu retezce tokenu
    token.type = sStart; // inicializace typu tokenu

    state = sStart; // inicializace automatu na pocatecni stav
    char c; // aktualne cteny znak ze vstupniho souboru

    while (1) {
        c = getchar(); // nacteni dalsiho znaku ze vstupu

        switch (state) {
            case sStart: // STAV: pocatecni stav automatu
                if (charIsSpace(c) /* tabulator a EOL TODO */)
                    state = sStart;
                else if (c == '-') {
                    stringAddChar(&token.atr, c); // zapsani znaku do tokenu
                    state = sMinus;
                }
                else if (c == '+') {
                    stringAddChar(&token.atr, c);
                    state = sPlus;
                }
                else if (c == '*') {
                    stringAddChar(&token.atr, c);
                    state = sMultiply;
                }
                else if (c == '/') {
                    stringAddChar(&token.atr, c);
                    state = sDivideD;
                }
                else if (c == '\\') {
                    stringAddChar(&token.atr, c);
                    state = sDivideI;
                }
                else if (c == '(') {
                    stringAddChar(&token.atr, c);
                    state = sLeftPar;
                }
                else if (c == ')') {
                    stringAddChar(&token.atr, c);
                    state = sRightPar;
                }
                else if (c == ';') {
                    stringAddChar(&token.atr, c);
                    state = sSemicolon;
                }
                else if (c == '=') {
                    stringAddChar(&token.atr, c);
                    state = sAssignment;
                }
                else if (c == '<') {
                    stringAddChar(&token.atr, c);
                    state = sLess;
                }
                else {// cokoliv jineho indikuje chybu
                    state = sLexError;
                }
                break;

            case sMinus: // KONECNE STAVY
            case sPlus:
            case sMultiply:
            case sDivideD:
            case sDivideI:
            case sLeftPar:
            case sRightPar:
            case sSemicolon:
            case sAssignment:
            case sLexError: // lexx error
                charUndo(c); // vrati zpet aktualne cteny znak
                token.type = state; // naplni token typem nalezeneho lexemu
                state = sEnd;
                break;

            case sLess:
                if (c == '=') {
                    stringAddChar(&token.atr, c);
                    state = sLessEqual;
                }
                else {
                    charUndo(c);

                }

                break;

            case sEnd:
                return token;
        }
    }
}