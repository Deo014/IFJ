/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   scaner.c - lexikální analyzátor
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */
#include "string.h"
#include "error_code.h"
#include <malloc.h>

int stringInit(string *str) {
    // alokovani pameti
    if ( (str->value = (char*)malloc(INIT_ALLOC_SIZE * sizeof(char))) == NULL )
        return ERROR_CODE_INTERNAL;
    str->length = 0;
    str->lengthAllocated = INIT_ALLOC_SIZE;
    // pokud vse probehlo v poradku
    return ERROR_CODE_OK;
}

int stringAddChar(string *str, char c) {
    // pokud neni misto pro dalsi znak
    if (str->length+1 >= str->lengthAllocated) {
        // realokace pameti: navyseni o INIT_ALLOC_SIZE
        if ( (str->value = (char*)realloc(str->value, str->lengthAllocated + INIT_ALLOC_SIZE)) == NULL )
            return ERROR_CODE_INTERNAL;
        str->lengthAllocated = str->lengthAllocated + INIT_ALLOC_SIZE;
    }
    // pridani znaku na konec retezce
    str->value[str->length] = c;
    str->value[str->length+1] = '\0';
    str->length++;
    // pokud vse probehlo v poradku
    return ERROR_CODE_OK;
}

int stringClear(string *str) {
    // smaze znaky v retezci
    for (int i = 0; i < str->lengthAllocated; i++) {
        str->value[i] = '\0';
    }
    str->length = 0;
    // pokud vse probehlo v poradku
    return ERROR_CODE_OK;
}


// FUNKCE PRO PRACI SE ZNAKY

int charIsSpace(char c) {
    if (c == ' ')
        return ERROR_CODE_TRUE;
    else
        return ERROR_CODE_FALSE;
}