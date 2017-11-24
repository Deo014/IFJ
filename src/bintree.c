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
#include "string.h"
#include <string.h>

/* ----------funkce binarniho vyhledavaciho stromu----------*/
void BSTInit (tBSTNodePtr *RootPtr) {

    (*RootPtr) = NULL;

}

tBSTNodePtr BSTSearch (tBSTNodePtr RootPtr, char* K)	{

    if (RootPtr == NULL) {
        return NULL;
    }
    else {
        if ( strcmp(K, RootPtr->Key) < 0) {
            return BSTSearch(RootPtr->LPtr, K);
        } else if (strcmp(K, RootPtr->Key) > 0) {
            return BSTSearch(RootPtr->RPtr, K);
        }
        else {
            return RootPtr;
        }
    }

}

void BSTInsert (tBSTNodePtr* RootPtr, char* K, void* Data, tNodeDataType nodeDataType)	{

    if ( RootPtr != NULL && (*RootPtr) != NULL) {
        if ( strcmp(K, (*RootPtr)->Key) != 0 ) { // vyhledavani pokracuje v levem nebo pravem podstrumu
            if ( strcmp(K, ((*RootPtr)->Key)) < 0 ) {
                BSTInsert( &((*RootPtr)->LPtr), K, Data, nodeDataType);
            } else if ( strcmp(K, (*RootPtr)->Key) > 0 ) {
                BSTInsert( &((*RootPtr)->RPtr), K, Data, nodeDataType);
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
        newitem->nodeDataType = nodeDataType;
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
        tBSTNodePtr itemToDelete = *RootPtr;
        *RootPtr = (*RootPtr)->LPtr;
        free(itemToDelete);
    }
    else {
        ReplaceByRightmost(PtrReplaced, &((*RootPtr)->RPtr));
    }

}

void BSTDelete (tBSTNodePtr *RootPtr, char* K) 		{

    if ( RootPtr && (*RootPtr) ) {
        if ( strcmp(K, (*RootPtr)->Key) < 0 ) {
            BSTDelete( &((*RootPtr)->LPtr), K);
        }
        else if ( strcmp(K, (*RootPtr)->Key) > 0 ) {
            BSTDelete( &((*RootPtr)->RPtr), K);
        }
        else { // pokud byl nalezen uzel s danym klicem
            if ( ((*RootPtr)->LPtr == NULL) && ((*RootPtr)->RPtr == NULL) ) { // pokud se jedna o listovy uzel
                free((*RootPtr)->Data); // uvolneni pameti dat
                free(*RootPtr);
                *RootPtr = NULL;
            }
            else if ( ((*RootPtr)->LPtr != NULL) && ((*RootPtr)->RPtr == NULL) ) { // pokud ma uzel jen levy podstrom
                free((*RootPtr)->Data); // uvolneni pameti dat
                free(*RootPtr);
                *RootPtr = (*RootPtr)->LPtr;
            }
            else if ( ((*RootPtr)->RPtr != NULL) && ((*RootPtr)->LPtr == NULL) ) { // pokud ma uzel jen pravy podstrom
                free((*RootPtr)->Data); // uvolneni pameti dat
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

    if ( (*RootPtr) != NULL ) {
        BSTDispose(&(*RootPtr)->LPtr); // zruseni leveho podstromu
        BSTDispose(&(*RootPtr)->RPtr); // zruseni praveho podstromu
        // uvolneni aktualniho prvku
        free(*RootPtr);
        *RootPtr = NULL;
    }

}

/* --------------------funkce pro ladeni (z projektu IAL)-------------------- */
void Print_tree2(tBSTNodePtr TempTree, char* sufix, char fromdir) { /* vykresli sktrukturu binarniho stromu */
    if (TempTree != NULL)
    {
        char* suf2 = (char*) malloc(strlen(sufix) + 4);
        strcpy(suf2, sufix);
        if (fromdir == 'L')
        {
            suf2 = strcat(suf2, "  |");
            printf("%s\n", suf2);
        }
        else
            suf2 = strcat(suf2, "   ");
        Print_tree2(TempTree->RPtr, suf2, 'R');
        if (TempTree->nodeDataType == ndtFunction)
            printf("%s  +-[FCE => %s, RV:%d, DC:%d DF:%d, ARG:%s]\n", sufix, TempTree->Key, ((tDataFunction*)TempTree->Data)->returnDataType, ((tDataFunction*)TempTree->Data)->declared, ((tDataFunction*)TempTree->Data)->defined, ((tDataFunction*)TempTree->Data)->parameters.value);
        else
            printf("%s  +-[VAR => %s, DT:%d]\n", sufix, TempTree->Key, ((tDataVariable*)TempTree->Data)->dataType);
        strcpy(suf2, sufix);
        if (fromdir == 'R')
            suf2 = strcat(suf2, "  |");
        else
            suf2 = strcat(suf2, "   ");
        Print_tree2(TempTree->LPtr, suf2, 'L');
        if (fromdir == 'R') printf("%s\n", suf2);
        free(suf2);
    }
}

void Print_tree(tBSTNodePtr TempTree) {
    printf("Struktura binarniho stromu:\n");
    printf("\n");
    if (TempTree != NULL)
        Print_tree2(TempTree, "", 'X');
    else
        printf("strom je prazdny\n");
    printf("\n");
    printf("=================================================\n");
}

