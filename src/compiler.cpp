#include "lexer.hpp"
#include "parser.hpp"
#include "codegen.hpp"

#include <getopt.h>

#include <vector>
#include <iostream>

#define PREFIX_UNUSED(variable) ((void)variable)

static int print_lexemas(LEXER*lexer)
{
    TOKEN*tok = lexer->next_token();
    while (tok->get_tag() != DOMAIN_TAG::END_OF_FILE) {
        KEYWORD_TOKEN*keyword_tok;
        DATATYPE_TOKEN*datatype_tok;
        BRACE_TOKEN*brace_tok;
        PAREN_TOKEN*paren_tok;
        SEP_TOKEN*sep_tok;
        IDENT_TOKEN*ident_tok;
        INTEGER_TOKEN*integer_tok;
        EQUALITY_TOKEN*equality_tok;
        LOGICAL_TOKEN*logical_tok;
        RELATIONAL_TOKEN*relational_tok;
        ASSIGN_TOKEN*assign_tok;
        BIN_OP_TOKEN*bin_op_tok;
        COMMENT_TOKEN*comment_tok;
        UNKNOWN_TOKEN*unknown_tok;

        if ((keyword_tok = dynamic_cast<KEYWORD_TOKEN*>(tok)) != nullptr) {
            std::cout << *keyword_tok << std::endl;
        } else if ((datatype_tok = dynamic_cast<DATATYPE_TOKEN*>(tok)) != nullptr) {
            std::cout << *datatype_tok << std::endl;
        } else if ((brace_tok = dynamic_cast<BRACE_TOKEN*>(tok)) != nullptr) {
            std::cout << *brace_tok << std::endl;
        } else if ((paren_tok = dynamic_cast<PAREN_TOKEN*>(tok)) != nullptr) {
            std::cout << *paren_tok << std::endl;
        } else if ((sep_tok = dynamic_cast<SEP_TOKEN*>(tok)) != nullptr) {
            std::cout << *sep_tok << std::endl;
        } else if ((ident_tok = dynamic_cast<IDENT_TOKEN*>(tok)) != nullptr) {
            std::cout << *ident_tok << std::endl;
        } else if ((integer_tok = dynamic_cast<INTEGER_TOKEN*>(tok)) != nullptr) {
            std::cout << *integer_tok << std::endl;
        } else if ((equality_tok = dynamic_cast<EQUALITY_TOKEN*>(tok)) != nullptr) {
            std::cout << *equality_tok << std::endl;
        } else if ((logical_tok = dynamic_cast<LOGICAL_TOKEN*>(tok)) != nullptr) {
            std::cout << *logical_tok << std::endl;
        } else if ((relational_tok = dynamic_cast<RELATIONAL_TOKEN*>(tok)) != nullptr) {
            std::cout << *relational_tok << std::endl;
        } else if ((assign_tok = dynamic_cast<ASSIGN_TOKEN*>(tok)) != nullptr) {
            std::cout << *assign_tok << std::endl;
        } else if ((bin_op_tok = dynamic_cast<BIN_OP_TOKEN*>(tok)) != nullptr) {
            std::cout << *bin_op_tok << std::endl;
        } else if ((comment_tok = dynamic_cast<COMMENT_TOKEN*>(tok)) != nullptr) {
            std::cout << *comment_tok << std::endl;
        }

        if ((unknown_tok = dynamic_cast<UNKNOWN_TOKEN*>(tok)) != nullptr) {
            std::cout << *unknown_tok << std::endl;
        }

        delete tok;
        tok = lexer->next_token();
    }

    delete tok;

    return 0;
}

static void print_usage(char*program_name)
{
    std::cout << "Usage: " << program_name << " [options] file..." << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --help  (-h)                     Print this help info." << std::endl;
    std::cout << "  --print (-p)={lex|parse|codegen} Print lexemas|parsed tree|llvm bitcode." << std::endl;
    exit(0);
}

enum class COMPILER_OPTION
{
    // Compiler params:
    LEX,   // show lexer work;
    PARSE,  // show parser work;
    CODEGEN, // show codegen work;
    ALL,     // build executable;
};

static const char*optarg_lex =     "lex";
static const char*optarg_parse =   "parse";
static const char*optarg_codegen = "codegen";

static COMPILER_OPTION compiler_option = COMPILER_OPTION::ALL;

int parse_args(int argc, char**argv)
{
    struct option opts[] = {
        {"help",  0, 0, 'h'},
        {"print", 1, 0, 'p'},
        {0,0,0,0}
    };

    int c;
    int idx;
    while ((c = getopt_long(argc, argv, "p:h", opts, &idx)) != -1) {
        switch (c) {
        case 'h':
            print_usage(argv[0]);
            break;
        case 'p':
            if (!strcmp(optarg_lex, optarg)) {
                compiler_option = COMPILER_OPTION::LEX;
            } else if (!strcmp(optarg_parse, optarg)) {
                compiler_option = COMPILER_OPTION::PARSE;
            } else if (!strcmp(optarg_codegen, optarg)) {
                compiler_option = COMPILER_OPTION::CODEGEN;
            } else {
                std::cout << "Unknown param for print option: " << optarg << std::endl;
                exit(1);
            }
            break;
        default:
            // Do nothing.
            break;
        }
    }

    return 0;
}

int main(int argc, char**argv)
{
    parse_args(argc, argv);
    LEXER lexer = LEXER(std::string(argv[optind]));
    if (compiler_option == COMPILER_OPTION::LEX) {
        print_lexemas(&lexer);
        return 0;
    }
    lexer.enable_skip_comments();
    PARSER parser = PARSER(&lexer);
    UNIT_AST*unit = parser.get_unit();
    if (compiler_option == COMPILER_OPTION::PARSE) {
        std::cout << *unit << std::endl;
        delete unit;
        return 0;
    }
    CODEGEN codegen = CODEGEN(unit);
    if (compiler_option == COMPILER_OPTION::CODEGEN) {
        std::cout << "heh" << std::endl;
        return 0;
    }
    delete unit;
    
    return 0;
}
