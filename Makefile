SRC_PREFIX=src/
BIN_PREFIX=bin/
LIBS_PREFIX=.libs/
OBJS_PREFIX=.objs/

CC=g++

CFLAGS=-g -Wall -Wextra -std=c++14 -O2

LLVMFLAGS=`llvm-config-5.0 --cxxflags --ldflags --system-libs --libs core`

all: $(BIN_PREFIX)compiler

LEXER_SRC_PREFIX=$(SRC_PREFIX)lexer/
LEXER_SRC=$(shell find $(LEXER_SRC_PREFIX) -maxdepth 1 -name '*.cpp')
LEXER_OBJS_PREFIX=$(OBJS_PREFIX)lexer/
LEXER_OBJS=$(patsubst $(LEXER_SRC_PREFIX)%.cpp,$(LEXER_OBJS_PREFIX)%.o,$(LEXER_SRC))
LEXER_LIB_PREFIX=$(LIBS_PREFIX)lexer/
LEXER_LIB=$(LEXER_LIB_PREFIX)lexer.a

$(LEXER_LIB): $(LEXER_OBJS)
	mkdir -p $(LEXER_LIB_PREFIX)
	ar rcs $@ $^

$(LEXER_OBJS_PREFIX)%.o: $(LEXER_SRC_PREFIX)%.cpp $(LEXER_SRC_PREFIX)%.hpp
	mkdir -p $(LEXER_OBJS_PREFIX)
	$(CC) $(CFLAGS) -c $< -o $@

PARSER_SRC_PREFIX=$(SRC_PREFIX)parser/
PARSER_SRC=$(shell find $(PARSER_SRC_PREFIX) -maxdepth 1 -name '*.cpp')
PARSER_OBJS_PREFIX=$(OBJS_PREFIX)parser/
PARSER_OBJS=$(patsubst $(PARSER_SRC_PREFIX)%.cpp,$(PARSER_OBJS_PREFIX)%.o,$(PARSER_SRC))
PARSER_LIB_PREFIX=$(LIBS_PREFIX)parser/
PARSER_LIB=$(PARSER_LIB_PREFIX)parser.a

$(PARSER_LIB): $(PARSER_OBJS)
	mkdir -p $(PARSER_LIB_PREFIX)
	ar rcs $@ $^

$(PARSER_OBJS_PREFIX)%.o: $(PARSER_SRC_PREFIX)%.cpp $(PARSER_SRC_PREFIX)%.hpp
	mkdir -p $(PARSER_OBJS_PREFIX)
	$(CC) $(CFLAGS) -I$(LEXER_SRC_PREFIX) -c $< -o $@

CODEGEN_SRC_PREFIX=$(SRC_PREFIX)codegen/
CODEGEN_SRC=$(shell find $(CODEGEN_SRC_PREFIX) -maxdepth 1 -name '*.cpp')
CODEGEN_OBJS_PREFIX=$(OBJS_PREFIX)codegen/
CODEGEN_OBJS=$(patsubst $(CODEGEN_SRC_PREFIX)%.cpp,$(CODEGEN_OBJS_PREFIX)%.o,$(CODEGEN_SRC))
CODEGEN_LIB_PREFIX=$(LIBS_PREFIX)codegen/
CODEGEN_LIB=$(CODEGEN_LIB_PREFIX)codegen.a

$(CODEGEN_LIB): $(CODEGEN_OBJS)
	mkdir -p $(CODEGEN_LIB_PREFIX)
	ar rcs $@ $^

$(CODEGEN_OBJS_PREFIX)%.o: $(CODEGEN_SRC_PREFIX)%.cpp $(CODEGEN_SRC_PREFIX)%.hpp
	mkdir -p $(CODEGEN_OBJS_PREFIX)
	$(CC) $(CFLAGS) $(LLVMFLAGS) -I$(LEXER_SRC_PREFIX) -I$(PARSER_SRC_PREFIX) -c $< -o $@

IRBUILDER_SRC_PREFIX=$(SRC_PREFIX)irbuilder/
IRBUILDER_SRC=$(shell find $(IRBUILDER_SRC_PREFIX) -maxdepth 1 -name '*.cpp')
IRBUILDER_OBJS_PREFIX=$(OBJS_PREFIX)irbuilder/
IRBUILDER_OBJS=$(patsubst $(IRBUILDER_SRC_PREFIX)%.cpp,$(IRBUILDER_OBJS_PREFIX)%.o,$(IRBUILDER_SRC))
IRBUILDER_LIB_PREFIX=$(LIBS_PREFIX)irbuilder/
IRBUILDER_LIB=$(IRBUILDER_LIB_PREFIX)irbuilder.a

$(IRBUILDER_LIB): $(IRBUILDER_OBJS)
	mkdir -p $(IRBUILDER_LIB_PREFIX)
	ar rcs $@ $^

$(IRBUILDER_OBJS_PREFIX)%.o: $(IRBUILDER_SRC_PREFIX)%.cpp $(IRBUILDER_SRC_PREFIX)%.hpp
	mkdir -p $(IRBUILDER_OBJS_PREFIX)
	$(CC) $(CFLAGS) $(LLVMFLAGS) -I$(LEXER_SRC_PREFIX) -I$(PARSER_SRC_PREFIX) -c $< -o $@

COMPILER_SRC_PREFIX=$(SRC_PREFIX)
COMPILER_SRC=$(shell find $(COMPILER_SRC_PREFIX) -maxdepth 1 -name '*.cpp')
COMPILER_OBJS_PREFIX=$(OBJS_PREFIX)
COMPILER_OBJS=$(patsubst $(COMPILER_SRC_PREFIX)%.cpp,$(COMPILER_OBJS_PREFIX)%.o,$(COMPILER_SRC))
COMPILER_INCLUDES=-I$(LEXER_SRC_PREFIX) -I$(PARSER_SRC_PREFIX) -I$(IRBUILDER_SRC_PREFIX) -I$(CODEGEN_SRC_PREFIX)

$(COMPILER_OBJS_PREFIX)%.o: $(COMPILER_SRC_PREFIX)%.cpp
	mkdir -p $(COMPILER_OBJS_PREFIX)
	$(CC) $(CFLAGS) $(LLVMFLAGS) $(COMPILER_INCLUDES) -c $< -o $@

$(BIN_PREFIX)compiler: $(COMPILER_OBJS) $(LEXER_LIB) $(PARSER_LIB) $(CODEGEN_LIB) $(IRBUILDER_LIB)
	mkdir -p $(BIN_PREFIX)
	$(CC) $(CFLAGS) $(LLVMFLAGS) $^ -o $@

TEST_SRC_PREFIX=$(SRC_PREFIX)test/
TEST_SRC=$(shell find $(TEST_SRC_PREFIX) -maxdepth 1 -name '*.c')
test: $(TEST_SRC)
	mkdir -p ./debug
	./bin/compiler -v > ./debug/version.txt
	./bin/compiler -h > ./debug/help.txt
	./bin/compiler -p lex     -i ./src/test/test.c -o ./debug/lex.txt
	./bin/compiler -p parse   -i ./src/test/test.c -o ./debug/parse.txt
	./bin/compiler -p ir      -i ./src/test/test.c -o ./debug/ir.txt
	./bin/compiler -p ir      -i ./src/test/test.c -o ./debug/ir.dot -g
	./bin/compiler -p ssa     -i ./src/test/test.c -o ./debug/ssa.txt
	./bin/compiler -p ssa     -i ./src/test/test.c -o ./debug/ssa.dot -g
	./bin/compiler -p codegen -i ./src/test/test.c -o ./debug/codegen.txt >> ./debug/codegen.txt
	./bin/compiler -i ./src/test/test.c -o ./debug/object.o
	gcc -g -Wall -Wextra ./src/test/extern.c ./debug/object.o -o ./debug/test
	./debug/test > ./debug/test.txt

.PHONY: clean

clean:
	rm -rf $(BIN_PREFIX) $(OBJS_PREFIX) $(LIBS_PREFIX) ./debug

