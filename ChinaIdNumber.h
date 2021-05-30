//
// Created by smile on 2021/5/29.
//

#ifndef CHINA_ID_NUMBER_H
#define CHINA_ID_NUMBER_H

#include <array>
#include <map>
#include <string>

class ChinaIdNumber {
public:

    // 身份证号的组成结构：
    //  区域码(省2位+市2位+区2位) 出生日期码(8位) 顺序码(2位) 性别码(1位) 校验码(1位)
    std::array<char, 2> ProvinceId{};       // 省
    std::array<char, 2> CityId{};           // 市
    std::array<char, 2> CountyId{};         // 区

    std::array<char, 4> BirthYear{};
    std::array<char, 2> BirthMonth{};
    std::array<char, 2> BirthDay{};

    // csv文件来自于 https://github.com/jxlwqq/address-code-of-china 项目
    explicit ChinaIdNumber(std::array<char, 18> &Id, std::string csvFilePath = "address_code.csv");


    // 地区码与地区名对应Map
    std::map<std::array<char, 6>, std::wstring> areaNameMap;

private:


    // 加载文件并初始化
    void initAreaNameMap(std::string &csvFilePath);

    // 获取6位区域ID
    std::array<char, 6> getAreaId();

};


#endif //CHINA_ID_NUMBER_H
