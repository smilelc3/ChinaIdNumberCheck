//
// Created by smile on 2021/5/29.
//
#include "ChinaIdNumber.h"
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <ctime>

ChinaIdNumber::ChinaIdNumber(std::array<char, 18> &id, std::string csvFilePath) {
    this->ProvinceId = {id[0], id[1]};
    this->CityId = {id[2], id[3]};
    this->CountyId = {id[4], id[5]};

    this->BirthYear = {id[6], id[7], id[8], id[9]};
    this->BirthMonth = {id[10], id[11]};
    this->BirthDay = {id[12], id[13]};

    this->OrderCode = {id[14], id[15]};
    this->GenderCode = {id[16]};

    this->CheckCode = {id[17]};

    initAreaNameMap(csvFilePath);

    this->isLegal = this->CheckCode.front() == CalcCheckCode();
}

ChinaIdNumber::ChinaIdNumber(std::string &idStr, std::string csvFilePath) {
    if (idStr.size() != 18) {
        throw std::length_error("id size must be 18");
    }
    std::array<char, 18> id{};
    for (auto idx = 0; idx < 18; idx++) {
        id[idx] = idStr[idx];
    }
    new(this) ChinaIdNumber(id, std::move(csvFilePath));
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
        std::string areaCode;
        std::wstring areaName;

        // 按照逗号分隔,并赋值给year, areaId, areaName
        while (getline(lineStream, itemStr, ',')) {
            if (not itemStr.empty() and itemStr.back() == '\r') {
                itemStr.erase(itemStr.size() - 1);
            }

            if (col == 0) { // 标准年份
                year = std::move(itemStr);
            } else if (col == 1) { // 地区码
                areaCode = std::move(itemStr);
            } else if (col == 2) {
                areaName = std::move(converter.from_bytes(itemStr));
            }
            col++;
        }
        this->areaNameMap[areaCode] = {areaName, year};
        row++;
    }
}

std::wstring ChinaIdNumber::GetAreaInfo() {
    std::wstring res;
    auto areaCode = this->GetAreaCode();
    decltype(areaNameMap)::iterator iter;
    iter = areaNameMap.find(areaCode.substr(0,2) + "0000");
    if (iter != areaNameMap.end()) {
        res.append(iter->second.first);
    }
    iter = areaNameMap.find(areaCode.substr(0,4) + "00");
    if (iter != areaNameMap.end()) {
        res.append(iter->second.first);
    }
    iter = areaNameMap.find(areaCode);
    if (iter != areaNameMap.end()) {
        res.append(iter->second.first);
    }
    return res;
}

std::wstring ChinaIdNumber::GetBirthInfo() {
    std::wstringstream wss;
    for (const auto &Ch: this->BirthYear) {
        wss << Ch;
    }
    wss << L"年";
    for (const auto &Ch: this->BirthMonth) {
        wss << Ch;
    }
    wss << L"月";
    for (const auto &Ch: this->BirthDay) {
        wss << Ch;
    }
    wss << L"日";
    return wss.str();
}

std::wstring ChinaIdNumber::GetGenderInfo() {
    if (((this->GenderCode.front() - '0') & 1) == 1) {
        // 奇数代表
        return L"男";
    }
    return L"女";
}

char ChinaIdNumber::CalcCheckCode() {
    const std::array<int, 17> weights = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    int sum = 0;
    for (auto idx = 0; idx < weights.size(); idx++) {
        sum += weights[idx] * (this->getCharByIdx(idx) - '0');
    }
    char checkCode = '0' + (12 - (sum % 11)) % 11;
    return checkCode <= '9' ? checkCode : 'X';
}

char ChinaIdNumber::getCharByIdx(size_t idx) {
    if (idx < 2) {
        return this->ProvinceId[idx];
    } else if (idx < 4) {
        return this->CityId[idx - 2];
    } else if (idx < 6) {
        return this->CountyId[idx - 4];
    } else if (idx < 10) {
        return this->BirthYear[idx - 6];
    } else if (idx < 12) {
        return this->BirthMonth[idx - 10];
    } else if (idx < 14) {
        return this->BirthDay[idx - 12];
    } else if (idx < 16) {
        return this->OrderCode[idx - 14];
    } else if (idx < 17) {
        return this->GenderCode[idx - 16];
    } else {
        throw std::out_of_range("idx must be >= 0 and <= 16");
    }
}

std::string ChinaIdNumber::GetAreaCode() {
    std::stringstream ss;
    for (const auto &item : this->ProvinceId) {
        ss << item;
    }
    for (const auto &item : this->CityId) {
        ss << item;
    }
    for (const auto &item : this->CountyId) {
        ss << item;
    }
    return ss.str();
}

std::wstring ChinaIdNumber::GetAgeInfo() {
    return std::to_wstring(this->calcAge()) + L"岁";
}

int ChinaIdNumber::calcAge() {
    std::time_t t = std::time(nullptr);   // get time now
    std::tm *now = std::localtime(&t);
    int birthYear = (BirthYear[0] - '0') * 1000
                    + (BirthYear[1] - '0') * 100
                    + (BirthYear[2] - '0') * 10
                    + (BirthYear[3] - '0');
    int birthMonth = (BirthMonth[0] - '0') * 10 + (BirthMonth[1] - '0');
    int birthDay = (BirthDay[0] - '0') * 10 + (BirthDay[1] - '0');

    // 当前年份：t.tm_year + 1900
    // 当前月份：t.tm_mon + 1
    // 当前日：t.tm_mday
    auto age = now->tm_year + 1900 - birthYear;
    if (now->tm_mon + 1 < birthMonth or (now->tm_mon + 1 == birthMonth and now->tm_mday < birthDay)) {
        age--;
    }
    return age;
}

