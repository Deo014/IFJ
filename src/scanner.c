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


tToken getNextToken(){
    tToken token;
    tState state;

    stringInit(&token.atr); // inicializace attributu tokenu
    token.type = sStart; // inicializace typu tokenu: pocatecni stav
    state = sStart; // inicializace pocatecniho stavu automatu

    char c; // aktualne cteny znak ze vstupniho souboru (stdin)
    int escapeValue, escapeCounter; // pomocne promenne pro escape sekvenci u stringu

    while (1) {
        c = getchar(); // nacteni dalsiho znaku ze vstupu

        switch (state) {
            /* ----------------------------------------START POCATECNI STAV AUTOMATU----------------------------------*/
            case sStart:
                if ( charIsSpace(c) || charIsTab(c) )
                    state = sStart;
                else if ( c == EOF ) { // konec souboru
                    stringAddChar(&token.atr, 'E');
                    stringAddChar(&token.atr, 'O');
                    stringAddChar(&token.atr, 'F');
                    token.type = sEndOfFile;
                    return token;
                }
                else if ( c == '\n' ) { // novy radek
                    stringAddChar(&token.atr, 'E');
                    stringAddChar(&token.atr, 'O');
                    stringAddChar(&token.atr, 'L');
                    token.type = sEndOfLine;
                    return token;
                }
                else if (c == '-' ) {
                    stringAddChar(&token.atr, c); // zapsani znaku do tokenu
                    state = sMinus;
                }
                else if (c == '+' ) {
                    stringAddChar(&token.atr, c);
                    state = sPlus;
                }
                else if (c == '*' ) {
                    stringAddChar(&token.atr, c);
                    state = sMultiply;
                }
                else if (c == '/' ) {
                    stringAddChar(&token.atr, c);
                    state = sDivideDOrBlockComment;
                }
                else if (c == '\\' ) {
                    stringAddChar(&token.atr, c);
                    state = sDivideI;
                }
                else if (c == '(' ) {
                    stringAddChar(&token.atr, c);
                    state = sLeftPar;
                }
                else if (c == ')' ) {
                    stringAddChar(&token.atr, c);
                    state = sRightPar;
                }
                else if (c == ';' ) {
                    stringAddChar(&token.atr, c);
                    state = sSemicolon;
                }
                else if (c == '=' ) {
                    stringAddChar(&token.atr, c);
                    state = sAssignment;
                }
                else if (c == '<' ) {
                    stringAddChar(&token.atr, c);
                    state = sLess;
                }
                else if (c =='>' ) {
                    stringAddChar(&token.atr, c);
                    state = sMore;
                }
                else if (c == '_' ) {
                    stringAddChar(&token.atr, c);
                    state = sIdentificator;
                }
                else if (c == ',') {
                    stringAddChar(&token.atr, c);
                    state = sComma;
                }
                else if( charIsLetter(c) ) {
                    stringAddChar(&token.atr, c);
                    state = sIdentificatorOrKeyWord;
                }
                else if (c == '\'' ) {
                    state = sLineComment;
                }
                else if ( charIsDigit(c) ) {
                    stringAddChar(&token.atr, c);
                    state = sInteger;
                }
                else if ( c == '!' ) { // pocatek retezce
                    state = sStringStart;
                }
                else {// nacteni nepovoleneho znaku: lex error
                    stringAddChar(&token.atr, c);
                    token.type = sLexError;
                    return token;
                }
                break;
            /* ----------------------------------------END POCATECNI STAV AUTOMATU------------------------------------*/


            /* ----------------------------------------START LESS / MORE----------------------------------------------*/
            case sLess: // <
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

            case sMore: // >
                if (c == '=') { // vrat token >=
                    stringAddChar(&token.atr, c);
                    state = sMoreEqual;
                }
                else { // vrat token >
                    charUndo(c);
                    token.type = sMore;
                    return token;
                }
                break;
            /* ----------------------------------------END LESS / MORE------------------------------------------------*/


            /* ----------------------------------------START IDENTIFIKATOR / KLICOVE SLOVO----------------------------*/
            case sIdentificatorOrKeyWord:
                if (c == '_' || charIsDigit(c) ) {
                    stringAddChar(&token.atr, c);
                    state = sIdentificator;
                }
                else if ( charIsLetter(c) ) {
                    stringAddChar(&token.atr, c);
                    state = sIdentificatorOrKeyWord;
                }
                else { // vrat token identifikator / klicove slovo
                    charUndo(c);
                    // zjisteni, jeslti je token keyword nebo identifikator
                    stringToLowercase(&token.atr); // prevod atributu tokenu na lowercase
                    if ( stringIsKeyWord(&token.atr) || stringIsResKeyWord(&token.atr) ) {
                        token.type = sKeyWord;
                    }
                    else {
                        token.type = sIdentificator;
                    }
                    return token;
                }
                break;

            case sIdentificator:
                if ( c == '_' || charIsLetter(c) || charIsDigit(c) ) {
                    stringAddChar(&token.atr, c);
                    state = sIdentificator;
                }
                else { // vrat token identifikator
                    charUndo(c);
                    stringToLowercase(&token.atr); // prevod atributu tokenu na lowercase
                    token.type = sIdentificator;
                    return token;
                }
                break;

            case sKeyWord:
                break;
            /* ----------------------------------------END IDENTIFIKATOR / KLICOVE SLOVO------------------------------*/


            /* ----------------------------------------START KOMENTARE----------------------------------------------- */
            case sLineComment: // '
                if (c == '\n' || c == EOF) {
                    charUndo(c);
                    state = sStart;
                }
                // pokud precte jakykoliv jiny znak nez '\n' nebo EOF nic se nestane
                break;

            case sBlockComment: // /'
                if ( ((stringGetLastChar(&token.atr) == '\'') && (c == '/')) || (c == EOF) ) { // ukonceni blokoveho komentare
                    stringClear(&token.atr);
                    state = sStart;
                    break;
                }
                stringAddChar(&token.atr, c);
                break;
            /* ----------------------------------------END KOMENTARE------------------------------------------------- */


            /* ----------------------------------------START INTEGER------------------------------------------------- */
            case sInteger:
                if ( charIsDigit(c) ) { // cteni dalsich cislic
                    stringAddChar(&token.atr, c);
                    state = sInteger;
                }
                else if (c == '.') {
                    stringAddChar(&token.atr, c);
                    state = sDoublePoint;
                }
                else if (c == 'e' || c == 'E') {
                    stringAddChar(&token.atr, c);
                    state = sDoubleExponent;
                }
                else { // vrat token integer
                    charUndo(c);
                    token.type = sInteger;
                    return token;
                }
                break;
            /* ----------------------------------------END INTEGER--------------------------------------------------- */


            /* ----------------------------------------START DOUBLE-------------------------------------------------- */
            case sDoublePoint:
                if ( charIsDigit(c) ) { // cteni dalsich cislic
                    stringAddChar(&token.atr, c);
                    state = sDoublePointNumber;
                }
                else { // nacteni nepovoleneho znaku: lex error
                    charUndo(c);
                    token.type = sLexError;
                    return token;
                }
                break;

            case sDoublePointNumber:
                if ( charIsDigit(c) ) { // nacteni dalsiho cisla
                    stringAddChar(&token.atr, c);
                    state = sDoublePointNumber;
                }
                else if ( c == 'e' || c == 'E' ) {
                    stringAddChar(&token.atr, c);
                    state = sDoubleExponent;
                }
                else { // nepovoleny znak: vrat token double
                    charUndo(c);
                    token.type = sDouble;
                    return token;
                }
                break;

            case sDoubleExponent:
                if ( charIsDigit(c) ) { // nacteni dalsiho cisla: presun do stavu sDouble
                    stringAddChar(&token.atr, c);
                    state = sDouble;
                }
                else if ( c == '+' || c == '-') {
                    stringAddChar(&token.atr, c);
                    state = sDoubleExponentOperator;
                }
                else { // nepovoleny znak: lex error
                    charUndo(c);
                    token.type = sLexError;
                    return token;
                }
                break;

            case sDoubleExponentOperator:
                if ( charIsDigit(c) ) { // nacteni dalsiho cisla
                    stringAddChar(&token.atr, c);
                    state = sDouble;
                }
                else { // nepovoleny znak: lex error
                    charUndo(c);
                    token.type = sLexError;
                    return token;
                }
                break;

            case sDouble:
                if ( charIsDigit(c) ) { // nacteni dalsiho cisla
                    stringAddChar(&token.atr, c);
                    state = sDouble;
                }
                else { // pri nacteni nepovoleneho znaku: vrat token double
                    charUndo(c);
                    token.type = sDouble;
                    return token;
                }
                break;
            /* ----------------------------------------END DOUBLE---------------------------------------------------- */


            /* ----------------------------------------START STRING-------------------------------------------------- */
            case sStringStart: // precteno: !
                if ( c == '"' ) { // znak "
                    token.type = sString; // predpokladany typ tokenu je sString
                    state = sString;
                }
                else { // nepovoleneny znak: lex error
                    charUndo(c);
                    stringAddChar(&token.atr, '!'); // pro lepsi porozumeni chybe: prida do tokenu znak !
                    stringAddChar(&token.atr, c);
                    token.type = sLexError;
                    return token;
                }
                break;

            case sString: // precteno !"
                if ( c == '"' ) { // znak " ukonceni stringu: vrat token string
                    return token;
                }
                else if ( c == '\\') { // zapis znaku pomoci escape sekvence
                    state = sStringEscape;
                }
                else if ( c == '#') { // zapis znaku pomoci escape sekvence
                    stringAddChar(&token.atr, '\\');
                    stringAddChar(&token.atr, '0');
                    stringAddChar(&token.atr, '3');
                    stringAddChar(&token.atr, '5'); // string: \035
                }
                else if ( c == ' ') { // zapis znaku pomoci escape sekvence
                    stringAddChar(&token.atr, '\\');
                    stringAddChar(&token.atr, '0');
                    stringAddChar(&token.atr, '3');
                    stringAddChar(&token.atr, '2');
                }
                else if ( c > 31 ) { // primy zapis znaku
                    stringAddChar(&token.atr, c);
                    state = sString;
                }
                else { // nepovoleny znak: lex error
                    charUndo(c);
                    stringAddFirstChar(&token.atr, '"'); // zapis !" do tokenu, aby bylo jasne, ze k chybe doslo ve stringu
                    stringAddFirstChar(&token.atr, '!'); // zapis !" do tokenu, aby bylo jasne, ze k chybe doslo ve stringu
                    token.type = sLexError;
                    return token;
                }
                break;

            case sStringEscape:
                if ( c == '\\') {
                    stringAddChar(&token.atr, c);
                    stringAddChar(&token.atr, '0');
                    stringAddChar(&token.atr, '9');
                    stringAddChar(&token.atr, '2'); // string: \092
                    state = sString;
                }
                else if ( c == 'n' ) {
                    stringAddChar(&token.atr, '\\');
                    stringAddChar(&token.atr, '0');
                    stringAddChar(&token.atr, '1');
                    stringAddChar(&token.atr, '0'); // string: \010
                    state = sString;
                }
                else if ( c == 't' ) {
                    stringAddChar(&token.atr, '\\');
                    stringAddChar(&token.atr, '0');
                    stringAddChar(&token.atr, '0');
                    stringAddChar(&token.atr, '9'); // string \009
                    state = sString;
                }
                else if ( c == '"' ) {
                    stringAddChar(&token.atr, '"');
                    state = sString;
                }
                else if ( charIsDigit(c) ) {
                    escapeCounter = 1;
                    escapeValue = charToDec(c) * 100; // prevod znaku na ciselnou hodnotu a pricteni do escapeValue, prvni cislice je v radu stovek
                    state = sStringEscapeNumber;
                }
                else { // nepovoleny znak: lexx error
                    token.type = sLexError;
                    state = sString;
                }
                break;

            case sStringEscapeNumber:
                if ( charIsDigit(c) && (escapeCounter == 1) ) { // prijeti druheho cisla
                    escapeCounter = 2;
                    escapeValue += charToDec(c) * 10; // prevod znaku na ciselnou hodnotu a pricteni do escapeValue, druha cislice je v radu desitek
                    state = sStringEscapeNumber;
                }
                else if ( charIsDigit(c) && (escapeCounter == 2) ) {
                    escapeValue += charToDec(c); // prevod znaku na ciselnou hodnotu a pricteni do escapeValue, treti cislice je v radu jednotek

                    if ( !((escapeValue >= 0) && (escapeValue <= 255)) ) { // zjisteni, jestli je escape sekvence platna
                        token.type = sLexError;
                    }
                    // zapsani escape sekvence do stringu
                    stringAddChar(&token.atr, '\\');
                    stringAddChar(&token.atr, decToChar(escapeValue/100) );
                    escapeValue -= (escapeValue/100) * 100; // odstraneni stovek
                    stringAddChar(&token.atr, decToChar(escapeValue/10) );
                    escapeValue -= (escapeValue/10) * 10; // odstraneni desitek
                    stringAddChar(&token.atr, decToChar(escapeValue) );

                    state = sString;
                }
                else { // nepovoleny znak: lexx error
                    token.type = sLexError;
                    state = sString;
                }
                break;

            /* ----------------------------------------END STRING---------------------------------------------------- */


            /* ----------------------------------------START OSTATNI KONCOVE STAVY------------------------------------*/
            case sMinus:
            case sPlus:
            case sMultiply:
            case sDivideD:
            case sDivideI:
            case sLeftPar:
            case sRightPar:
            case sSemicolon:
            case sComma:
            case sAssignment:
            case sLessEqual:
            case sMoreEqual:
            case sNotEqual:
                charUndo(c); // vrati zpet aktualne cteny znak
                token.type = state; // naplni token typem nalezeneho lexemu
                return token;
                break;
            /* ----------------------------------------END OSTATNI KONCOVE STAVY--------------------------------------*/


            /* ----------------------------------------START OSTATNI POMOCNE STAVY------------------------------------*/
            case sDivideDOrBlockComment: // deleno / nebo blokovy komentar /'
                if (c == '\'') { // start blokoveho komentare
                    stringClear(&token.atr); // smaze znak / z tokenu
                    state = sBlockComment;
                }
                else {
                    charUndo(c);
                    state = sDivideD;
                }
                break;
            /* ----------------------------------------END OSTATNI POMOCNE STAVY--------------------------------------*/


            case sEndOfLine: // pouze pro preklad
            case sEndOfFile:
            case sLexError:
                break;
        }
    }
}