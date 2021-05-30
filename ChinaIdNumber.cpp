//
// Created by smile on 2021/5/29.
//

#include "ChinaIdNumber.h"
#include <fstream>
#include <string>
#include <iostream>
#include <locale>
#include <codecvt>
#include <sstream>


ChinaIdNumber::ChinaIdNumber(std::array<char, 18> &Id, std::string csvFilePath) {
    this->ProvinceId = {Id[0], Id[1]};
    this->CityId = {Id[2], Id[3]};
    this->CountyId = {Id[4], Id[5]};

    initAreaNameMap(csvFilePath);
}

std::array<char, 6> ChinaIdNumber::getAreaId() {
    return std::array<char, 6>{ProvinceId[0], ProvinceId[1],
                               CityId[0], CityId[1],
                               CountyId[0], CountyId[1]};
}

void ChinaIdNumber::initAreaNameMap(std::string &csvFilePath) {
    std::ifstream fp(csvFilePath);
    std::string line;  // 每行 bytes源码
    size_t row = 0;

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    while (std::getline(fp, line)) {
        size_t col = 0;
        std::stringstream lineStream(line);
        std::string itemStr;
        std::string year;
        std::array<char, 6> areaId{};
        std::wstring areaName;

        // 按照逗号分隔,并赋值给year, areaId, areaName
        while (getline(lineStream, itemStr, ',')) {
            if (col == 0) {
                // 标准年份
                year = itemStr;
            } else if (col == 1) {
                // 地区码
                for (auto idx = 0; idx < areaId.size(); idx++) {
                    areaId[idx] = itemStr.at(idx);
                }
            } else if (col == 2) {
                areaName = converter.from_bytes(itemStr);
            }
            col++;
        }

        if (areaId[2] == '0' and areaId[3] == '0') {        // 省级代码
            areaNameMap[areaId] = areaName;
        } else {
            if (areaId[4] == '0' and areaId[5] == '0') {    // 市级别代码
                auto provinceFullId = areaId;
                provinceFullId[2] = provinceFullId[3] = '0';
                areaNameMap[areaId] = areaNameMap[provinceFullId] + areaName;
            } else {
                auto cityFullId = areaId;
                cityFullId[4] = cityFullId[5] = '0';
                auto iter = areaNameMap.find(cityFullId);
                if (iter != areaNameMap.end()) {
                    cityFullId[2] = cityFullId[3] = '0';
                    areaNameMap[areaId] = areaNameMap[cityFullId] + areaName;   // 直辖市
                } else {
                    // 地级市
                    areaNameMap[areaId] = iter->second + areaName;

                }

            }
        }
        row++;
    }
    std::wcout << row << std::endl;

}

