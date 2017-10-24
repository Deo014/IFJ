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
            // STAV: pocatecni stav automatu
            case sStart:
                if ( charIsWhiteChar(c) )
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
                    state = sDivideDOrBlockComment;
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
                else if (c =='>') {
                    stringAddChar(&token.atr, c);
                    state = sMore;
                }
                else if (c == '_') {
                    stringAddChar(&token.atr, c);
                    state = sIdentificator;
                }
                else if( charIsLetter(c) ) {
                    stringAddChar(&token.atr, c);
                    state = sIdentificatorOrKeyWord;
                }
                else if (c == '\'') {
                    state = sLineComment;
                }
                else if ( charIsDigit(c) ) {
                    stringAddChar(&token.atr, c);
                    state = sInteger;
                }
                else {// jakykoliv jiny znak je nepovoleny
                    stringAddChar(&token.atr, c);
                    state = sLexError;
                }
                break;
            // STAV: <
            case sLess:
                if (c == '=') { // vrat token <=
                    stringAddChar(&token.atr, c);
                    state = sLessEqual;
                }
                else if (c == '>') { // vrat token <>
                    stringAddChar(&token.atr, c);
                    state = sNotEqual;
                }
                else { // vrat token <
                    charUndo(c);
                    token.type = sLess;
                    return token;
                }
                break;

            // STAV: >
            case sMore:
                if (c == '=') { // vrat token >=
                    stringAddChar(&token.atr, c);
                    state = sMoreEqueal;
                }
                else { // vrat token >
                    charUndo(c);
                    token.type = sMore;
                    return token;
                }
                break;

            // STAV: identifikator/klicove slovo
            case sIdentificatorOrKeyWord:
                if (c == '_') {
                    stringAddChar(&token.atr, c);
                    state = sIdentificator;
                }
                else if ( charIsLetter(c) || charIsDigit(c) ) {
                    stringAddChar(&token.atr, c);
                    state = sIdentificatorOrKeyWord;
                }
                else { // vrat token identifikator/klicove slovo
                    charUndo(c);
                    /* TODO zjistit jestli je to ID nebo KEYWORD */
                    token.type = sIdentificatorOrKeyWord;
                    return token;
                }
                break;

            // STAV: identifikator
            case sIdentificator:
                if ( c == '_' || charIsLetter(c) || charIsDigit(c) ) {
                    stringAddChar(&token.atr, c);
                    state = sIdentificator;
                }
                else { // vrat token identifikator
                    charUndo(c);
                    token.type = sIdentificator;
                    return token;
                }
                break;

            // STAV: ' radkovy komentar
            case sLineComment:
                if (c == '\n' || c == EOF) {
                    state = sStart;
                }
                // pokud precte jakykoliv jiny znak nez '\n' nebo EOF nic se nestane
                break;

            // STAV: /' blokovy komentar
            case sBlockComment:
                if ( ((stringGetLastChar(&token.atr) == '\'') && (c == '/')) || (c == EOF) ) { // ukonceni blokoveho komentare
                    stringClear(&token.atr);
                    state = sStart;
                    break;
                }
                stringAddChar(&token.atr, c);
                break;

            // STAV: / deleno nebo blokovy komentar
            case sDivideDOrBlockComment:
                if (c == '\'') {
                    stringClear(&token.atr); // smaze znak / z tokenu
                    state = sBlockComment;
                }
                else {
                    charUndo(c);
                    state = sDivideD;
                }
                break;

            // STAV: 0..9 integer
            case sInteger:
                if ( charIsDigit(c) ) { // cteni dalsich cislic
                    stringAddChar(&token.atr, c);
                    state = sInteger;
                }
                else if (c == '.') {
                    stringAddChar(&token.atr, c);
                    state = sDouble;
                }
                else if (c == 'e' || c == 'E') {
                    stringAddChar(&token.atr, c);
                    state = sDouble;
                }
                else { // vrat token integer
                    charUndo(c);
                    token.type = sInteger;
                    return token;
                }
                break;

            // STAV: . nebo eE double
            case sDouble:
                if ( charIsDigit(c) ) { // cteni dalsich cislic
                    stringAddChar(&token.atr, c);
                    state = sDouble;
                }
                else if ( (c == 'e' || c == 'E') && (!stringContainsChar(&token.atr, 'e') || !stringContainsChar(&token.atr, 'E')) && (stringGetLastChar(&token.atr) != '.') ) {
                    // pokud je cteny znak exponent a token neobsahuje exponent a zaroven posledni znak neni tecka: zapise exponent
                    stringAddChar(&token.atr, c);
                    state = sDouble;
                }
                else { // vrat token double nebo lexx error
                    if ( charIsDigit(stringGetLastChar(&token.atr)) ) { // pokud token konci cislem: vrat token double
                        charUndo(c);
                        token.type = sDouble;
                        return token;
                    }
                    else { // jinak lex error
                        charUndo(c);
                        state = sLexError;
                    }
                }
                break;

            // OSTATNI KONCOVE STAVY
            case sMinus:
            case sPlus:
            case sMultiply:
            case sDivideD:
            case sDivideI:
            case sLeftPar:
            case sRightPar:
            case sSemicolon:
            case sAssignment:
            case sLessEqual:
            case sMoreEqueal:
            case sNotEqual:
                charUndo(c); // vrati zpet aktualne cteny znak
                token.type = state; // naplni token typem nalezeneho lexemu
                return token;

            // STAV: Lexikalni chyba
            case sLexError:
                //charUndo(c); // vrati zpet aktualne cteny znak
                token.type = state; // naplni token typem nalezeneho lexemu
                return token;
        }
    }
}