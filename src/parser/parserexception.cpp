#include "parserexception.hpp"

PARSER_EXCEPTION::PARSER_EXCEPTION(std::string exception) : exception(exception) {}

void PARSER_EXCEPTION::prepend_exception(std::string prepend)
{
    exception = prepend + exception;
}

std::string PARSER_EXCEPTION::get_exception()
{
    return exception;
}

std::ostream& operator<<(std::ostream &strm, const PARSER_EXCEPTION &excp)
{
    strm << excp.exception;
    return strm;
}
