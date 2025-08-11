
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
    std::cout << "---" << std::endl;
    tbl.select({"n1>20", 10, 0});
    for (auto row : tbl.rows) {
        for (auto col : row) {
            std::cout << std::setw(5) << std::right << col;
        }
        std::cout << std::endl;
    }

    tbl.insert({
        std::vector<Column>({
            IntCol("n1"),
            IntCol("n2"),
            IntCol("n3"),
            IntCol("n4"),
            IntCol("n5"),
            IntCol("n6"),
            IntCol("super"),
        }),
        std::vector<std::vector<DB_Type>>({
        {50, 50, 50, 50, 50, 50, 50},
        {51, 51, 51, 51, 51, 51, 51}})
    });
    std::cout << "---" << std::endl;
    tbl.select({"n1>20", 10, 0});
    for (auto row : tbl.rows) {
        for (auto col : row) {
            std::cout << std::setw(5) << std::right << col;
        }
        std::cout << std::endl;
    }
}