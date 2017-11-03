/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   instList.h - Lineární seznam pro ukládání instrukcí
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#ifndef IFJ_INSTLIST_H
#define IFJ_INSTLIST_H

typedef struct {
    int instType;
    void *addr1;
    void *addr2;
    void *addr3;
} tInstr;

typedef struct tDLElem {
    tInstr instruction;
    struct tDLElem *lptr;
    struct tDLElem *rptr;
} *tDLElemPtr;


typedef struct {
    tDLElemPtr First;
    tDLElemPtr Act;
    tDLElemPtr Last;
} tDLList;

void DLInitList (tDLList *);
void DLDisposeList (tDLList *);
int DLInsertFirst (tDLList *, tInstr);
int DLInsertLast(tDLList *, tInstr);
void DLFirst (tDLList *);
void DLLast (tDLList *);
void DLCopyFirst (tDLList *, tInstr *);
void DLCopyLast (tDLList *, tInstr *);
void DLDeleteFirst (tDLList *);
void DLDeleteLast (tDLList *);
void DLPostDelete (tDLList *);
void DLPreDelete (tDLList *);
int DLPostInsert (tDLList *, tInstr);
int DLPreInsert (tDLList *, tInstr);
int DLCopy (tDLList *, tInstr *);
void DLActualize (tDLList *, tInstr);
void DLSucc (tDLList *);
void DLPred (tDLList *);
int DLActive (tDLList *);
#endif //IFJ_INSTLIST_H
