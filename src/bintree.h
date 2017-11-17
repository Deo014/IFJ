/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   bintree.h - Hlavičkový soubor binárního stromu
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek (projekt IAL2 c401)
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */
#include "stdbool.h"

#ifndef IFJ_BINTREE_H
#define IFJ_BINTREE_H


typedef enum {
    ndtVariable,     // nodeDataTypeVariable
    ndtFunction,     // nodeDataTypeFunction
} tNodeDataType;

/* ----------uzel stromu---------- */
typedef struct tBSTNode {
    char* Key;			                             /* klic */
    tNodeDataType nodeDataType;                      /* typ dat, ktera uzel uchovava */
    void* Data;                                      /* uzitecny obsah uzlu */
    struct tBSTNode * LPtr;                          /* ukazatel na levy podstrom */
    struct tBSTNode * RPtr;                          /* ukazatel na pravy podstrom */
} *tBSTNodePtr;

/* ----------prototypy funkcí---------- */
void BSTInit   (tBSTNodePtr *);
tBSTNodePtr BSTSearch (tBSTNodePtr, char*);
void BSTInsert (tBSTNodePtr *, char*, void*, tNodeDataType);
void BSTDelete (tBSTNodePtr *, char*);
void BSTDispose(tBSTNodePtr *);

/* ----------funkce pro ladeni---------- */
void Print_tree2(tBSTNodePtr TempTree, char* sufix, char fromdir); /* pomocna funkce pro vykresleni sktruktury binarniho stromu */
void Print_tree(tBSTNodePtr TempTree); /* volana funkce pro vykresleni funkce binarniho stromu */
#endif //IFJ_BINTREE_H
