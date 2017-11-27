cmake_minimum_required(VERSION 3.7)
project(IFJ)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "$(CMAKE_C_FLAGS) -std=gnu99 -Wall -Wextra -Werror -pedantic -pthread")

set(CMAKE_C_FLAGS "-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread")

set(SOURCE_FILES src/parser.h src/parser.c src/expression.h src/expression.c src/stack.c src/stack.h src/instlist.c src/instlist.h src/main.c src/scanner.h src/scanner.c src/symtable.c src/symtable.h src/error_code.h src/string.h src/string.c)

add_executable(IFJ ${SOURCE_FILES})