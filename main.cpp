#include <iostream>
#include <string>
#include "ChinaIdNumber.h"

int main() {
    std::string IdStr = "510902199808085952";
    std::array<char, 18> ID{};
    for (auto idx = 0; idx < 18; idx++) {
        ID[idx] = IdStr[idx];
    }
    std::string path = R"(C:\Users\smile\CLionProjects\ChinaIdNumberCheck\address_code.csv)";
    auto test = ChinaIdNumber(ID, path);

    auto t = test.areaNameMap[{'5','1','0','9','0','2'}];
    return 0;
}
