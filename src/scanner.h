/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   scanner.h - lexikální analyzátor
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

/*
 * Datkovy typ enum popisujici stavy automatu
 */
typedef enum {
    sStart,                     //  S       POCATECNI STAV AUTOMATU

    /* identifikator / klicove slovo */
            sIdentificator,             //  F15     KONCOVY STAV
    sIdentificatorOrKeyWord,    //  F21     mezistav
    sKeyWord,                   //          KONCOVY STAV
    //sResKeyWord,              //          KONCOVY STAV

    /* datove type */
            sInteger,                   //  F12     KONCOVY STAV

    sDouble,                    //  F13     KONCOVY STAV
    sDoublePoint,               //  Q1      mezistav
    sDoublePointNumber,         //  Q2      mezistav
    sDoubleExponent,            //  Q3      mezistav
    sDoubleExponentOperator,    //  Q8      mezistav

    sString,                    //  F14     KONCOVY STAV
    sStringStart,               //  Q4      mezistav
    sStringRead,                //  Q7      mezistav
    sStringEscape,              //  Q6      mezistav
    sStringEscapeNumber,        //  Q11-12  mezistav

    /* operatory */
            sPlus,                      //  F1      KONCOVY STAV    +
    sMinus,                     //  F2      KONCOVY STAV    -
    sDivideD,                   //  F3      KONCOVY STAV    /
    sDivideDOrBlockComment,     //          mezistav
    sMultiply,                  //  F4      KONCOVY STAV    *
    sDivideI,                   //  F5      KONCOVY STAV    obracene lomitko
    sLess,                      //  F6      KONCOVY STAV    <
    sNotEqual,                  //  F7      KONCOVY STAV    <>
    sLessEqual,                 //  F8      KONCOVY STAV    <=
    sMore,                      //  F9      KONCOVY STAV    >
    sMoreEqual,                 //  F10     KONCOVY STAV    >=
    sAssignment,                //  F11     KONCOVY STAV    =

    sLeftPar,                   //  F16     KONCOVY STAV    (
    sRightPar,                  //  F17     KONCOVY STAV    )
    sSemicolon,                 //  F20     KONCOVY STAV    ;
    sComma,                     //  F22     KONCOVY STAV    ,

    /* komentare */
            sLineComment,               //  Q10     mezistav        '
    sBlockComment,              //  Q9      mezistav /' ( ceka na ukonceni komentare: '/ )

    sEndOfLine,                 //          KONCOVY STAV
    sEndOfFile,                 //          KONCOVY STAV    KONEC VSTUPNIHO SOUBORU

    /* chybovy stav */
            sLexError = -1,             //          KONCOVY STAV
} tState;

/*
 * Struktura popisujici token
 */
typedef struct {
    string atr;
    int type;
} tToken;

/*
 * Funkce vrati nasledujici token ze vstupniho souboru
 */
tToken getNextToken();

#endif //IFJ_SCANNER_H