/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   instList.c - Lineární seznam pro ukládání instrukcí
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#include "instList.h"
#include <stdio.h>
#include <stdlib.h>

void DLInitList (tDLList *L) {
    // pocatecni ukazatele jsou NULL
    L->First = NULL;
    L->Last = NULL;
    L->Act = NULL;
}

void DLDisposeList (tDLList *L) {
    tDLElemPtr elemToDelete;
    // dokud neni seznam prazdny: mazani posledniho prvku
    while ( (elemToDelete = L->Last) != NULL) {
        L->Last = elemToDelete->lptr; // poslednim prvkem se stane predposledni prvek
        elemToDelete->lptr = elemToDelete->rptr = NULL;
        free(elemToDelete);
    }
    L->Last = L->First = L->Act = NULL;
}

int DLInsertFirst (tDLList *L, tInstr instruction) {
    // vytvoreni noveho prku + alokovani pameti pro novy prvek
    tDLElemPtr newElem;
    if( (newElem = (struct tDLElem*)malloc(sizeof(struct tDLElem))) == NULL ) {
        return 0;
    }
    newElem->instruction = instruction;

    // pokud byl seznam prazdny: pripojeni noveho prvku na zacatek
    if (L->First == NULL) {
        L->First = newElem;
        L->Last = newElem;
        newElem->lptr = NULL;
        newElem->rptr = NULL;
    }
    else {
        tDLElemPtr elemToJoin = L->First; // prvek pred ktery vlozime novy prvek
        L->First = newElem;
        newElem->lptr = NULL;
        newElem->rptr = elemToJoin;
        elemToJoin->lptr = newElem;
    }
    return 1;
}

int DLInsertLast(tDLList *L, tInstr instruction) {
    // vytvoreni noveho prku + alokovani pameti pro novy prvek
    tDLElemPtr newElem;
    if( (newElem = (struct tDLElem*)malloc(sizeof(struct tDLElem))) == NULL ) {
        return 0;
    }
    newElem->instruction = instruction;

    // pokud byl seznam prazdny: pripojeni noveho prvku na zacatek
    if (L->First == NULL) {
        L->First = newElem;
        L->Last = newElem;
        newElem->lptr = NULL;
        newElem->rptr = NULL;
    }
    else {
        L->Last->rptr = newElem;
        newElem->lptr = L->Last;
        newElem->rptr = NULL;
        L->Last = newElem;
    }
    return 1;
}

void DLFirst (tDLList *L) {
    L->Act = L->First;
}

void DLLast (tDLList *L) {
    L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, tInstr *instruction) {
    // pokud je seznam prazdny: chyba
    if (L->First == NULL) {
        return;
    }
    // vraci data prvniho prvku seznamu
    *instruction = L->First->instruction;
}

void DLCopyLast (tDLList *L, tInstr *instruction) {
    // pokud je seznam prazdny: chyba
    if (L->Last == NULL) {
        return;
    }
    // vraci data prvniho prvku seznamu
    *instruction = L->Last->instruction;
}

void DLDeleteFirst (tDLList *L) {
    if (L->First) {
        // pokud je prvni prvek aktivni: zruseni aktivity
        if (L->Act == L->First)
            L->Act = NULL;
        // odstraneni prvniho prvku
        L->First = L->First->rptr;
        free(L->First->lptr); // smazani nadbytecneho prvku
        L->First->lptr = NULL;
    }
}

void DLDeleteLast (tDLList *L) {
    if (L->Last) {
        // pokud je posledni prvek aktivni: zruseni aktivity
        if (L->Act == L->Last)
            L->Act = NULL;
        // odstraneni posledniho prvku
        L->Last = L->Last->lptr; // predposledni prvek se stane poslednim
        free(L->Last->rptr); // smazani nadbytecneho prvku
        L->Last->rptr = NULL;
    }
}

void DLPostDelete (tDLList *L) {
    if (L->Act && L->Act != L->Last) {
        tDLElemPtr elemToDelete = L->Act->rptr;
        tDLElemPtr elemToJoin = elemToDelete->rptr;
        // pokud neni prvek pred aktivnim prvkem poslednim prvkem seznamu
        if (elemToJoin) {
            elemToJoin->lptr = L->Act;
            L->Act->rptr = elemToJoin;
        }
        else {
            L->Act->rptr = NULL;
            L->Last = L->Act;
        }
        free(elemToDelete);
    }
}

void DLPreDelete (tDLList *L) {
    if (L->Act && L->Act != L->First) {
        tDLElemPtr elemToDelete = L->Act->lptr;
        tDLElemPtr elemToJoin = elemToDelete->lptr;
        // pokud neni prvek za aktivnim prvkem prvnim prvkem seznamu
        if (elemToJoin) {
            elemToJoin->rptr = L->Act;
            L->Act->lptr = elemToJoin;
        }
        else {
            L->Act->lptr = NULL;
            L->First = L->Act;
        }
        free(elemToDelete);
    }
}
int DLPostInsert (tDLList *L, tInstr instruction) {
    if (L->Act) {
        // vytvoreni noveho prku + alokovani pameti pro novy prvek
        tDLElemPtr newElem;
        if( (newElem = (struct tDLElem*)malloc(sizeof(struct tDLElem))) == NULL ) {
            return 0;
        }
        newElem->instruction = instruction;

        tDLElemPtr elemToJoin = L->Act->rptr;
        // vlozeni do seznamu
        // pokud je aktivni prvek poslednim prvkem seznamu: vlozeni prvku na konec
        if (elemToJoin == NULL) {
            L->Act->rptr = newElem;
            newElem->lptr = L->Act;
            newElem->rptr = NULL;
            L->Last = newElem;
        }
        else { // vlozeni prvku mezi Act a elemToJoin
            newElem->lptr = L->Act;
            newElem->rptr = elemToJoin;
            L->Act->rptr = newElem;
            elemToJoin->lptr = newElem;
        }
    }
    return 1;
}

int DLPreInsert (tDLList *L, tInstr instruction) {
    if (L->Act) {
        // vytvoreni noveho prku + alokovani pameti pro novy prvek
        tDLElemPtr newElem;
        if( (newElem = (struct tDLElem*)malloc(sizeof(struct tDLElem))) == NULL ) {
            return 0;
        }
        newElem->instruction = instruction;

        tDLElemPtr elemToJoin = L->Act->lptr;
        // vlozeni do seznamu
        // pokud je aktivni prvek prvnim prvkem seznamu: vlozeni prvku na zacatek
        if (elemToJoin == NULL) {
            L->Act->lptr = newElem;
            newElem->rptr = L->Act;
            newElem->lptr = NULL;
            L->First = newElem;
        }
        else { // vlozeni prvku mezi Act a elemToJoin
            newElem->rptr = L->Act;
            newElem->lptr = elemToJoin;
            L->Act->lptr = newElem;
            elemToJoin->rptr = newElem;
        }
    }
    return 1;
}

int DLCopy (tDLList *L, tInstr *instruction) {
    //pokud je seznam prazdny: chyba
    if (L->Act == NULL) {
        return 0;
    }
    // vraci data aktivniho prvku seznamu
    *instruction = L->Act->instruction;
    return 1;
}

void DLActualize (tDLList *L, tInstr instruction) {
    // pokud je seznam aktivni
    if (L->Act) {
        // aktualizuj data
        L->Act->instruction = instruction;
    }
}

void DLSucc (tDLList *L) {
    // pokud je seznam aktivni, posune aktivitu na nasledujici prvek
    if (L->Act) {
        L->Act = L->Act->rptr;
    }
}

void DLPred (tDLList *L) {
    // pokud je seznam aktivni
    if (L->Act) {
        // posune aktivitu na dalsi prvek
        L->Act = L->Act->lptr;
    }
}
int DLActive (tDLList *L) {
    return(L->Act != NULL);
}