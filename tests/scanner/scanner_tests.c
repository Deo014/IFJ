#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

#include "../../src/scanner.h" // UUT
#include "../../src/string.h"

static void test1(void** state) {
    (void) state; // unused
    freopen("test1.txt", "r", stdin);

    tToken token;
    token = getNextToken();
    assert_int_equal(token.type, sEndOfFile);
    assert_string_equal(token.atr.value, "EOF");
}

static void test2(void** state) {
    (void)state; // unused
    tToken token;
    freopen("test2.txt", "r", stdin);

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

static void test3(void** state) {
    (void) state; // unused
    freopen("test3.txt", "r", stdin);

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

int main(void)
{
    const struct CMUnitTest tests[] =
            {
                    cmocka_unit_test(test1),
                    cmocka_unit_test(test2),
                    cmocka_unit_test(test3),
            };

    return cmocka_run_group_tests(tests, NULL, NULL);
}