/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   scanner.c - lexikální analyzátor
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */

#include "error_code.h"
#include "scanner.h"
#include <string.h>

int getNextToken(char *attr){
    attr = attr;
    int state = 0; // stav automatu
    //char c; // aktualni stav

    while (1) {
        //c = getchar(); // nacteni dalsiho znaku

        switch (state) {
            case 0:
                // pocatecni stav automatu

                break;
            case 1:
                // komentar
                break;
            case 2:
                // indetifikator nebo klicove slovo
                break;
        }
    }
}