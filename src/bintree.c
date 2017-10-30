/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   bintree.c - Implementace binárního stromu
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#include "bintree.h"

// Funkce inicialializuje nový binární strom s ukazatelem Tree
void BSTinit(TreePointer *Tree) {
    checkTreeInit = false;
    Tree->root=NULL;
    checkTreeInit = true;
    return;
}

// Funkce odstraní strom Tree a uvolní všechny jeho prvky
void BSTdispose(TreePointer *Tree) {
    checkTreeDispose = false;
    if(Tree->root == NULL)
        return;
    else if(Tree->root != NULL){
        BSTdelete(Tree->root);
        Tree->root = NULL;
        checkTreeDispose = true;
        return;
    }
}
// Funkce na najde a vrátí prvek v binárního stromu Tree podle klíče key, vlevo jsou menší hodnoty, vpravo větší
TreeStructure BSTsearch(TreePointer *Tree, char *key) {
    checkTreeSearch = false;
    if(Tree == NULL)
        return NULL;
    else if(key == NULL)
        return NULL;
    else{
        TreeStructure foundedNode = treeNodesSearch(Tree->root, key);
        checkTreeSearch = true;
        return foundedNode;
    }
}
// Funkce uloží do binárního stromu Tree prvek s klíčem key a jeho daty
ERROR_CODE BSTinsert(TreePointer *Tree, char *key,void *data) {
    int checkStrcmp = 0; checkTreeInsert = false;
    //kořen stromu je prázdný -> inicializují se jeho atributy na NULL
    if(Tree->root == NULL){
        Tree->root = malloc(sizeof(struct tree_structure));
        if(Tree->root!=NULL){
            Tree->root->left=NULL;
            Tree->root->right=NULL;
            Tree->root->data=data;
            Tree->root->key=key;
        }
    // kořen stromu není prázdný, provede se vložení
    }else if(Tree->root != NULL){
        for(TreeStructure pom=Tree->root; pom != NULL; /*další ukazatel se nastaví podle checkStrcmp*/){
            if((checkStrcmp = strcmp(pom->key,key)) == 0){
                pom->data = data;
                checkTreeInsert = true;
                return ERROR_CODE_OK;
            }else if(checkStrcmp < 0){
                //***************************
                if(pom->left==NULL){
                    pom->left=malloc(sizeof(struct tree_structure));
                    if(pom->left!=NULL){
                        pom->left->left=NULL;
                        pom->left->right=NULL;
                        pom->left->data=data;
                        pom->left->key=key;
                    }
                }
                else // posunutí na levého potomka
                    pom=pom->left;
            }else if(checkStrcmp > 0){
                //**********************
                if(pom->right==NULL){
                    pom->right=malloc(sizeof(struct tree_structure));
                    if(pom->right!=NULL){
                        pom->right->left=NULL;
                        pom->right->right=NULL;
                        pom->right->data=data;
                        pom->right->key=key;
                    }
                }
                // není to list, je třeba se posunout ádl
                else
                    pom=pom->right;
                //******************************
            }
        }
    }
    checkTreeInsert = true;
    return  ERROR_CODE_OK;
}
// Vrací ukazatel na hledanou položku node hledanou podle klíče key
TreeStructure treeNodesSearch(TreeStructure node, char *key) {
    int checkStrcmp = 0; checkTreeNodesSearch = false;
    if(node==NULL)
        return NULL;
    else
        if((checkStrcmp = strcmp(node->key, key)) == 0) {
            checkTreeNodesSearch = true;
            return node;
        }
        else if(checkStrcmp < 0)
            return treeNodesSearch(node->left,key);
        else if(checkStrcmp > 0)
            return treeNodesSearch(node->right,key);
}

// Funkce rekurzivně odstraní podstromy uzlu node
void BSTdelete(TreeStructure node) {
    checkTreeNodesDelete = false;
    if(node->left!=NULL)
        BSTdelete(node->left);
    if(node->right!=NULL)
        BSTdelete(node->right);
    free(node);
    checkTreeNodesDelete = true;
    return;
}


