/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   string.c - test strlen, strcmp
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#include <stdio.h>
#include "../../src/string.c"

int main(){
    string str1; string str2;
    stringInit(&str1); stringInit(&str2);
    printf("add 'str2' -> 'c'\n");
    stringAddFirstChar(&str1, 'c');
    printf("Delka 'str1' je %d\n", stringLenght(&str1));

    if(stringCompare(&str1, &str2))
        printf("Retezce 'str1' a 'str2' jsou shodne\n");
    else
        printf("Retezce 'str1' a 'str2' nejsou shodne\n");

    printf("add 'str2' -> 'c'\n");
    stringAddFirstChar(&str2, 'c');

    if(stringCompare(&str1, &str2))
        printf("Retezce 'str1' a 'str2' jsou shodne\n");
    else
        printf("Retezce 'str1' a 'str2' nejsou shodne\n");

}
