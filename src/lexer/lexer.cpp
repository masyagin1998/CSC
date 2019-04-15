#include "lexer.hpp"

#include <fstream>
#include <iostream>

LEXER::LEXER(std::string fname) : skip_comments(false), curr(nullptr)
{
      std::ifstream f(fname);
      program = new std::string();
      std::string content((std::istreambuf_iterator<char>(f)),
                          (std::istreambuf_iterator<char>()));
      *program = content;

      curr = POSITION(program);
}

void LEXER::enable_skip_comments() {
    skip_comments = true;
}

void LEXER::disable_skip_comments() {
    skip_comments = false;
}

TOKEN*LEXER::next_token()
{
    while (!curr.is_EOF()) {
        while (curr.is_whitespace() || curr.is_newline()) {
            curr = curr.next();
        }
        TOKEN*tok = nullptr;
        DOMAIN_TAG tag;
        // Keywords and datatypes (and all tokens, which could be idents without special context)
        // should be checked before idents.
        if ((tag = KEYWORD_TOKEN::is(curr)) != DOMAIN_TAG::END_OF_FILE) {
            tok = KEYWORD_TOKEN::read(curr, tag);
        } else if ((tag = DATATYPE_TOKEN::is(curr)) != DOMAIN_TAG::END_OF_FILE) {
            tok = DATATYPE_TOKEN::read(curr, tag);
        } else if ((tag = BRACE_TOKEN::is(curr)) != DOMAIN_TAG::END_OF_FILE) {
            tok = BRACE_TOKEN::read(curr, tag);
        } else if ((tag = PAREN_TOKEN::is(curr)) != DOMAIN_TAG::END_OF_FILE) {
            tok = PAREN_TOKEN::read(curr, tag);
        } else if ((tag = SEP_TOKEN::is(curr)) != DOMAIN_TAG::END_OF_FILE) {
            tok = SEP_TOKEN::read(curr, tag);
        } else if (curr.is_letter()) {
            tok = IDENT_TOKEN::read(curr);
        } else if (curr.is_decimal_digit()) {
            tok = INTEGER_TOKEN::read(curr);
            // Comments should be checked before binary operations.
        } else if ((tag = COMMENT_TOKEN::is(curr)) != DOMAIN_TAG::END_OF_FILE) {
            tok = COMMENT_TOKEN::read(curr, tag);
            // Equality tokens should be checked before logical tokens.
        } else if ((tag = EQUALITY_TOKEN::is(curr)) != DOMAIN_TAG::END_OF_FILE) {
            tok = EQUALITY_TOKEN::read(curr, tag);
        } else if ((tag = LOGICAL_TOKEN::is(curr)) != DOMAIN_TAG::END_OF_FILE) {
            tok = LOGICAL_TOKEN::read(curr, tag);
        } else if ((tag = RELATIONAL_TOKEN::is(curr)) != DOMAIN_TAG::END_OF_FILE) {
            tok = RELATIONAL_TOKEN::read(curr, tag);
        } else if ((tag = ASSIGN_TOKEN::is(curr)) != DOMAIN_TAG::END_OF_FILE) {
            tok = ASSIGN_TOKEN::read(curr, tag);
        } else if ((tag = BIN_OP_TOKEN::is(curr)) != DOMAIN_TAG::END_OF_FILE) {
            tok = BIN_OP_TOKEN::read(curr, tag);
        } else {
            tok = UNKNOWN_TOKEN::read(curr);
        }

        curr = tok->get_frag().get_following();
        if (tok->get_tag() == DOMAIN_TAG::UNKNOWN) {
            UNKNOWN_TOKEN*unknown_tok = dynamic_cast<UNKNOWN_TOKEN*>(tok);
            std::cerr << std::endl;
            std::cerr << "WARNING: " << *unknown_tok << std::endl;
            std::cerr << std::endl;
            delete tok;
        } else if ((tok->get_tag() != DOMAIN_TAG::COMMENT) || ((tok->get_tag() == DOMAIN_TAG::COMMENT) && (!skip_comments))) {
            return tok;
        }
        
    }
    return new UNKNOWN_TOKEN(DOMAIN_TAG::END_OF_FILE, "EOF", curr, curr);
}

LEXER::~LEXER()
{
    delete program;
}
