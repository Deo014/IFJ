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
    sStart,                 //      POCATECNI STAV AUTOMATU
    sIdentificator,         //      KONCOVY STAV
    sIdentificatorOrKeyWord,//      mezistav
    //sKeyWord,             //      KONCOVY STAV

    /* datove type */
    sInteger,               //      KONCOVY STAV
    sDouble,                //      KONCOVY STAV
    sDoublePoint,           //      mezistav
    sDoublePointNumber,     //      mezistav
    sDoubleExponent,        //      mezistav
    sDoubleExponentOperator,//      mezistav
    //sTODO,                   // Q3   0..9, +, -

    /* operatory */
    sPlus,                  //      KONCOVY STAV    +
    sMinus,                 //      KONCOVY STAV    -
    sDivideD,               //      KONCOVY STAV    /
    sDivideDOrBlockComment, //      mezistav
    sMultiply,              //      KONCOVY STAV    *
    sDivideI,               //      KONCOVY STAV    obracene lomitko
    sLess,                  //      KONCOVY STAV    <
    sNotEqual,              //      KONCOVY STAV    <>
    sLessEqual,             //      KONCOVY STAV    <=
    sMore,                  //      KONCOVY STAV    >
    sMoreEqueal,            //      KONCOVY STAV    >=
    sAssignment,            //      KONCOVY STAV    =

    sLeftPar,               //      KONCOVY STAV    (
    sRightPar,              //      KONCOVY STAV    )
    sSemicolon,             //      KONCOVY STAV    ;

    sLineComment,           //      mezistav        '
    sBlockComment,          //      mezistav /' ( ceka na ukonceni komentare: '/ )
    //sEndOfFile,           //      KONEC VSTUPNIHO SOUBORU
    sLexError = -1,         //      LEXIKALNI CHYBA
    //sEnd

} tState;

// struktura popisujici token
typedef struct {
    string atr;
    int type;
} tToken;

// funkce vracejici dalsi token
tToken getNextToken();

#endif //IFJ_SCANNER_H
