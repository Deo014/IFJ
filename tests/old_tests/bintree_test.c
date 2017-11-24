/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   bintree_test.c - Test implementace binárního stromu
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#include "../../src/bintree.c"

int main(){
    // Deklarace struktury reprezentující tokeny
    struct Tokens{ int id; char znaky[20];}token;

    // Definice dvou testovacích struktur
    struct Tokens token1, token2;
    token1.id = 1; token2.id = 2;
    strcpy(token1.znaky, "neco1"); strcpy(token2.znaky, "neco2");

    // Deklarace binárního stromu
    TreePointer strom;
    // Inicializace binárního stromu
    BSTinit(&strom);
    printf("Tree initialization:\t\t %d\n", checkTreeInit);

    // Vložení dvou testovacích dat do binárního stromu
    // "a" = key, &token1 jsou data předávaná jako struktura
    BSTinsert(&strom,"a", &token1);
    printf("Inserting value 'a':\t\t %d\n", checkTreeInsert);
    BSTinsert(&strom,"b", &token2);
    printf("Inserting value 'b':\t\t %d\n", checkTreeInsert);

    // Vyhledání a uložení uzlu do foundedNodeA hledaného v '&strom' podle klíče "a"
    TreeStructure foundedNodeA = BSTsearch(&strom,"a");
    printf("Searching for 'a' in tree:\t %d\n", checkTreeNodesSearch);
    TreeStructure foundedNodeB = BSTsearch(&strom,"b");
    printf("Searching for 'b' in tree:\t %d\n", checkTreeNodesSearch);
    TreeStructure foundedNodeC = BSTsearch(&strom,"c");
    printf("Searching for 'c' in tree:\t %d\n", checkTreeNodesSearch);

    // Vypsání hodnot nalezenách uzlů na stdout
    printf("First inserted token:\t\t id=%d \t\t string=%s\n",
           ((struct Tokens*)(foundedNodeA->data))->id,((struct Tokens*)(foundedNodeA->data))->znaky);
    printf("Second inserted token:\t\t id=%d \t\t string=%s\n",
           ((struct Tokens*)(foundedNodeB->data))->id,((struct Tokens*)(foundedNodeB->data))->znaky);

    // Zdrušení stromu a všechno jeho podstromů
    BSTdispose(&strom);
    printf("Tree destroying:\t\t %d\n", checkTreeDispose);

	return 0;
}

