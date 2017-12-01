#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

/* testovane moduly */
#include "../scanner.h"
#include "../expression.h"
#include "../symtable.h"

/*--------------------------------------------------SCANNER-----------------------------------------------------------*/
static void scanner_prazdny_vstup(void** state) {
    (void) state; // unused
    freopen("prazdny_vstup.txt", "r", stdin);

    tToken token;
    token = getNextToken();
    assert_int_equal(token.type, sEndOfFile);
    assert_string_equal(token.atr.value, "EOF");
}

static void scanner_vypocet_faktorialu_nerekurzivne(void** state) {
    (void)state; // unused
    tToken token;
    freopen("vypocet_faktorialu_nerekurzivne.txt", "r", stdin);

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "scope");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "dim");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "a");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "as");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "integer");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "dim");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "vysl");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "as");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "integer");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "print");

    token = getNextToken();
    assert_int_equal(token.type, sString);
    assert_string_equal(token.atr.value, "Zadejte\\032cislo\\032pro\\032vypocet\\032faktorialu");

    token = getNextToken();
    assert_int_equal(token.type, sSemicolon);
    assert_string_equal(token.atr.value, ";");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "input");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "a");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "if");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "a");

    token = getNextToken();
    assert_int_equal(token.type, sLess);
    assert_string_equal(token.atr.value, "<");

    token = getNextToken();
    assert_int_equal(token.type, sInteger);
    assert_string_equal(token.atr.value, "0");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "then");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "print");

    token = getNextToken();
    assert_int_equal(token.type, sString);
    assert_string_equal(token.atr.value, "\\010Faktorial\\032nelze\\032spocitat\\010");

    token = getNextToken();
    assert_int_equal(token.type, sSemicolon);
    assert_string_equal(token.atr.value, ";");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "else");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "vysl");

    token = getNextToken();
    assert_int_equal(token.type, sAssignment);
    assert_string_equal(token.atr.value, "=");

    token = getNextToken();
    assert_int_equal(token.type, sInteger);
    assert_string_equal(token.atr.value, "1");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "do");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "while");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "a");

    token = getNextToken();
    assert_int_equal(token.type, sMore);
    assert_string_equal(token.atr.value, ">");

    token = getNextToken();
    assert_int_equal(token.type, sInteger);
    assert_string_equal(token.atr.value, "0");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "vysl");

    token = getNextToken();
    assert_int_equal(token.type, sAssignment);
    assert_string_equal(token.atr.value, "=");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "vysl");

    token = getNextToken();
    assert_int_equal(token.type, sMultiply);
    assert_string_equal(token.atr.value, "*");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "a");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "a");

    token = getNextToken();
    assert_int_equal(token.type, sAssignment);
    assert_string_equal(token.atr.value, "=");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "a");

    token = getNextToken();
    assert_int_equal(token.type, sMinus);
    assert_string_equal(token.atr.value, "-");

    token = getNextToken();
    assert_int_equal(token.type, sInteger);
    assert_string_equal(token.atr.value, "1");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "loop");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "print");

    token = getNextToken();
    assert_int_equal(token.type, sString);
    assert_string_equal(token.atr.value, "\\010Vysledek\\032je:");

    token = getNextToken();
    assert_int_equal(token.type, sSemicolon);
    assert_string_equal(token.atr.value, ";");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "vysl");

    token = getNextToken();
    assert_int_equal(token.type, sSemicolon);
    assert_string_equal(token.atr.value, ";");

    token = getNextToken();
    assert_int_equal(token.type, sString);
    assert_string_equal(token.atr.value, "\\010");

    token = getNextToken();
    assert_int_equal(token.type, sSemicolon);
    assert_string_equal(token.atr.value, ";");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "end");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "if");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "end");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "scope");

}

static void scanner_vypocet_faktorialu_rekurzivne(void** state) {
    (void) state; // unused
    freopen("vypocet_faktorialu_rekurzivne.txt", "r", stdin);

    tToken token;

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");


    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "declare");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "function");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "factorial");

    token = getNextToken();
    assert_int_equal(token.type, sLeftPar);
    assert_string_equal(token.atr.value, "(");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "n");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "as");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "integer");

    token = getNextToken();
    assert_int_equal(token.type, sRightPar);
    assert_string_equal(token.atr.value, ")");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "as");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "integer");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "function");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "factorial");

    token = getNextToken();
    assert_int_equal(token.type, sLeftPar);
    assert_string_equal(token.atr.value, "(");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "n");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "as");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "integer");

    token = getNextToken();
    assert_int_equal(token.type, sRightPar);
    assert_string_equal(token.atr.value, ")");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "as");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "integer");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "dim");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "temp_result");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "as");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "integer");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "dim");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "decremented_n");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "as");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "integer");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "dim");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "result");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "as");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "integer");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "if");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "n");

    token = getNextToken();
    assert_int_equal(token.type, sLess);
    assert_string_equal(token.atr.value, "<");

    token = getNextToken();
    assert_int_equal(token.type, sInteger);
    assert_string_equal(token.atr.value, "2");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "then");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "result");

    token = getNextToken();
    assert_int_equal(token.type, sAssignment);
    assert_string_equal(token.atr.value, "=");

    token = getNextToken();
    assert_int_equal(token.type, sInteger);
    assert_string_equal(token.atr.value, "1");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "else");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "decremented_n");

    token = getNextToken();
    assert_int_equal(token.type, sAssignment);
    assert_string_equal(token.atr.value, "=");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "n");

    token = getNextToken();
    assert_int_equal(token.type, sMinus);
    assert_string_equal(token.atr.value, "-");

    token = getNextToken();
    assert_int_equal(token.type, sInteger);
    assert_string_equal(token.atr.value, "1");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "temp_result");

    token = getNextToken();
    assert_int_equal(token.type, sAssignment);
    assert_string_equal(token.atr.value, "=");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "factorial");

    token = getNextToken();
    assert_int_equal(token.type, sLeftPar);
    assert_string_equal(token.atr.value, "(");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "decremented_n");

    token = getNextToken();
    assert_int_equal(token.type, sRightPar);
    assert_string_equal(token.atr.value, ")");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "result");

    token = getNextToken();
    assert_int_equal(token.type, sAssignment);
    assert_string_equal(token.atr.value, "=");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "n");

    token = getNextToken();
    assert_int_equal(token.type, sMultiply);
    assert_string_equal(token.atr.value, "*");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "temp_result");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "end");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "if");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "return");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "result");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "end");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "function");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "scope");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "dim");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "a");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "as");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "integer");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "dim");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "vysl");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "as");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "integer");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "print");

    token = getNextToken();
    assert_int_equal(token.type, sString);
    assert_string_equal(token.atr.value, "Zadejte\\032cislo\\032pro\\032vypocet\\032faktorialu");

    token = getNextToken();
    assert_int_equal(token.type, sSemicolon);
    assert_string_equal(token.atr.value, ";");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "input");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "a");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "if");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "a");

    token = getNextToken();
    assert_int_equal(token.type, sLess);
    assert_string_equal(token.atr.value, "<");

    token = getNextToken();
    assert_int_equal(token.type, sInteger);
    assert_string_equal(token.atr.value, "0");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "then");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "print");

    token = getNextToken();
    assert_int_equal(token.type, sString);
    assert_string_equal(token.atr.value, "\\010Faktorial\\032nelze\\032spocitat\\010");

    token = getNextToken();
    assert_int_equal(token.type, sSemicolon);
    assert_string_equal(token.atr.value, ";");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "else");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "vysl");

    token = getNextToken();
    assert_int_equal(token.type, sAssignment);
    assert_string_equal(token.atr.value, "=");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "factorial");

    token = getNextToken();
    assert_int_equal(token.type, sLeftPar);
    assert_string_equal(token.atr.value, "(");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "a");

    token = getNextToken();
    assert_int_equal(token.type, sRightPar);
    assert_string_equal(token.atr.value, ")");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "print");

    token = getNextToken();
    assert_int_equal(token.type, sString);
    assert_string_equal(token.atr.value, "\\010Vysledek\\032je:");

    token = getNextToken();
    assert_int_equal(token.type, sSemicolon);
    assert_string_equal(token.atr.value, ";");

    token = getNextToken();
    assert_int_equal(token.type, sIdentificator);
    assert_string_equal(token.atr.value, "vysl");

    token = getNextToken();
    assert_int_equal(token.type, sSemicolon);
    assert_string_equal(token.atr.value, ";");

    token = getNextToken();
    assert_int_equal(token.type, sString);
    assert_string_equal(token.atr.value, "\\010");

    token = getNextToken();
    assert_int_equal(token.type, sSemicolon);
    assert_string_equal(token.atr.value, ";");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "end");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "if");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    /* next line */

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "end");

    token = getNextToken();
    assert_int_equal(token.type, sKeyWord);
    assert_string_equal(token.atr.value, "scope");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfLine);
    assert_string_equal(token.atr.value, "EOL");

    token = getNextToken();
    assert_int_equal(token.type, sEndOfFile);
    assert_string_equal(token.atr.value, "EOF");


}

/*--------------------------------------------------PARSER------------------------------------------------------------*/
static void parser_prazdny_vstup( void** state) {
    (void) state; // unused
    freopen("prazdny_vstup.txt", "r", stdin);

}

static void parser_vypocet_faktorialu_nerekurzivne( void** state) {
    (void) state; // unused
    freopen("vypocet_faktorialu_nerekurzivne.txt", "r", stdin);

}

static void parser_faktorialu_rekurzivne( void** state) {
    (void) state; // unused
    freopen("vypocet_faktorialu_rekurzivne.txt", "r", stdin);

}

/*--------------------------------------------------EXPRESSION--------------------------------------------------------*/

static void expression_test1( void** state) {
    (void) state; // unused
    freopen("expression_test1.txt", "r", stdin);
    assert_int_equal(expression(getNextToken(), sInteger), ERROR_CODE_OK);
}
/*
static void expression_vypocet_faktorialu_nerekurzivne( void** state) {
    (void) state; // unused
    freopen("vypocet_faktorialu_nerekurzivne.txt", "r", stdin);

}

static void expression_faktorialu_rekurzivne( void** state) {
    (void) state; // unused
    freopen("vypocet_faktorialu_rekurzivne.txt", "r", stdin);

}
*/

int main(void)
{
    printf("--------------------SCANNER--------------------\n");
    const struct CMUnitTest scanner_tests[] =
            {
                    cmocka_unit_test(scanner_prazdny_vstup),
                    cmocka_unit_test(scanner_vypocet_faktorialu_nerekurzivne),
                    cmocka_unit_test(scanner_vypocet_faktorialu_rekurzivne),
            };
    cmocka_run_group_tests(scanner_tests, NULL, NULL);
    printf("--------------------PARSER---------------------\n");
    const struct CMUnitTest parser_tests[] =
            {
                    cmocka_unit_test(parser_prazdny_vstup),
                    cmocka_unit_test(parser_vypocet_faktorialu_nerekurzivne),
                    cmocka_unit_test(parser_faktorialu_rekurzivne),
            };
    cmocka_run_group_tests(parser_tests, NULL, NULL);
    printf("--------------------EXPRESSION---------------------\n");
    const struct CMUnitTest expression_tests[] =
            {
                    cmocka_unit_test(expression_test1),
            };
    cmocka_run_group_tests(expression_tests, NULL, NULL);

}