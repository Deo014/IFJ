/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   bintree.h - Hlavičkový soubor binárního stromu
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#ifndef IFJ_BINTREE_H
#define IFJ_BINTREE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include"error_code.h"

// Deklarace elementu binárního stromu
typedef struct tree {
    struct tree *left;
    struct tree *right;
    void *data;
    char *key;
} *TreeStructure;

// Struktura binárního stromu
typedef struct {
    TreeStructure root;
} TreePointer;

// Deklarace funkcí binárního stromu
void treeInit(TreePointer *);
void treeDestroy(TreePointer *);
TreeStructure treeSearch(TreePointer *, char *);
ERROR_CODE treeInsert(TreePointer *,char *,void *);
TreeStructure treeNodesSearch(Tree, char *key);
void treeNodesDestroy(Tree);

#endif //IFJ_BINTREE_H
