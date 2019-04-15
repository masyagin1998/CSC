#include "datatypetokens.hpp"

const std::string DATATYPE_TOKEN::INT = "int";

static bool check_datatype(std::string datatype, POSITION pos)
{
    std::size_t i = 0;
    while (i != datatype.size()) {
        if (datatype[i] != pos.get_code()) {
            return false;
        }
        i++;
        pos = pos.next();
    }
    
    if (!pos.is_letter() && !pos.is_decimal_digit()) {
        return true;
    }

    return false;
}

DOMAIN_TAG DATATYPE_TOKEN::is(POSITION pos)
{
    if (check_datatype(INT, pos)) {
        return DOMAIN_TAG::INT;
    }

    return DOMAIN_TAG::END_OF_FILE;
}

DATATYPE_TOKEN*DATATYPE_TOKEN::read(POSITION pos, DOMAIN_TAG tag)
{
    POSITION starting = pos;
    std::string datatype;
    
    switch (tag) {
    case DOMAIN_TAG::INT:
        datatype = INT;
        break;
    default:
        // TODO: throw exception.
        break;
    }

    std::size_t i = 0;
    while (i < datatype.size()) {
        i++;
        pos = pos.next();
    }
    
    return new DATATYPE_TOKEN(tag, starting, pos, datatype);
}

DATATYPE_TOKEN::DATATYPE_TOKEN(DOMAIN_TAG tag, POSITION starting, POSITION following, std::string datatype) : TOKEN(tag, starting, following), datatype(datatype) {}

std::ostream& operator<<(std::ostream &strm, const DATATYPE_TOKEN &tok)
{
    return strm << "DATATYPE " << ((TOKEN) tok) << ": " << tok.datatype;
}
