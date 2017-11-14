/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   parser.h - syntaktický a sémantický analyzátor
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */


#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#define SYNTAX_OK 0
#define LEX_ERROR     1
#define SYNTAX_ERROR 2

typedef enum {
    sEnd = 100,
    sScope, sDeclare, sFuntion, sAs, sDim, sPrint, sInput, sIf, sThen, sElse, sDo, sWhile, sLoop, sReturn
} keyword_type;


#endif //IFJ_PARSER_H
