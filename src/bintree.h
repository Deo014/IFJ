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

// pomocné proměnné pro účely testování
bool checkTreeInit = false;
bool checkTreeDispose = false;
bool checkTreeSearch = false;
bool checkTreeInsert = false;
bool checkTreeNodesSearch = false;
bool checkTreeNodesDelete = false;

// Deklarace elementu binárního stromu
typedef struct tree_structure {
    struct tree_structure *left;
    struct tree_structure *right;
    void *data;
    char *key;
} *TreeStructure;

// Struktura binárního stromu
typedef struct {
    TreeStructure root;
} TreePointer;

// Deklarace funkcí binárního stromu
void BSTinit(TreePointer *);
void BSTdispose(TreePointer *);
TreeStructure BSTsearch(TreePointer *, char *);
ERROR_CODE BSTinsert(TreePointer *,char *,void *);
TreeStructure treeNodesSearch(TreeStructure, char *key);
void BSTdelete(TreeStructure);


#endif //IFJ_BINTREE_H
