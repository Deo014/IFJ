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

#include "error_code.h"
#include"symtable.h"
#include "instList.h"

typedef enum {
    sEnd = 100,
    sScope,
    sDeclare,
    sFuntion,
    sAs,
    sDim,
    sPrint,
    sInput,
    sIf,
    sThen,
    sElse,
    sDo,
    sWhile,
    sLoop,
    sReturn,
    tInteger,
    tDouble,
    tString
} keyword_type;

int parse(tSymtable *, tDLListInstruction *);

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
#endif //IFJ_PARSER_H
