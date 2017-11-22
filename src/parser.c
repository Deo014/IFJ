/*
 * Předmět  :   IFJ / IAL
 * Soubor   :   parser.c - syntaktický a sémantický analyzátor
 * Projekt  :   Implementace překladače imperativního jazyka IFJ17
 * Tým č    :   21
 * Varianta :   1
 * Autoři   : xhribe02, David Hříbek
 *            xkoval14, Marek Kovalčík
 *            xvalus02, Ondřej Valušek
 *            xrutad00, Dominik Ruta
 */
#include <stdlib.h>
#include "parser.h"
#include "scanner.h"

//TODO eoly pred scopem
extern tSymtable glSymTable;
extern tSymtable table;
tDLListInstruction list;
tToken aktualni_token;
tBSTNodePtr node;
tBSTNodePtr glNode;
extern tToken next_exp_token;
//Pomocna promenna pro semantickou analyzu
bool comingFromDefinition;
//V tele programu se nastavi na true
bool inScope;
//Promenna, ktera povoluje zavolat adjustTokenType po navratu z vyrazu
bool exprAdjust = false;
//Pomocna promenna, pro zjisteni, zda vsechny deklarovane funkce byly i definovane
int allDeclaredAreDefined = ERROR_CODE_OK;
//Promenna pro uchovani cisla parametru pri zapisu nebo kontrole
int paramIndex;
//Pomocna promenna, ktera znaci ze funkce byla deklarovana nyni
bool declRecently = false;
//Pomocne parametry pro zapis vice parametru do tabulky
string paramName;
//pomocna promenna pro zapis funkce do tabluky
string functionName;
int paramsToDeclare;
bool inFunctionBody = false;
int expectedValue;
string processing;

//Pomocna funkce, ktera z obsahu atributu tokenu klicovych slov priradi cislo k pouziti ve switchi
int adjustTokenType(tToken tok) {
    if ((tok.type == 3) || (exprAdjust == true)) {
        exprAdjust = false;
        if (strcmp(tok.atr.value, "end") == 0)
            return sEnd;
        if (strcmp(tok.atr.value, "scope") == 0)
            return sScope;
        if (strcmp(tok.atr.value, "declare") == 0)
            return sDeclare;
        if (strcmp(tok.atr.value, "function") == 0)
            return sFuntion;
        if (strcmp(tok.atr.value, "as") == 0)
            return sAs;
        if (strcmp(tok.atr.value, "dim") == 0)
            return sDim;
        if (strcmp(tok.atr.value, "print") == 0)
            return sPrint;
        if (strcmp(tok.atr.value, "input") == 0)
            return sInput;
        if (strcmp(tok.atr.value, "if") == 0)
            return sIf;
        if (strcmp(tok.atr.value, "then") == 0)
            return sThen;
        if (strcmp(tok.atr.value, "else") == 0)
            return sElse;
        if (strcmp(tok.atr.value, "do") == 0)
            return sDo;
        if (strcmp(tok.atr.value, "while") == 0)
            return sWhile;
        if (strcmp(tok.atr.value, "loop") == 0)
            return sLoop;
        if (strcmp(tok.atr.value, "return") == 0)
            return sReturn;
        if (strcmp(tok.atr.value, "integer") == 0)
            return tInteger;
        if (strcmp(tok.atr.value, "double") == 0)
            return tDouble;
        if (strcmp(tok.atr.value, "string") == 0)
            return tString;
    }
    return tok.type;
}

//Funkce projde tabulku a zkontroluje jestli byly vsechny deklarovane funkce i definovane
void checkDefinitionsOfDeclarations(tBSTNodePtr TempTree) { /* vykresli sktrukturu binarniho stromu */
    if (TempTree != NULL) {

        checkDefinitionsOfDeclarations(TempTree->RPtr);
        if (TempTree->nodeDataType == ndtFunction) {
            if (!((((tDataFunction *) TempTree->Data)->defined) && (((tDataFunction *) TempTree->Data)->declared)))
                allDeclaredAreDefined = ERROR_CODE_SEM;
        }
        checkDefinitionsOfDeclarations(TempTree->LPtr);
    }
}

//Funkce nacte dalsi token a aktualizuje jeho typ
int dalsiToken() {
    aktualni_token = getNextToken();
    if (aktualni_token.type != sLexError) {
        aktualni_token.type = adjustTokenType(aktualni_token);
        return ERROR_CODE_OK;
    } else
        return ERROR_CODE_LEX;
}

int parse() {
    //inicializace tabulky symbolů a instrukčního listu
    int result;
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    //pokud hned prvni token je chybny
    if (aktualni_token.type == sLexError)
        result = ERROR_CODE_LEX;
    else
        //Prvni token je v poradku, volame prvni pravidlo
        result = Program();
    return result;
}

int Line() {
    int result;
    switch (aktualni_token.type) {
        case sEndOfLine:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Line();
        default:
            return ERROR_CODE_OK;

    }

    return result;
}
int Program() {
    //<Program> -> <Deklarace_funkci_definice_funkci><Telo_programu><EOF>
    int result;
    switch (aktualni_token.type) {
        case sEndOfLine:
            result = Line();
            if (result != ERROR_CODE_OK) return result;
        case sDeclare:
        case sFuntion:
        case sScope:
            result = Deklarace_fci_definice_fci();
            if (result != ERROR_CODE_OK) return result;
            result = Telo_programu();
            if (result != ERROR_CODE_OK) return result;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX; //posledni eol
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX; //eof
            if (aktualni_token.type != sEndOfFile) return ERROR_CODE_SYN;
            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Deklarace_fci_definice_fci() {
    int result;
    switch (aktualni_token.type) {
        //<Deklarace_funkci_definice_funkci> -> < Nekolik_Deklaraci_fce > <Deklarace_funkci_definice_funkci>
        case sDeclare:
            result = Nekolik_deklaraci_fce();
            if (result != ERROR_CODE_OK) return result;
            result = Deklarace_fci_definice_fci();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
            //<Deklarace_funkci_definice_funkci> -> < Nekolik_Definici_fce > <Deklarace_funkci_definice_funkci>
        case sFuntion:
            result = Nekolik_definici_fce();
            if (result != ERROR_CODE_OK) return result;
            result = Deklarace_fci_definice_fci();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
            //<Deklarace_funkci_definice_funkci> -> e
        case sScope:
            return ERROR_CODE_OK;

    }
    return ERROR_CODE_SYN;
}

int Nekolik_deklaraci_fce() {
    int result;
    switch (aktualni_token.type) {
        //<Nekolik_Deklaraci_fce> -> <Deklarace_fce> <Nekolik_Deklaraci_fce>
        case sDeclare:
            result = Deklarace_fce();
            if (result != ERROR_CODE_OK) return result;

            return Nekolik_deklaraci_fce();
//<Nekolik_Deklaraci_fce> -> e
        case sFuntion:
        case sScope:
            return ERROR_CODE_OK;
    }

    return ERROR_CODE_SYN;
}

int Nekolik_definici_fce() {
    int result;
    switch (aktualni_token.type) {
        //<Nekolik_Definici_fce> -> < Definice_fce><Nekolik_Definici_fce>
        case sFuntion:
            result = Definice_fce();
            if (result != ERROR_CODE_OK) return result;
            return Nekolik_definici_fce();
            //<Nekolik_Definici_fce> -> e
        case sDeclare:
        case sScope:
            return ERROR_CODE_OK;
    }

    return ERROR_CODE_SYN;
}

int Telo_programu() {
    //<Telo_programu> -> <Scope><Deklarace_promennych_a _prikazy><End><Scope>
    int result;
    switch (aktualni_token.type) {
        case sScope:
            //Pred vstupem do tela zkontrolujeme jestli vsechny deklarovane funkce byly i definovany
            checkDefinitionsOfDeclarations(glSymTable.root);
            if ((allDeclaredAreDefined) != ERROR_CODE_OK)
                return ERROR_CODE_SEM;
            inScope = true;

            glNode = symTableSearch(&glSymTable, functionName);
            paramsToDeclare = ((tDataFunction *) glNode->Data)->parameters.length;
            for (int i = 0; i < paramsToDeclare; i++) {
                symTableInsertVariable(&glSymTable, ((tDataFunction *) glNode->Data)->paramName[i]);
                i++;
            }
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;

            result = Line();
            if (result != ERROR_CODE_OK) return result;
            result = Deklarace_prom_a_prikazy();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEnd) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sScope) return ERROR_CODE_SYN;
            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Deklarace_fce() {
    //<Deklarace_fce> -> <Declare> <Hlavicka_fce><EOL>
    int result;
    switch (aktualni_token.type) {
        case sDeclare:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sFuntion) return ERROR_CODE_SYN;
            comingFromDefinition = false;
            result = Hlavicka_fce();
            if (result != ERROR_CODE_OK) return result;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Definice_fce() {
    //<Definice_fce> -> <Hlavicka_fce><EOL><Telo_fce><End><Function><EOL>
    int result;
    switch (aktualni_token.type) {
        case sFuntion:
            comingFromDefinition = 1;
            result = Hlavicka_fce();
            if (result != ERROR_CODE_OK) return result;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Telo_funkce();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEnd) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sFuntion) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Hlavicka_fce() {
    //<Hlavicka_fce> -> <Function><Id><<(><Parametry><)><As><Typ>
    int result;
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    if (aktualni_token.type != sIdentificator) return ERROR_CODE_SYN;
    //Jedna se o definici nebo deklaraci?
    if (comingFromDefinition == true) {
        //Pokus o definici
        //Podivame se jestli je fce vubec v tabulce
        functionName = aktualni_token.atr;
        if ((symTableSearch(&glSymTable, aktualni_token.atr)) != NULL) {
            //V tabulce uz je o teto funkci nejaky zaznam
            node = symTableSearch(&glSymTable, aktualni_token.atr);
            if (((tDataFunction *) node->Data)->defined == true) {
                //Uz byla definovana, pokus o redefinici -> error
                return ERROR_CODE_SEM;
            } else {
                //Byla deklarovana, nastavim ze uz bude i definovana
                ((tDataFunction *) node->Data)->defined = true;
            }
        } else {
            //Neni v tabulce, vlozim a reknu ze je od ted definovana a deklarovana
            symTableInsertFunction(&glSymTable, aktualni_token.atr);
            node = symTableSearch(&glSymTable, aktualni_token.atr);
            ((tDataFunction *) node->Data)->declared = true;
            declRecently = true;
            ((tDataFunction *) node->Data)->defined = true;

        }
    } else {
        //Comingfromdefinition==0, takze jsem tu z deklarace
        //Neco o funkci tam je, coz u deklarace nelze
        if ((symTableSearch(&glSymTable, aktualni_token.atr)) != NULL) return ERROR_CODE_SEM;
        else {
            //Jdu z deklarace a fce je deklarovana poprve
            symTableInsertFunction(&glSymTable, aktualni_token.atr);
            node = symTableSearch(&glSymTable, aktualni_token.atr);
            ((tDataFunction *) node->Data)->declared = true;
            declRecently = true;
            ((tDataFunction *) node->Data)->defined = false;
        }

    }


    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    if (aktualni_token.type != sLeftPar)return ERROR_CODE_SYN;
    paramIndex = 0;
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    result = Parametry();
    if (result != ERROR_CODE_OK)return result;
    if (aktualni_token.type != sRightPar)return ERROR_CODE_SYN;
    if (dalsiToken() != ERROR_CODE_OK)return ERROR_CODE_LEX;
    if (aktualni_token.type != sAs)return ERROR_CODE_SYN;
    if (dalsiToken() != ERROR_CODE_OK)return ERROR_CODE_LEX;
    result = Typ();
    if (result != ERROR_CODE_OK)return result;
    if (comingFromDefinition == 1) {
        if (declRecently != true) {
            switch (aktualni_token.type) {
                case tInteger:
                    if ((((tDataFunction *) node->Data)->returnDataType) != sInteger)
                        return ERROR_CODE_SEM;
                    break;

                case tDouble:
                    if ((((tDataFunction *) node->Data)->returnDataType) != sDouble)
                        return ERROR_CODE_SEM;
                    break;

                case tString:
                    if ((((tDataFunction *) node->Data)->returnDataType) != sString)
                        return ERROR_CODE_SEM;
                    break;

            }
        }
    } else {
        switch (aktualni_token.type) {
            case tInteger:
                ((tDataFunction *) node->Data)->returnDataType = sInteger;
                break;
            case tDouble:
                ((tDataFunction *) node->Data)->returnDataType = sDouble;
                break;
            case tString:
                ((tDataFunction *) node->Data)->returnDataType = sString;
                break;
        }

    }
    declRecently = false;
    return ERROR_CODE_OK;

}

int Typ() {
    switch (aktualni_token.type) {
        //<Typ> -> <Integer>
        //<Typ> -> <Double>
        //<Typ> -> <String>
        case tInteger:
        case tDouble:
        case tString:
            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Parametry() {
    int result;
    switch (aktualni_token.type) {
        //<Parametry> -> <Id><As><Typ><Dalsi_parametry>
        case sIdentificator:
            //Nebyla, vlozime ju
            paramName = aktualni_token.atr;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sAs) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Typ();
            if (result != ERROR_CODE_OK) return result;
            if (comingFromDefinition == 0) {
                //Jdu z deklarace, parametry nekontroluju jen zapisu
                switch (aktualni_token.type) {
                    case tInteger:
                        stringAddChar(&((tDataFunction *) node->Data)->parameters, 'i');
                        ((tDataFunction *) node->Data)->paramName[paramIndex] = paramName;
                        break;
                    case tDouble:
                        stringAddChar(&((tDataFunction *) node->Data)->parameters, 'd');
                        ((tDataFunction *) node->Data)->paramName[paramIndex] = paramName;
                        break;
                    case tString:
                        stringAddChar(&((tDataFunction *) node->Data)->parameters, 's');
                        ((tDataFunction *) node->Data)->paramName[paramIndex] = paramName;
                        break;
                }
            } else {

                //Jdu z definice a funkce byla deklarovana, musim zkontrolovat, jestli souhlasi typy parametru
                if (!declRecently) {
                    switch (aktualni_token.type) {
                        case tInteger:
                            //  stringAddChar(,'a');
                            if (((tDataFunction *) node->Data)->parameters.value[paramIndex] != 'i')

                                return ERROR_CODE_SEM;
                            break;
                        case tDouble:
                            if (((tDataFunction *) node->Data)->parameters.value[paramIndex] != 'd')
                                return ERROR_CODE_SEM;
                            break;
                        case tString:
                            if (((tDataFunction *) node->Data)->parameters.value[paramIndex] != 's')
                                return ERROR_CODE_SEM;
                            break;
                    }


                } else {
                    switch (aktualni_token.type) {
                        case tInteger:
                            //  stringAddChar(,'a');
                            stringAddChar(&((tDataFunction *) node->Data)->parameters, 'i');
                            ((tDataFunction *) node->Data)->paramName[paramIndex] = paramName;
                            break;
                        case tDouble:
                            stringAddChar(&((tDataFunction *) node->Data)->parameters, 'd');
                            ((tDataFunction *) node->Data)->paramName[paramIndex] = paramName;
                            break;
                        case tString:
                            stringAddChar(&((tDataFunction *) node->Data)->parameters, 's');
                            ((tDataFunction *) node->Data)->paramName[paramIndex] = paramName;
                            break;
                    }
                }
            }
            paramIndex++;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Dalsi_parametry();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
            //<Parametry> -> e
        case sRightPar:
            paramIndex = 0;
            return ERROR_CODE_OK;

    }
    return ERROR_CODE_SYN;
}

int Dalsi_parametry() {
    int result;
    switch (aktualni_token.type) {
        //<Dalsi_parametry> -> <,><Parametry><Dalsi_parametry>
        case sComma:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Parametry();
            if (result != ERROR_CODE_OK) return result;
            return Dalsi_parametry();
            //<Dalsi_parametry> -> e
        case sRightPar:
            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Telo_funkce() {
    //<Telo_fce> -> <Deklarace_promennych_a _prikazy>
    int result;
    symTableInit(&table);
    glNode = symTableSearch(&glSymTable, functionName);
    paramsToDeclare = ((tDataFunction *) glNode->Data)->parameters.length;
    for (int i = 0; i < paramsToDeclare; i++) {
        symTableInsertVariable(&table, ((tDataFunction *) glNode->Data)->paramName[i]);
        i++;
    }
    inFunctionBody = true;
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    result = Deklarace_prom_a_prikazy();
    if (result != ERROR_CODE_OK) return result;
    inFunctionBody = false;
    return ERROR_CODE_OK;
}

int Deklarace_promennych() {
    int result;
    switch (aktualni_token.type) {
        //< Deklarace _promennych> ->< Deklarace_promenne> <Deklarace_promennych>
        case sDim:
            result = Deklarace_promenne();
            if (result != ERROR_CODE_OK) return result;
            return Deklarace_promennych();
            //< Deklarace _promennych> -> e
        case sScope:
        case sEnd:
        case sIdentificator:
        case sEndOfLine:
        case sPrint:
        case sInput:
        case sIf:
        case sDo:
        case sReturn:
            return ERROR_CODE_OK;
    }

    return ERROR_CODE_SYN;
}

int Prikazy() {
    int result;
    switch (aktualni_token.type) {
        //<Prikazy> -> <Prikaz><Prikazy>
        case sPrint:
        case sInput:
        case sIf:
        case sDo:
        case sIdentificator:
        case sReturn:
            result = Prikaz();
            if (result != ERROR_CODE_OK) return result;
            ///if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            return Prikazy();
            //<Prikazy> -> ed
        case sEndOfLine:
        case sElse:
        case sLoop:
        case sEnd:
        case sDim:
        case sScope:
            //if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            return ERROR_CODE_OK;
    }

    return ERROR_CODE_SYN;
}

int Prikaz() {
    int result = 0;
    switch (aktualni_token.type) {
        //<Prikaz> -> <Print><Vyraz><;><Dalsi_vyrazy><EOL>
        case sPrint:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            expectedValue = -1;
            result = Vyraz();
            if (aktualni_token.type != sSemicolon) return ERROR_CODE_SYN;
            if (result != ERROR_CODE_OK) return result;
            result = Dalsi_vyrazy();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            break;
            //<Prikaz> -> <Input><Id><EOL>
        case sInput:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sIdentificator) return ERROR_CODE_SYN;
            //Promenna musi byt v tabulce symbolu
            if (!inScope) {
                if (!((symTableSearch(&table, aktualni_token.atr)) != NULL)) return ERROR_CODE_SEM;
                node = symTableSearch(&table, aktualni_token.atr);

            } else {
                if (!((symTableSearch(&glSymTable, aktualni_token.atr)) != NULL)) return ERROR_CODE_SEM;
                node = symTableSearch(&glSymTable, aktualni_token.atr);
            }

            //Overeni ze klic co jsme nasli je promenna a ne funkce
            if (node->nodeDataType != ndtVariable)
                return ERROR_CODE_SEM;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            break;
            //<Prikaz> -> <If><Vyraz><Then><EOL><Prikazy><Else><EOL><Prikazy><End><If><EOL>
        case sIf:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            expectedValue = -1;
            result = Vyraz();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sThen) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Prikazy();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sElse) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Prikazy();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEnd) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sIf) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            break;
            //<Prikaz> -> <Do><While><Vyraz><EOL><Prikazy><Loop><EOL>
        case sDo:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sWhile) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            expectedValue = -1;
            result = Vyraz();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Prikazy();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sLoop) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            break;
            //<Prikaz> -> <Id><=><Vyraz><EOL>
        case sIdentificator:
            if (!inScope) {
                if (!((symTableSearch(&table, aktualni_token.atr)) != NULL)) return ERROR_CODE_SEM;
                node = symTableSearch(&table, aktualni_token.atr);
                expectedValue = ((tDataVariable *) node->Data)->dataType;

            } else {
                if (!((symTableSearch(&glSymTable, aktualni_token.atr)) != NULL)) return ERROR_CODE_SEM;
                node = symTableSearch(&glSymTable, aktualni_token.atr);
                expectedValue = ((tDataVariable *) node->Data)->dataType;
            }
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sAssignment) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;

            result = Vyraz();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            break;
            //<Prikaz> -> <Return><Vyraz><EOL>
        case sReturn:
            //V hlavnim tele scope nemuze return byt
            if (inScope == true)
                return ERROR_CODE_SEM;


            if (((symTableSearch(&glSymTable, functionName)) == NULL)) return ERROR_CODE_SEM;
            node = symTableSearch(&glSymTable, functionName);
            expectedValue = ((tDataFunction *) node->Data)->returnDataType;


            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Vyraz();
            if (result != ERROR_CODE_OK) return result;
            if (aktualni_token.type != sEndOfLine) return ERROR_CODE_SYN;
            result = Line();
            if (result != ERROR_CODE_OK) return result;
            break;
    }
    return result;

}

int Deklarace_promenne() {
    //<Deklarace_promenne> -> <Dim><Id><As><Typ><Prirazeni_hodnoty>
    int result;
    switch (aktualni_token.type) {
        case sDim:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sIdentificator) return ERROR_CODE_SYN;
            //Kontrola, zda jiz promenna s timto ID nebyla deklarovana
            if (inScope) {
                if ((symTableSearch(&glSymTable, aktualni_token.atr)) != NULL) return ERROR_CODE_SEM;
                //Nebyla, vlozime ju
                symTableInsertVariable(&glSymTable, aktualni_token.atr);
                node = symTableSearch(&glSymTable, aktualni_token.atr);
            } else {

                if ((symTableSearch(&table, aktualni_token.atr)) != NULL) return ERROR_CODE_SEM;
                //Nebyla, vlozime ju
                symTableInsertVariable(&table, aktualni_token.atr);
                node = symTableSearch(&table, aktualni_token.atr);
            }
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            if (aktualni_token.type != sAs) return ERROR_CODE_SYN;
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Typ();
            if (result != ERROR_CODE_OK) return result;
            //uprava typu do tabulky na typ scanneru
            switch (aktualni_token.type) {
                case tInteger:
                    ((tDataVariable *) node->Data)->dataType = sInteger;
                    break;
                case tDouble:
                    ((tDataVariable *) node->Data)->dataType = sDouble;
                    break;
                case tString:
                    ((tDataVariable *) node->Data)->dataType = sString;
                    break;
            }
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;

            switch (aktualni_token.type) {
                case sAssignment:
                    result = Prirazeni_hodnoty();
                    if (result != ERROR_CODE_OK) return result;
                    result = Line();
                    if (result != ERROR_CODE_OK) return result;
                    return ERROR_CODE_OK;
                case sEndOfLine:
                    result = Line();
                    if (result != ERROR_CODE_OK) return result;
                    return ERROR_CODE_OK;

            }
            return ERROR_CODE_SYN;

    }

    return ERROR_CODE_SYN;
}

int Deklarace_prom_a_prikazy() {
    int result;
    switch (aktualni_token.type) {
        //< Deklarace_promennych_a _prikazy >-> <Prikazy><Deklarace_promennych_a _prikazy>
        case sPrint:
        case sInput:
        case sIf:
        case sDo:
        case sIdentificator:
        case sReturn:
            result = Prikazy();
            if (result != ERROR_CODE_OK) return result;
            result = Deklarace_prom_a_prikazy();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
            //< Deklarace_promennych_a _prikazy> -> <Deklarace_promennych> <Deklarace_promennych_a _prikazy>
        case sDim:
            result = Deklarace_promennych();
            if (result != ERROR_CODE_OK) return result;
            result = Deklarace_prom_a_prikazy();
            if (result != ERROR_CODE_OK) return result;
            return ERROR_CODE_OK;
            //<Deklarace_promennych_a _prikazy> -> e
        case sEnd:
        case sScope:
            return ERROR_CODE_OK;

    }
    return ERROR_CODE_SYN;
}

int Prirazeni_hodnoty() {
    int result;
    switch (aktualni_token.type) {
        //<Prirazeni_hodnoty> -><=><Vyraz>
        case sAssignment:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            result = Vyraz();
            return result;
            //<Prirazeni_hodnoty> -> e
        case sEndOfLine:
            if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
            return ERROR_CODE_OK;

    }
    return ERROR_CODE_SYN;
}


int Dalsi_vyrazy() {
    int result;
    if (dalsiToken() != ERROR_CODE_OK) return ERROR_CODE_LEX;
    switch (aktualni_token.type) {
        //<Dalsi_vyrazy> -> <Vyraz> <;><Dalsi_vyrazy>
        case sIdentificator:
        case sInteger:
        case sDouble:
        case sString:
            result = Vyraz();
            if (result != ERROR_CODE_OK) return result;
            result = Dalsi_vyrazy();
            if (result != ERROR_CODE_OK) return result;
            //<Dalsi_vyrazy> -> e
        case sEndOfLine:
        case sSemicolon:

            return ERROR_CODE_OK;
    }
    return ERROR_CODE_SYN;
}

int Vyraz() {
    int result;
    result = expression(aktualni_token, expectedValue);
    exprAdjust = true;
    aktualni_token = next_exp_token;
    aktualni_token.type = adjustTokenType(aktualni_token);
    return result;
}