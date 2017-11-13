/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   bintree.c - Implementace binárního stromu
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek (projekt IAL2 c401)
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#include "bintree.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "symtable.h"

void BSTInit (tBSTNodePtr *RootPtr) {

    (*RootPtr) = NULL;

}

tBSTNodePtr BSTSearch (tBSTNodePtr RootPtr, char K)	{

    if (RootPtr == NULL) {
        return NULL;
    }
    else {
        if (K < RootPtr->Key) {
            return BSTSearch(RootPtr->LPtr, K);
        } else if (K > RootPtr->Key) {
            return BSTSearch(RootPtr->RPtr, K);
        }
        else {
            return RootPtr;
        }
    }

}


void BSTInsert (tBSTNodePtr* RootPtr, char K, void *Data)	{

    if ( RootPtr != NULL && (*RootPtr) != NULL) {
        if ( K != (*RootPtr)->Key ) { // vyhledavani pokracuje v levem nebo pravem podstrumu
            if ( K < ((*RootPtr)->Key) ) {
                BSTInsert( &((*RootPtr)->LPtr), K, Data);
            } else if ( K > (*RootPtr)->Key ) {
                BSTInsert( &((*RootPtr)->RPtr), K, Data);
            }
        }
        else { // aktualizace dat pri nalezeni stejneho klice
            (*RootPtr)->Data = Data;
        }
    }
    else {
        // alokace pameti pro novy uzel
        struct tBSTNode *newitem;
        if ( (newitem = (struct tBSTNode*)malloc(sizeof(struct tBSTNode))) == NULL ) {
            return;
        }
        // inicializace dat noveho uzlu
        newitem->Key = K;
        newitem->Data = Data;
        newitem->LPtr = newitem->RPtr = NULL;

        (*RootPtr) = newitem;
    }

}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {

    if ( (*RootPtr)->RPtr == NULL ) {
        // prekopirovani hodnot uzlu
        PtrReplaced->Data = (*RootPtr)->Data;
        PtrReplaced->Key = (*RootPtr)->Key;
        // uvolneni uzlu
        BSTDelete(RootPtr, (*RootPtr)->Key);
    }
    else {
        ReplaceByRightmost(PtrReplaced, &((*RootPtr)->RPtr));
    }

}

void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
/*   ---------
** Zruší uzel stromu, který obsahuje klíč K.
**
** Pokud uzel se zadaným klíčem neexistuje, nedělá funkce nic.
** Pokud má rušený uzel jen jeden podstrom, pak jej zdědí otec rušeného uzlu.
** Pokud má rušený uzel oba podstromy, pak je rušený uzel nahrazen nejpravějším
** uzlem levého podstromu. Pozor! Nejpravější uzel nemusí být listem.
**
** Tuto funkci implementujte rekurzivně s využitím dříve deklarované
** pomocné funkce ReplaceByRightmost.
**/

    if ( RootPtr && (*RootPtr) ) {
        if ( K < (*RootPtr)->Key ) {
            BSTDelete( &((*RootPtr)->LPtr), K);
        }
        else if ( K > (*RootPtr)->Key ) {
            BSTDelete( &((*RootPtr)->RPtr), K);
        }
        else { // pokud byl nalezen uzel s danym klicem
            if ( ((*RootPtr)->LPtr == NULL) && ((*RootPtr)->RPtr == NULL) ) { // pokud se jedna o listovy uzel
                free(*RootPtr);
                *RootPtr = NULL;
            }
            else if ( ((*RootPtr)->LPtr != NULL) && ((*RootPtr)->RPtr == NULL) ) { // pokud ma uzel jen levy podstrom
                free(*RootPtr);
                *RootPtr = (*RootPtr)->LPtr;
            }
            else if ( ((*RootPtr)->RPtr != NULL) && ((*RootPtr)->LPtr == NULL) ) { // pokud ma uzel jen pravy podstrom
                free(*RootPtr);
                *RootPtr = (*RootPtr)->RPtr;
            }
            else { // pokud ma ruseny uzel oba podstromy
                ReplaceByRightmost((*RootPtr), &((*RootPtr)->LPtr));
            }
        }
    }

}

void BSTDispose (tBSTNodePtr *RootPtr) {
/*   ----------
** Zruší celý binární vyhledávací strom a korektně uvolní paměť.
**
** Po zrušení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivně bez deklarování pomocné
** funkce.
**/

    if ( (*RootPtr) != NULL ) {
        BSTDelete(RootPtr, (*RootPtr)->Key);
        BSTDispose(RootPtr);
    }

}

