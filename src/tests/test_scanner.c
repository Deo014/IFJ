#include "../scanner.h"
#include "../string.c"
#include "../string.h"
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
        tokenyTestOcekavane[0].type = 1;
        tokenyTestOcekavane[0].atr.value = "int";
        tokenyTestOcekavane[1].type = 3;
        tokenyTestOcekavane[1].atr.value = "integer";
        tokenyTestOcekavane[2].type = 4;
        tokenyTestOcekavane[2].atr.value = "42";
        tokenyTestOcekavane[3].type = 1;
        tokenyTestOcekavane[3].atr.value = "_";
        tokenyTestOcekavane[4].type = 29;
        tokenyTestOcekavane[4].atr.value = "EOL";
        tokenyTestOcekavane[5].type = 30;
        tokenyTestOcekavane[5].atr.value = "EOF";
        tToken tokenObdrzeny;
        int i = 0;

        while (i < POCET_TOKENU_TEST_1) {
            tokenObdrzeny = getNextToken();
/*
            printf("%s\n",tokenObdrzeny.atr.value);
            printf("%s\n",tokenyTestOcekavane[i].atr.value);
            printf("%d\n",tokenObdrzeny.type);
            printf("%d\n",tokenyTestOcekavane[i].type);
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
        tokenyTestOcekavane[0].type = 12;
        tokenyTestOcekavane[0].atr.value = "+";
        tokenyTestOcekavane[1].type = 13;
        tokenyTestOcekavane[1].atr.value = "-";
        tokenyTestOcekavane[2].type = 16;
        tokenyTestOcekavane[2].atr.value = "*";
        tokenyTestOcekavane[3].type = 14;
        tokenyTestOcekavane[3].atr.value = "/";
        tokenyTestOcekavane[4].type = 17;
        tokenyTestOcekavane[4].atr.value = "\\";
        tokenyTestOcekavane[5].type = 29;
        tokenyTestOcekavane[5].atr.value = "EOL";
        tokenyTestOcekavane[6].type = 30;
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
        tokenyTestOcekavane[0].type = 19;
        tokenyTestOcekavane[0].atr.value = "<>";
        tokenyTestOcekavane[1].type = 21;
        tokenyTestOcekavane[1].atr.value = ">";
        tokenyTestOcekavane[2].type = 22;
        tokenyTestOcekavane[2].atr.value = ">=";
        tokenyTestOcekavane[3].type = 23;
        tokenyTestOcekavane[3].atr.value = "=";
        tokenyTestOcekavane[4].type = 24;
        tokenyTestOcekavane[4].atr.value = "(";
        tokenyTestOcekavane[5].type = 20;
        tokenyTestOcekavane[5].atr.value = "<=";
        tokenyTestOcekavane[6].type = 18;
        tokenyTestOcekavane[6].atr.value = "<";
        tokenyTestOcekavane[7].type = 25;
        tokenyTestOcekavane[7].atr.value = ")";
        tokenyTestOcekavane[8].type = 26;
        tokenyTestOcekavane[8].atr.value = ";";
        tokenyTestOcekavane[9].type = 29;
        tokenyTestOcekavane[9].atr.value = "EOL";
        tokenyTestOcekavane[10].type = 30;
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
                tokenyTestOcekavane[j].type = 1;
            else if (j == 36)
                tokenyTestOcekavane[j].type = 29;
            else if (j == 37)
                tokenyTestOcekavane[j].type = 30;
            else
                tokenyTestOcekavane[j].type = 3;

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
        tokenyTestOcekavane[0].type = 5;
        tokenyTestOcekavane[0].atr.value = "2e-1";
        tokenyTestOcekavane[1].type = 5;
        tokenyTestOcekavane[1].atr.value = "10E2";
        tokenyTestOcekavane[2].type = 5;
        tokenyTestOcekavane[2].atr.value = "0.25E5";
        tokenyTestOcekavane[3].type = 29;
        tokenyTestOcekavane[3].atr.value = "EOL";
        tokenyTestOcekavane[4].type = 30;
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
        tokenyTestOcekavane[0].type = 3;
        tokenyTestOcekavane[0].atr.value = "scope";
        tokenyTestOcekavane[1].type = 29;
        tokenyTestOcekavane[1].atr.value = "EOL";
        tokenyTestOcekavane[2].type = 3;
        tokenyTestOcekavane[2].atr.value = "Dim";
        tokenyTestOcekavane[3].type = 1;
        tokenyTestOcekavane[3].atr.value = "a";
        tokenyTestOcekavane[4].type = 3;
        tokenyTestOcekavane[4].atr.value = "as";
        tokenyTestOcekavane[5].type = 3;
        tokenyTestOcekavane[5].atr.value = "integer";
        tokenyTestOcekavane[6].type = 29;
        tokenyTestOcekavane[6].atr.value = "EOL";

        tokenyTestOcekavane[7].type = 3;
        tokenyTestOcekavane[7].atr.value = "dim";
        tokenyTestOcekavane[8].type = 1;
        tokenyTestOcekavane[8].atr.value = "vysl";
        tokenyTestOcekavane[9].type = 3;
        tokenyTestOcekavane[9].atr.value = "as";
        tokenyTestOcekavane[10].type = 3;
        tokenyTestOcekavane[10].atr.value = "integer";
        tokenyTestOcekavane[11].type = 29;
        tokenyTestOcekavane[11].atr.value = "EOL";

        tokenyTestOcekavane[12].type = 29;
        tokenyTestOcekavane[12].atr.value = "EOL";

        tokenyTestOcekavane[13].type = 3;
        tokenyTestOcekavane[13].atr.value = "print";
        tokenyTestOcekavane[14].type = 10;
        tokenyTestOcekavane[14].atr.value = "Zadejte cislo pro vypocet faktorialu";
        tokenyTestOcekavane[15].type = 26;
        tokenyTestOcekavane[15].atr.value = ";";
        tokenyTestOcekavane[16].type = 29;
        tokenyTestOcekavane[16].atr.value = "EOL";

        tokenyTestOcekavane[17].type = 3;
        tokenyTestOcekavane[17].atr.value = "input";
        tokenyTestOcekavane[18].type = 1;
        tokenyTestOcekavane[18].atr.value = "a";
        tokenyTestOcekavane[19].type = 29;
        tokenyTestOcekavane[19].atr.value = "EOL";

        tokenyTestOcekavane[20].type = 3;
        tokenyTestOcekavane[20].atr.value = "if";
        tokenyTestOcekavane[21].type = 1;
        tokenyTestOcekavane[21].atr.value = "a";
        tokenyTestOcekavane[22].type = 18;
        tokenyTestOcekavane[22].atr.value = "<";
        tokenyTestOcekavane[23].type = 4;
        tokenyTestOcekavane[23].atr.value = "0";
        tokenyTestOcekavane[24].type = 3;
        tokenyTestOcekavane[24].atr.value = "THEN";
        tokenyTestOcekavane[25].type = 26;
        tokenyTestOcekavane[25].atr.value = "EOL";

        tokenyTestOcekavane[26].type = 3;
        tokenyTestOcekavane[26].atr.value = "print";
        tokenyTestOcekavane[27].type = 10;
        tokenyTestOcekavane[27].atr.value = "\nFaktorial nelze spocitat\n"; //tady si nejsem jisty tema lomitkama :(
        tokenyTestOcekavane[28].type = 26;
        tokenyTestOcekavane[28].atr.value = ";";
        tokenyTestOcekavane[29].type = 29;
        tokenyTestOcekavane[29].atr.value = "EOL";
        tokenyTestOcekavane[30].type = 30;
        tokenyTestOcekavane[30].atr.value = "EOF";

        tToken tokenObdrzeny;
        int i = 0;
        printf("\n");
        while (i < POCET_TOKENU_TEST_6) {
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
