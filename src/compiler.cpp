#include "lexer.hpp"
#include "parser.hpp"
#include "irbuilder.hpp"
#include "codegen.hpp"

#include <getopt.h>

#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>

#define PREFIX_UNUSED(variable) ((void)variable)

static int print_lexemas(LEXER*lexer, std::string fname)
{
    std::ofstream lex_out;
    lex_out.open(fname);
    
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
            lex_out << *keyword_tok << std::endl;
        } else if ((datatype_tok = dynamic_cast<DATATYPE_TOKEN*>(tok)) != nullptr) {
            lex_out << *datatype_tok << std::endl;
        } else if ((brace_tok = dynamic_cast<BRACE_TOKEN*>(tok)) != nullptr) {
            lex_out << *brace_tok << std::endl;
        } else if ((paren_tok = dynamic_cast<PAREN_TOKEN*>(tok)) != nullptr) {
            lex_out << *paren_tok << std::endl;
        } else if ((sep_tok = dynamic_cast<SEP_TOKEN*>(tok)) != nullptr) {
            lex_out << *sep_tok << std::endl;
        } else if ((ident_tok = dynamic_cast<IDENT_TOKEN*>(tok)) != nullptr) {
            lex_out << *ident_tok << std::endl;
        } else if ((integer_tok = dynamic_cast<INTEGER_TOKEN*>(tok)) != nullptr) {
            lex_out << *integer_tok << std::endl;
        } else if ((equality_tok = dynamic_cast<EQUALITY_TOKEN*>(tok)) != nullptr) {
            lex_out << *equality_tok << std::endl;
        } else if ((logical_tok = dynamic_cast<LOGICAL_TOKEN*>(tok)) != nullptr) {
            lex_out << *logical_tok << std::endl;
        } else if ((relational_tok = dynamic_cast<RELATIONAL_TOKEN*>(tok)) != nullptr) {
            lex_out << *relational_tok << std::endl;
        } else if ((assign_tok = dynamic_cast<ASSIGN_TOKEN*>(tok)) != nullptr) {
            lex_out << *assign_tok << std::endl;
        } else if ((bin_op_tok = dynamic_cast<BIN_OP_TOKEN*>(tok)) != nullptr) {
            lex_out << *bin_op_tok << std::endl;
        } else if ((comment_tok = dynamic_cast<COMMENT_TOKEN*>(tok)) != nullptr) {
            lex_out << *comment_tok << std::endl;
        }

        if ((unknown_tok = dynamic_cast<UNKNOWN_TOKEN*>(tok)) != nullptr) {
            lex_out << *unknown_tok << std::endl;
        }

        delete tok;
        tok = lexer->next_token();
    }

    delete tok;
    lex_out.close();

    return 0;
}

static void print_version(char*program_name)
{
    std::cout << program_name << std::endl;
    std::cout << "CSC V0.1" << std::endl;
    std::cout << "CSC is a C Subset Compiler." << std::endl;
    std::cout << "It supports integer types, if-else-stmts and while loops." << std::endl;
    std::cout << "Functionality:" << std::endl;
    std::cout << "  - lexer;" << std::endl;
    std::cout << "  - parser;" << std::endl;
    std::cout << "  - ir builder;" << std::endl;
    std::cout << "  - ssa builder;" << std::endl;
    std::cout << "  - llvm bitcode generator;" << std::endl;
    std::cout << "  - objects builder;" << std::endl;
    exit(0);
}

static void print_help(char*program_name)
{
    std::cout << "Usage: " << program_name << " [options] file..." << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --version (-v)                            Print version info." << std::endl;
    std::cout << "  --help    (-h)                            Print this help info." << std::endl;
    std::cout << "  --input   (-i)=<file>                     Place the input into <file>." << std::endl;
    std::cout << "  --output  (-o)=<file>                     Place the output into <file>." << std::endl;
    std::cout << "  --print   (-p)={lex|parse|ir|ssa|codegen} Output lexemas|AST|IR|SSA IR|llvm bitcode." << std::endl;
    std::cout << "  --graph   (-g)=<file>                     Place the output as .dot file if possible." << std::endl;
    exit(0);
}

enum class COMPILER_OPTION
{
    // Compiler params:
    LEX,     // show lexer work;
    PARSE,   // show parser work;
    IR,      // show IR work;
    SSA,     // show SSA work;
    CODEGEN, // show codegen work;
    ALL,     // build executable;
};

static const char*optarg_lex =     "lex";
static const char*optarg_parse =   "parse";
static const char*optarg_ir =      "ir";
static const char*optarg_ssa =     "ssa";
static const char*optarg_codegen = "codegen";

static std::string lex_name =     "lex.txt";
static std::string parse_name =   "parse.txt";
static std::string ir_name =      "ir.txt";
static std::string dot_ir_name =  "ir.dot";
static std::string ssa_name =     "ssa.txt";
static std::string dot_ssa_name = "ssa.dot";
static std::string codegen_name = "codegen.txt";
static std::string all_name =     "object.o";

static std::string input  =              "";
static std::string output =              "";
static bool use_default_output =         true;
static COMPILER_OPTION compiler_option = COMPILER_OPTION::ALL;
static bool is_graph =                   false;

int parse_args(int argc, char**argv)
{
    struct option opts[] = {
        {"version", 0, 0, 'v'},
        {"help",    0, 0, 'h'},
        {"input",   1, 0, 'i'},
        {"output",  1, 0, 'o'},
        {"print",   1, 0, 'p'},
        {"graph",   0, 0, 'g'},
        {0,0,0,0}
    };

    int c;
    int idx;
    while ((c = getopt_long(argc, argv, "i:o:p:gvh", opts, &idx)) != -1) {
        switch (c) {
        case 'v':
            print_version(argv[0]);
            break;
        case 'h':
            print_help(argv[0]);
            break;
        case 'i':
            input = std::string(optarg);
            break;
        case 'o':
            use_default_output = false;
            output = std::string(optarg);
            break;
        case 'p':
            if (!strcmp(optarg_lex, optarg)) {
                compiler_option = COMPILER_OPTION::LEX;
            } else if (!strcmp(optarg_parse, optarg)) {
                compiler_option = COMPILER_OPTION::PARSE;
            } else if (!strcmp(optarg_ir, optarg)) {
                compiler_option = COMPILER_OPTION::IR;
            }else if (!strcmp(optarg_ssa, optarg)) {
                compiler_option = COMPILER_OPTION::SSA;
            } else if (!strcmp(optarg_codegen, optarg)) {
                compiler_option = COMPILER_OPTION::CODEGEN;
            } else {
                std::cerr << "Unknown param for print option: " << optarg << std::endl;
                exit(1);
            }
            break;
        case 'g':
            is_graph = true;
            break;
        default:
            // Do nothing.
            break;
        }
    }

    if (input == "") {
        std::cerr << "Empty input params." << std::endl;
        exit(1);
    }

    switch (compiler_option) {
    case COMPILER_OPTION::LEX:
        is_graph = false;
        if (output == "") {
            output = lex_name;
        }
        break;
    case COMPILER_OPTION::PARSE:
        is_graph = false;
        if (output == "") {
            output = parse_name;
        }
        break;
    case COMPILER_OPTION::IR:
        if (output == "") {
            if (!is_graph) {
                output = ir_name;
            } else {
                output = dot_ir_name;
            }
        }
        break;
    case COMPILER_OPTION::SSA:
        if (output == "") {
            if (!is_graph) {
                output = ssa_name;
            } else {
                output = dot_ssa_name;
            }
        }
        break;
    case COMPILER_OPTION::CODEGEN:
        is_graph = false;
        if (output == "") {
            output = codegen_name;
        }
        break;
    case COMPILER_OPTION::ALL:
        is_graph = false;
        if (output == "") {
            output = all_name;
        }
        break;
    }

    return 0;
}

int main(int argc, char**argv)
{
    parse_args(argc, argv);
    
    LEXER lexer = LEXER(input);
    if (compiler_option == COMPILER_OPTION::LEX) {
        print_lexemas(&lexer, output);
        return 0;
    }
    lexer.enable_skip_comments();
    PARSER parser = PARSER(&lexer);
    UNIT_AST*unit = parser.get_unit();
    if (compiler_option == COMPILER_OPTION::PARSE) {
        std::ofstream parse_out;
        parse_out.open(output);
        if (unit != nullptr) {
            parse_out << *unit << std::endl;
            delete unit;
        } else {
            parse_out << "empty unit" << std::endl;
        }
        parse_out.close();
        return 0;
    }
    if ((compiler_option == COMPILER_OPTION::IR) || (compiler_option == COMPILER_OPTION::SSA)) {
        remove(output.c_str()); 
        auto funcs = unit->get_funcs();
        for (std::size_t i = 0; i < funcs.size(); i++) {
            auto ssa_form = IR_BUILDER(funcs[i]);
            auto cfg = ssa_form.get_cfg();
            if (compiler_option == COMPILER_OPTION::SSA) {
                cfg->to_ssa();
                if (is_graph) {
                    cfg->to_graphvis(output, APPEND_FILE);
                } else {
                    std::ofstream ir_out;
                    ir_out.open(output, std::ios_base::out | std::ios_base::app);
                    ir_out << *cfg;
                    ir_out.close();
                }
            } else if (compiler_option == COMPILER_OPTION::IR) {
                if (is_graph) {
                    cfg->to_graphvis(output, APPEND_FILE);
                } else {
                    std::ofstream ssa_out;
                    ssa_out.open(output, std::ios_base::out | std::ios_base::app);
                    ssa_out << *cfg;
                    ssa_out.close();
                }
            }
            delete cfg;
        }
        delete unit;
        return 0;
    }
    CODEGEN codegen = CODEGEN(unit);
    if (compiler_option == COMPILER_OPTION::CODEGEN) {
        auto module = codegen.get_module();
        std::ofstream codegen_out;
        codegen_out.open(output);
        module->print(llvm::outs(), nullptr); // TODO: 
        codegen_out.close();
        return 0;
    }

    codegen.generate_obj(output);
    
    return 0;
}
