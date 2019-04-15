#ifndef DOMAIN_TAG_HPP_INCLUDED
#define DOMAIN_TAG_HPP_INCLUDED

enum class DOMAIN_TAG
{
    // KEYWORDS.
    RETURN,       // return - return value from function;

    FOR,          // for - for-loop;
    WHILE,        // while - while-loop;
    DO,           // do - do-while-loop;
    
    IF,           // if-op;
    ELSE,         // else-op;

    // DATATYPES.
    INT,          // int;

    // INSTRUCTION SEPARATORS.
    LBRACE,       // { - block beginning;
    RBRACE,       // } - block ending;
    
    LPAREN,       // (;
    RPAREN,       // );

    SEMICOLON,    // ; - instructions separator;
    COMMA,        // , - function arguments separator;

    // IDENTS.
    IDENT,        // [a-z][a-z0-9]+ - variable identifiers;

    // INTEGER CONSTANTS.
    INTEGER,      // [0-9]+ - number in 10-th SS, ([2-9]|1[0-6])x[0-9a-z]+ - number in 2..16-th SS;

    // EQUALITY OPERATORS.
    EQ,           // ==;
    NEQ,          // !=;

    // LOGICAL OPERATORS.
    LOGICAL_AND,  // &&;
    LOGICAL_OR,   // ||;
    LOGICAL_NOT,  // !;

    // RELATIONAL OPERATORS.
    MORE,         // >;
    LESS,         // <;
    MORE_OR_EQ,   // >=;
    LESS_OR_EQ,   // <=;

    // ASSIGN OPERATORS.
    ASSIGN,       // =;
    ASSIGN_PLUS,  // +=;
    ASSIGN_MINUS, // -=;
    ASSIGN_MUL,   // *=;
    ASSIGN_DIV,   // /=;
    ASSIGN_MOD,   // %=;

    // BINARY OPERATORS.
    PLUS,         // +;
    MINUS,        // -;
    MUL,          // *;
    DIV,          // /;
    MOD,          // %;

    // COMMENTS.
    COMMENT,      // /*...*/ - многострочный комментарий, // ... - однострочный комментарий;

    // END OF FILE.
    END_OF_FILE,  // признак конца файла.

    // UNKNOWN TAG.
    UNKNOWN,      // неизвестный тэг;
};

#endif  // DOMAIN_TAG_HPP_INCLUDED
