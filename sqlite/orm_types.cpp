#include "orm.h"

std::string HeaderRow::getNames() const  {
    std::string result = headers[0].name;
    for (int i = 1; i < headers.size(); i++) {
        result += ", " + headers[i].name;
    }
    return result;
}


ColumnType HeaderRow::getColumnType(const std::string &name) const {
    for (auto col : headers) {
        if (col.name == name) {
            return col.type;
        }
    }
    throw std::runtime_error("Column not found");
}