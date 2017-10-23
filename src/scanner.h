/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   scanner.h - syntaktický a sémantický analyzátor
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */


#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H

#include "string.h"

// datkovy typ enum popisujici stavy automatu
typedef enum {
    sStart,             // s    pocatecni stav automatu
    //sIdentificator,     // F15  identifikator/klicove slovo             KONCOVY STAV
    //sKeyWord,           //      klicove slovo                           KONCOVY STAV

    /* datove type */
    //sInteger,           // F12  0..9    1+krat                          KONCOVY STAV

    //sDouble,            // F13  double                                  KONCOVY STAV
    //sDoublePoint,       // Q1   .    (desetinna tecka)
    //sTODO,                   // Q3   0..9, +, -

    /* operatory */
    sPlus,              // F1   +
    sMinus,             // F2   -
    sDivideD,           // F3   /
    sMultiply,          // F4   *
    sDivideI,           // F5
    sLess,              // F6   <
    //sLessMore,          // F7   <>
    sLessEqual,        // F8   <=
    //sMore,              // F9   >
    //sMoreEqueal,        // F10  >=
    sAssignment,        // F11  =

    sLeftPar,           // F16  (
    sRightPar,          // F17  )
    sSemicolon,         // F20  ;

    //sLineComment,       // F18  `   zacatek jednoradkoveho komentare
    //sBlockComment,      // F19  /`  zacatek blokoveho komentare ( ceka na ukonceni `/ )
    //sEndOfFile,         // END
    sLexError,
    sEnd

} tState;

// struktura popisujici token
typedef struct {
    string atr;
    int type;
} tToken;

// funkce vracejici dalsi token
tToken getNextToken();

#endif //IFJ_SCANNER_H
