# IFJ - Formální jazyky a překladače

## Implementace překladače imperativního jazyka IFJ17

## Zadání (vice viz zadani.pdf)
Vytvořte **program v jazyce C, který načte zdrojový kód zapsaný ve zdrojovém jazyce IFJ17
a přeloží jej do cílového jazyka IFJcode17 (mezikód)**.

Jestliže proběhne překlad bez chyb, vrací se návratová hodnota 0 (nula). Jestliže došlo k nějaké chybě, vrací se návratová
hodnota následovně:
* 1 - chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému).
* 2 - chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu).
* 3 - sémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefinici funkce/proměnné, atd.
* 4 - sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech, příp. špatný počet či typ parametrů u volání funkce.
* 6 - ostatní sémantické chyby.
* 99 - interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti, atd.).

**Překladač bude načítat řídicí program v jazyce IFJ17 ze standardního vstupu a generovat výsledný mezikód v jazyce IFJcode17** (viz kapitola 10) na standardní výstup.
Všechna chybová hlášení, varování a ladicí výpisy provádějte na standardní chybový výstup; tj. bude se jednat o konzolovou aplikaci (tzv. filtr) bez grafického uživatelského rozhraní.

Pro interpretaci výsledného programu v cílovém jazyce IFJcode17 bude na stránkách
předmětu k dispozici interpret.

Klíčová slova jsou sázena tučně a některé lexémy jsou pro zvýšení čitelnosti v apo-
strofech, přičemž znak apostrofu není v takovém případě součástí jazyka!