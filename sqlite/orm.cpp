
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "orm.h"




int main() {
    auto tbl = Table( "lotto", {
        std::vector<Column>({
            Column("n1", Integer),
            Column("n2", Integer),
            Column("n3", Integer),
            Column("n4", Integer),
            Column("n5", Integer),
            Column("n6", Integer),
            IntCol("super"),
        })
    });
    tbl.select({"n1>20", 10, 0});
    for (auto row : tbl.rows) {
        for (auto col : row) {
            std::cout << std::setw(5) << std::right << col;
        }
        std::cout << std::endl;
    }
    tbl.update({
        "n1=46",
        std::vector<UpdateSet>({{"n1", 56}})
    });
    tbl.select({"n1>20", 10, 0});
    for (auto row : tbl.rows) {
        for (auto col : row) {
            std::cout << std::setw(5) << std::right << col;
        }
        std::cout << std::endl;
    }
}