/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   string.c - pomocná knihovna pro lexikální analyzátor
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
#include <string.h>

int stringInit(string *str) {
    // alokovani pameti
    if ( (str->value = (char*) malloc(INIT_ALLOC_SIZE * sizeof(char))) == NULL )
        return ERROR_CODE_INTERNAL;
    str->length = 0;
    str->value[0] = '\0';
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

void stringUpdateLastChar(string *str, char c) {
    if (str->length > 0) {
        str->value[str->length-1] = c;
    }
}

int stringAddFirstChar(string * str, char c) {
    // pokud neni misto pro dalsi znak
    if (str->length+1 >= str->lengthAllocated) {
        // realokace pameti: navyseni o INIT_ALLOC_SIZE
        if ( (str->value = (char*)realloc(str->value, str->lengthAllocated + INIT_ALLOC_SIZE)) == NULL )
            return ERROR_CODE_INTERNAL;
        str->lengthAllocated = str->lengthAllocated + INIT_ALLOC_SIZE;
    }
    // posunuti retezce o jeden znak: vytvoreni mista pro zapsani prvniho znaku
    for (int i=str->length; i>=0; i--) {
        str->value[i+1] = str->value[i];
    }
    // pridani znaku na zacatek retezce
    str->value[0] = c;
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

char stringGetLastChar(string *str) {
    if (str->length > 0)
        return str->value[str->length-1];
    else
        return -1;
}

int stringIsKeyWord(string *str) {
    char *keywords[] = {
            "as\0" , "asc\0" , "declare\0" , "dim\0" , "do\0" , "double\0" , "else\0" , "end\0", "chr\0",
            "function\0", "if\0", "input\0", "integer\0", "length\0", "loop\0", "print\0", "return\0",
            "scope\0", "string\0", "substr\0", "then\0" , "while\0"
    };
    unsigned int keywordsLength = sizeof(keywords) / sizeof(keywords[0]); // pocet prvku v poli keywords
    // porovnani tokenu s klicovymi slovy
    for (unsigned int i=0; i<keywordsLength; i++) {
        if ( strcmp(keywords[i], str->value) == 0 )
            return ERROR_CODE_TRUE;
    }
    return ERROR_CODE_FALSE;
}

int stringIsResKeyWord(string *str) {
    char *resKeywords[] = {
            "and\0", "boolean\0", "continue\0", "elseif\0", "exit\0", "false\0", "for\0", "next\0",
            "not\0", "or\0", "shared\0", "static\0", "true\0"
    };
    unsigned int resKeywordsLength = sizeof(resKeywords) / sizeof(resKeywords[0]); // pocet prvku v poli keywords
    // porovnani tokenu s klicovymi slovy
    for (unsigned int i=0; i<resKeywordsLength; i++) {
        if ( strcmp(resKeywords[i], str->value) == 0 )
            return ERROR_CODE_TRUE;
    }
    return ERROR_CODE_FALSE;
}

void stringToLowercase(string *str) {
    // prevod na lowercase
    for (int i=0; i<str->length; i++) {
        if ( (str->value[i] >= 65) && (str->value[i] <= 90) ) {
            str->value[i] = str->value[i] + 32;
        }
    }
}

// FUNKCE PRO PRACI SE ZNAKY
void charUndo(char c) {
    ungetc(c, stdin);
}

int charIsSpace(char c) {
    if (c == ' ')
        return ERROR_CODE_TRUE;
    else
        return ERROR_CODE_FALSE;
}

int charIsTab(char c) {
    if ( c == '\t' )
        return ERROR_CODE_TRUE;
    else
        return ERROR_CODE_FALSE;
}

int charIsDigit(char c) {
    if (c >= '0' && c <= '9')
        return ERROR_CODE_TRUE;
    else
        return ERROR_CODE_FALSE;
}

int charIsLetter(char c) {
    if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') )
        return ERROR_CODE_TRUE;
    else
        return ERROR_CODE_FALSE;
}

int charToDec(char c) {
    return c-48;
}

char decToChar(int c) {
    return c+48;
}