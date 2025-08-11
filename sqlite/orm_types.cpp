#include "orm.h"

std::string HeaderRow::getNames() const  {
    std::string result = headers[0].name;
    for (int i = 1; i < headers.size(); i++) {
        result += ", " + headers[i].name;
    }
    return result;
}