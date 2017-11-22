#include "../../../src/scanner.h"
#include "../../../src/string.c"
#include "../../../src/string.h"
#include <string.h>

#define POCET_TOKENU_TEST_1 6
#define POCET_TOKENU_TEST_2 7
#define POCET_TOKENU_TEST_3 11
#define POCET_TOKENU_TEST_4 38
#define POCET_TOKENU_TEST_5 5
#define POCET_TOKENU_TEST_6 31

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define RST "\x1B[0m"

int test1() {

    //Obsah: int Integer 42 _
    tToken *tokenyTestOcekavane = malloc(POCET_TOKENU_TEST_1 * sizeof(tToken));
    int testResult = 0;
    if (tokenyTestOcekavane != NULL) {
        //Alokace ok, nastavime ocekavane hodnoty
        tokenyTestOcekavane[0].type = sIdentificator;
        tokenyTestOcekavane[0].atr.value = "int";
        tokenyTestOcekavane[1].type = sKeyWord;
        tokenyTestOcekavane[1].atr.value = "integer";
        tokenyTestOcekavane[2].type = sInteger;
        tokenyTestOcekavane[2].atr.value = "42";
        tokenyTestOcekavane[3].type = 1;
        tokenyTestOcekavane[3].atr.value = "_";
        tokenyTestOcekavane[4].type = 32;
        tokenyTestOcekavane[4].atr.value = "EOL";
        tokenyTestOcekavane[5].type = 33;
        tokenyTestOcekavane[5].atr.value = "EOF";
        tToken tokenObdrzeny;
        int i = 0;

        while (i < POCET_TOKENU_TEST_1) {
            tokenObdrzeny = getNextToken();

           /* 
	    printf("Obdrzeny:  %s\n",tokenObdrzeny.atr.value);
            printf("Ocekavany: %s\n",tokenyTestOcekavane[i].atr.value);
            printf("Obdrzeny:  %d\n",tokenObdrzeny.type);
            printf("Ocekavany: %d\n\n",tokenyTestOcekavane[i].type);
           */
            if ((strcmp(tokenObdrzeny.atr.value, tokenyTestOcekavane[i].atr.value) != 0) ||
                (tokenObdrzeny.type != tokenyTestOcekavane[i].type))
                testResult = 1;
            i++;
        }

    }

    free(tokenyTestOcekavane);
    return testResult;
}

int test2() {

    //Obsah: operatory
    tToken *tokenyTestOcekavane = malloc(POCET_TOKENU_TEST_2 * sizeof(tToken));
    int testResult = 0;
    if (tokenyTestOcekavane != NULL) {
        //Alokace ok, nastavime ocekavane hodnoty
        tokenyTestOcekavane[0].type = sPlus;
        tokenyTestOcekavane[0].atr.value = "+";
        tokenyTestOcekavane[1].type = sMinus;
        tokenyTestOcekavane[1].atr.value = "-";
        tokenyTestOcekavane[2].type = sMultiply;
        tokenyTestOcekavane[2].atr.value = "*";
        tokenyTestOcekavane[3].type = sDivideD;
        tokenyTestOcekavane[3].atr.value = "/";
        tokenyTestOcekavane[4].type = sDivideI;
        tokenyTestOcekavane[4].atr.value = "\\";
        tokenyTestOcekavane[5].type = sEndOfLine;
        tokenyTestOcekavane[5].atr.value = "EOL";
        tokenyTestOcekavane[6].type = sEndOfFile;
        tokenyTestOcekavane[6].atr.value = "EOF";
        tToken tokenObdrzeny;
        int i = 0;

        while (i < POCET_TOKENU_TEST_2) {
            tokenObdrzeny = getNextToken();
            if ((strcmp(tokenObdrzeny.atr.value, tokenyTestOcekavane[i].atr.value) != 0) ||
                (tokenObdrzeny.type != tokenyTestOcekavane[i].type))
                testResult = 1;
            i++;
        }

    }

    free(tokenyTestOcekavane);
    return testResult;
}

int test3() {

    //Obsah: operatory,zavorky,strednik
    tToken *tokenyTestOcekavane = malloc(POCET_TOKENU_TEST_3 * sizeof(tToken));
    int testResult = 0;
    if (tokenyTestOcekavane != NULL) {
        //Alokace ok, nastavime ocekavane hodnoty
        tokenyTestOcekavane[0].type = sNotEqual;
        tokenyTestOcekavane[0].atr.value = "<>";
        tokenyTestOcekavane[1].type = sMore;
        tokenyTestOcekavane[1].atr.value = ">";
        tokenyTestOcekavane[2].type = sMoreEqual;
        tokenyTestOcekavane[2].atr.value = ">=";
        tokenyTestOcekavane[3].type = sAssignment;
        tokenyTestOcekavane[3].atr.value = "=";
        tokenyTestOcekavane[4].type = sLeftPar;
        tokenyTestOcekavane[4].atr.value = "(";
        tokenyTestOcekavane[5].type = sLessEqual;
        tokenyTestOcekavane[5].atr.value = "<=";
        tokenyTestOcekavane[6].type = sLess;
        tokenyTestOcekavane[6].atr.value = "<";
        tokenyTestOcekavane[7].type = sRightPar;
        tokenyTestOcekavane[7].atr.value = ")";
        tokenyTestOcekavane[8].type = sSemicolon;
        tokenyTestOcekavane[8].atr.value = ";";
        tokenyTestOcekavane[9].type = sEndOfLine;
        tokenyTestOcekavane[9].atr.value = "EOL";
        tokenyTestOcekavane[10].type = sEndOfFile;
        tokenyTestOcekavane[10].atr.value = "EOF";
        tToken tokenObdrzeny;
        int i = 0;

        while (i < POCET_TOKENU_TEST_3) {
            tokenObdrzeny = getNextToken();
            if ((strcmp(tokenObdrzeny.atr.value, tokenyTestOcekavane[i].atr.value) != 0) ||
                (tokenObdrzeny.type != tokenyTestOcekavane[i].type))
                testResult = 1;
            i++;
        }

    }

    free(tokenyTestOcekavane);
    return testResult;
}

int test4() {
    //Obsah: vsechna klicova slova + identifikator na pozici 25
    tToken *tokenyTestOcekavane = malloc(POCET_TOKENU_TEST_4 * sizeof(tToken));
    char *valueArray[38] = {"as", "asc", "declare", "dim", "do", "double", "else", "end", "chr", "function", "if",
                            "input", "integer", "length", "loop", "print", "return", "scope", "string", "substr",
                            "then", "while", "and", "boolean", "continue", "totoneniklicoveslovo", "elseif",
                            "exit", "false", "for", "next", "not", "or", "shared", "static", "true", "EOL", "EOF"};
    int testResult = 0;
    if (tokenyTestOcekavane != NULL) {
        //Alokace ok, nastavime ocekavane hodnoty
        for (int j = 0; j < POCET_TOKENU_TEST_4; j++) {
            //Nastaveni typu(cisla) tokenu na ocekavanou hodnotu
            if (j == 25)
                tokenyTestOcekavane[j].type = sIdentificator;
            else if (j == 36)
                tokenyTestOcekavane[j].type = sEndOfLine;
            else if (j == 37)
                tokenyTestOcekavane[j].type = sEndOfFile;
            else
                tokenyTestOcekavane[j].type = sKeyWord;

        }

        for (int k = 0; k < POCET_TOKENU_TEST_4; k++) {
            //Nastaveni atributu tokenu na ocekavane slovo
            tokenyTestOcekavane[k].atr.value = valueArray[k];
        }

//Porovnani obdrzeneho a ocekavaneho
        tToken tokenObdrzeny;
        int i = 0;
        while (i < POCET_TOKENU_TEST_4) {
            tokenObdrzeny = getNextToken();


            if ((strcmp(tokenObdrzeny.atr.value, tokenyTestOcekavane[i].atr.value) != 0) ||
                (tokenObdrzeny.type != tokenyTestOcekavane[i].type))
                testResult = 1;
            i++;
        }

    }

    free(tokenyTestOcekavane);
    return testResult;
}

int test5() {

    //Obsah: double hodnoty
    tToken *tokenyTestOcekavane = malloc(POCET_TOKENU_TEST_5 * sizeof(tToken));
    int testResult = 0;
    if (tokenyTestOcekavane != NULL) {
        //Alokace ok, nastavime ocekavane hodnoty
        tokenyTestOcekavane[0].type = sDouble;
        tokenyTestOcekavane[0].atr.value = "2e-1";
        tokenyTestOcekavane[1].type = sDouble;
        tokenyTestOcekavane[1].atr.value = "10E2";
        tokenyTestOcekavane[2].type = sDouble;
        tokenyTestOcekavane[2].atr.value = "0.25E5";
        tokenyTestOcekavane[3].type = sEndOfLine;
        tokenyTestOcekavane[3].atr.value = "EOL";
        tokenyTestOcekavane[4].type = sEndOfFile;
        tokenyTestOcekavane[4].atr.value = "EOF";
        tToken tokenObdrzeny;
        int i = 0;

        while (i < POCET_TOKENU_TEST_5) {
            tokenObdrzeny = getNextToken();

            if ((strcmp(tokenObdrzeny.atr.value, tokenyTestOcekavane[i].atr.value) != 0) ||
                (tokenObdrzeny.type != tokenyTestOcekavane[i].type))
                testResult = 1;
            i++;
        }

    }

    free(tokenyTestOcekavane);
    return testResult;
}

int test6() {

    //kratky souvisly kod
    tToken *tokenyTestOcekavane = malloc(POCET_TOKENU_TEST_6 * sizeof(tToken));
    int testResult = 0;
    if (tokenyTestOcekavane != NULL) {
        //Alokace ok, nastavime ocekavane hodnoty
        tokenyTestOcekavane[0].type = sKeyWord;
        tokenyTestOcekavane[0].atr.value = "scope";
        tokenyTestOcekavane[1].type = sEndOfLine;
        tokenyTestOcekavane[1].atr.value = "EOL";
        tokenyTestOcekavane[2].type = sKeyWord;
        tokenyTestOcekavane[2].atr.value = "dim";
        tokenyTestOcekavane[3].type = sIdentificator;
        tokenyTestOcekavane[3].atr.value = "a";
        tokenyTestOcekavane[4].type = sKeyWord;
        tokenyTestOcekavane[4].atr.value = "as";
        tokenyTestOcekavane[5].type = sKeyWord;
        tokenyTestOcekavane[5].atr.value = "integer";
        tokenyTestOcekavane[6].type = sEndOfLine;
        tokenyTestOcekavane[6].atr.value = "EOL";

        tokenyTestOcekavane[7].type = sKeyWord;
        tokenyTestOcekavane[7].atr.value = "dim";
        tokenyTestOcekavane[8].type = sIdentificator;
        tokenyTestOcekavane[8].atr.value = "vysl";
        tokenyTestOcekavane[9].type = sKeyWord;
        tokenyTestOcekavane[9].atr.value = "as";
        tokenyTestOcekavane[10].type = sKeyWord;
        tokenyTestOcekavane[10].atr.value = "integer";
        tokenyTestOcekavane[11].type = sEndOfLine;
        tokenyTestOcekavane[11].atr.value = "EOL";

        tokenyTestOcekavane[12].type = sEndOfLine;
        tokenyTestOcekavane[12].atr.value = "EOL";

        tokenyTestOcekavane[13].type = sKeyWord;
        tokenyTestOcekavane[13].atr.value = "print";
        tokenyTestOcekavane[14].type = sString;
        tokenyTestOcekavane[14].atr.value = "Zadejte\\032cislo\\032pro\\032vypocet\\032faktorialu";
        tokenyTestOcekavane[15].type = sSemicolon;
        tokenyTestOcekavane[15].atr.value = ";";
        tokenyTestOcekavane[16].type = sEndOfLine;
        tokenyTestOcekavane[16].atr.value = "EOL";

        tokenyTestOcekavane[17].type = sKeyWord;
        tokenyTestOcekavane[17].atr.value = "input";
        tokenyTestOcekavane[18].type = sIdentificator;
        tokenyTestOcekavane[18].atr.value = "a";
        tokenyTestOcekavane[19].type = sEndOfLine;
        tokenyTestOcekavane[19].atr.value = "EOL";

        tokenyTestOcekavane[20].type = sKeyWord;
        tokenyTestOcekavane[20].atr.value = "if";
        tokenyTestOcekavane[21].type = sIdentificator;
        tokenyTestOcekavane[21].atr.value = "a";
        tokenyTestOcekavane[22].type = sLess;
        tokenyTestOcekavane[22].atr.value = "<";
        tokenyTestOcekavane[23].type = sInteger;
        tokenyTestOcekavane[23].atr.value = "0";
        tokenyTestOcekavane[24].type = sKeyWord;
        tokenyTestOcekavane[24].atr.value = "then";
        tokenyTestOcekavane[25].type = sEndOfLine;
        tokenyTestOcekavane[25].atr.value = "EOL";

        tokenyTestOcekavane[26].type = sKeyWord;
        tokenyTestOcekavane[26].atr.value = "print";
        tokenyTestOcekavane[27].type = sString;
        tokenyTestOcekavane[27].atr.value = "\\010Faktorial\\032nelze\\032spocitat\\010"; //tady si nejsem jisty tema lomitkama :(
        tokenyTestOcekavane[28].type = sSemicolon;
        tokenyTestOcekavane[28].atr.value = ";";
        tokenyTestOcekavane[29].type = sEndOfLine;
        tokenyTestOcekavane[29].atr.value = "EOL";
        tokenyTestOcekavane[30].type = sEndOfFile;
        tokenyTestOcekavane[30].atr.value = "EOF";

        tToken tokenObdrzeny;
        int i = 0;
        //printf("\n");
        while (i < POCET_TOKENU_TEST_6) {
            tokenObdrzeny = getNextToken();
            if ((strcmp(tokenObdrzeny.atr.value, tokenyTestOcekavane[i].atr.value) != 0) || (tokenObdrzeny.type != tokenyTestOcekavane[i].type)) {
				printf("OBDRZ: %d %s\n", tokenObdrzeny.type, tokenObdrzeny.atr.value);
				printf("OCEKA: %d %s\n\n", tokenyTestOcekavane[i].type, tokenyTestOcekavane[i].atr.value);	
            	testResult = 1;
            }
            i++;
        }

    }

    free(tokenyTestOcekavane);
    return testResult;
}

int main() {
    freopen("test1.txt", "r", stdin);
    printf("Test 1 se vstupem: \"int Integer 42 _\" ");
    test1() == 0 ? printf(GRN "PROSEL \n"RST) : printf((RED"NEPROSEL\n"RST));
    freopen("test2.txt", "r", stdin);
    printf("Test 2 se vstupem: \"+ - * / \\\" ");
    test2() == 0 ? printf(GRN"PROSEL \n"RST) : printf((RED"NEPROSEL\n"RST));
    freopen("test3.txt", "r", stdin);
    printf("Test 3 se vstupem: \"<> > >= = ( <= < ) ;\" ");
    test3() == 0 ? printf(GRN"PROSEL \n"RST) : printf((RED"NEPROSEL\n"RST));
    freopen("test4.txt", "r", stdin);
    printf("Test 4 - Rozpoznani klicovych slov ");
    test4() == 0 ? printf(GRN"PROSEL \n"RST) : printf((RED"NEPROSEL\n"RST));
    freopen("test5.txt", "r", stdin);
    printf("Test 5 se vstupem: \"2e-1 10E2 0.25E5\" ");
    test5() == 0 ? printf(GRN"PROSEL \n"RST) : printf((RED"NEPROSEL\n"RST));
    freopen("test6.txt", "r", stdin);
    printf("Test 6 - kratky kod ");
    test6() == 0 ? printf(GRN"PROSEL \n"RST) : printf((RED"NEPROSEL\n"RST));
    return 0;
}
