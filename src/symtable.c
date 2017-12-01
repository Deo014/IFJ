/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   symbtable.c - Implementace tabulky symbolů
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#include "symtable.h"
#include "scanner.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* ----------------------------------------BINARNIH VYHLEDAVACI STROM-------------------------------------------------*/

/*
 * Inicializace
 */
void BSTInit (tBSTNodePtr *RootPtr) {

    (*RootPtr) = NULL;

}

/*
 * Vyhledavani ve stromu
 */
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

/*
 * Vlozeni uzlu do stromu
 */
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

/*
 * Pomocna funkce
 */
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

/*
 * Odstraneni uzlu ze stromu
 */
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

/*
 * Odstraneni celeho stromu
 */
void BSTDispose (tBSTNodePtr *RootPtr) {

    if ( (*RootPtr) != NULL ) {
        BSTDispose(&(*RootPtr)->LPtr); // zruseni leveho podstromu
        BSTDispose(&(*RootPtr)->RPtr); // zruseni praveho podstromu
        // uvolneni aktualniho prvku
        free((*RootPtr)->Key); // uvolneni klice
        (*RootPtr)->Key = NULL;

        if ( (*RootPtr)->nodeDataType == ndtFunction ) {
            stringDispose(&(((tDataFunction*)(*RootPtr)->Data)->parameters));
        }

        free((*RootPtr)->Data); // uvolneni dat
        (*RootPtr)->Data = NULL;

        free(*RootPtr); // uvolneni celeho uzlu
        *RootPtr = NULL;
    }

}

/* ----------------------------------------FUNKCE PRO PRACI SE SYMTABLE-----------------------------------------------*/

/*
 * Inicialiace symtable
 */
void symTableInit(tSymtable* Table) {
    BSTInit(&(Table->root));
}


/*
 * Vlozeni dat o funkci do symtable
 */
void symTableInsertFunction(tSymtable* Table, string Key) { // vlozi do symtable zaznam s klicem Key a data dataPtr
    // alokovani pameti pro data
    tDataFunction * dataPtr;
    if ( (dataPtr = (tDataFunction*)malloc(sizeof(tDataFunction))) == NULL ) {
        return;
    }
    // inicializace dat
    string parameters;
    stringInit(&parameters);
    dataPtr->returnDataType = -1;
    dataPtr->declared = false;
    dataPtr->defined = false;
    dataPtr->parameters = parameters;
    // vytvoreni nove polozky v symtable
    BSTInsert(&(Table->root), Key.value, dataPtr, ndtFunction);
}

/*
 * Vlozeni dat o promenne do symtable
 */
void symTableInsertVariable(tSymtable* Table, string Key) { // vlozi do symtable zaznam s klicem Key a data dataPtr
    // alokovani pameti pro data
    tDataVariable * dataPtr;
    if ( (dataPtr = (tDataVariable*)malloc(sizeof(tDataVariable))) == NULL ) {
        return;
    }
    dataPtr->dataType = -1;
    // vytvoreni nove polozky v symtable
    BSTInsert(&(Table->root), Key.value, dataPtr, ndtVariable);
}

/*
 * Vyhledani prvku v symtable
 */
tBSTNodePtr symTableSearch(tSymtable* Table, string Key) { // vraci ukazatel na hledany uzel, pokud nenajde vraci NULL
    return BSTSearch(Table->root, Key.value);
}

/*
 * Smazani prvku ze symtable
 */
void symTableDelete(tSymtable* Table, string Key) {
    BSTDelete(&(Table->root), Key.value);
}

/*
 * Smazani cele symtable
 */
void symTableDispose(tSymtable* Table) {
    BSTDispose(&(Table->root));
}

/*
 * Vlozeni vestavenych funkci do symtable
 */
void symTableInsertBuiltInFunctions(tSymtable* Table) {
    tBSTNodePtr node;
    tDataFunction* data;

    /* --------------------Vestavena funkce length--------------------*/
    string length;
    stringInit(&length);

    // vlozeni funkce
    stringAddChars(&length, "length");
    symTableInsertFunction(Table, length);

    // vyhledani funkce
    node = symTableSearch(Table, length);
    data = (tDataFunction*)(node->Data);

    // inicializace
    data->declared = true; // deklarace
    data->defined = true; // definice
    stringAddChar(&(data->parameters), 's'); // parametry
    stringInit(&(data->paramName[0])); stringAddChar(&(data->paramName[0]), 's');
    data->returnDataType = sInteger; // navratova hodnota


    /* --------------------Vestavena funkce substr--------------------*/
    string substr;
    stringInit(&substr);

    // vlozeni funkce
    stringAddChars(&substr, "substr"); // nazev vestavene funkce
    symTableInsertFunction(Table, substr); // vlozeni vestavene funkce substr

    // vyhledani funkce
    node = symTableSearch(Table, substr);
    data = (tDataFunction*)(node->Data);

    // inicializace
    data->declared = true;
    data->defined = true;
    stringAddChars(&(data->parameters), "sii");
    stringInit(&(data->paramName[0])); stringAddChar(&(data->paramName[0]), 's');
    stringInit(&(data->paramName[1])); stringAddChar(&(data->paramName[1]), 'i');
    stringInit(&(data->paramName[2])); stringAddChar(&(data->paramName[2]), 'n');
    data->returnDataType = sString;


    /* --------------------Vestavena funkce asc--------------------*/
    string asc;
    stringInit(&asc);

    // vlozeni funkce
    stringAddChars(&asc, "asc");
    symTableInsertFunction(Table, asc);

    // vyhledani funkce
    node = symTableSearch(Table, asc);
    data = (tDataFunction*)(node->Data);

    // inicializace
    data->declared = true;
    data->defined = true;
    stringAddChars(&(data->parameters), "si");
    stringInit(&(data->paramName[0])); stringAddChar(&(data->paramName[0]), 's');
    stringInit(&(data->paramName[1])); stringAddChar(&(data->paramName[1]), 'i');
    data->returnDataType = sInteger;


    /* --------------------Vestavena funkce chr--------------------*/
    string chr;
    stringInit(&chr);

    // vlozeni funkce
    stringAddChars(&chr, "chr");
    symTableInsertFunction(Table, chr);

    // vyhledani funkce
    node = symTableSearch(Table, chr);
    data = (tDataFunction*)(node->Data);

    // inicializace
    data->declared = true;
    data->defined = true;
    stringAddChar(&(data->parameters), 'i');
    stringInit(&(data->paramName[0])); stringAddChar(&(data->paramName[0]), 'i');
    data->returnDataType = sString;
}