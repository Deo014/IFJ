PROJ=scanner_test
FILES=scanner_tests.c ../../src/scanner.c ../../src/string.c
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread
CC=gcc
RM=rm -f

all : $(PROJ)
	./$(PROJ)

$(PROJ) : $(FILES)
		$(CC) $(CFLAGS) -o $(PROJ) $(FILES) -lm -lcmocka

clean :
	$(RM) *.o $(PROJ)

#gcc scanner_tests.c -lcmocka