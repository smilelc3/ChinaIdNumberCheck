//
// Created by smile on 2021/5/29.
//

#ifndef CHINA_ID_NUMBER_H
#define CHINA_ID_NUMBER_H

#include <array>
#include <unordered_map>
#include <string>

class ChinaIdNumber {
public:

    // 身份证号的组成结构：
    //  区域码(8位） 省2位+市2位+区2位
    std::array<char, 2> ProvinceId{};       // 省
    std::array<char, 2> CityId{};           // 市
    std::array<char, 2> CountyId{};         // 区

    // 出生日期码（8位） 表示公民出生的公历年（4位）、月（2位）、日（2位）
    std::array<char, 4> BirthYear{};
    std::array<char, 2> BirthMonth{};
    std::array<char, 2> BirthDay{};

    // 顺序码(2位) 表示在同一区域码所标识的区域范围内，对同年、同月、同日出生的人编定的顺序号。
    std::array<char, 2> OrderCode{};

    // 性别码（1位） 奇数表示男性，偶数表示女性
    std::array<char, 1> GenderCode{};

    // 校验码（1位）
    std::array<char, 1> CheckCode{};

    // 是否是合法18位身份证
    bool isLegal = false;


    // csv文件来自于 https://github.com/jxlwqq/address-code-of-china 项目
    explicit ChinaIdNumber(std::array<char, 18> &id);
    explicit ChinaIdNumber(std::string &idStr);


    // 获取地区信息
    std::wstring GetAreaInfo();


    // 获取地区6位代码（以string返回）
    std::string GetAreaCode();

    // 获取出生日期信息
    std::wstring GetBirthInfo();

    // 获取性别信息
    std::wstring GetGenderInfo();

    // 计算校验码
    char CalcCheckCode();

    // 获取年龄信息
    std::wstring GetAgeInfo();

    // 读取CSV文件，并初始化AreaNameMap
    static void InitAreaNameMap(const std::string& csvFilePath = "address_code.csv");

protected:
    // 地区码与地区名+标准年份对应Map
    static std::unordered_map<std::string, std::pair<std::wstring, std::wstring>> areaNameMap;

private:

    // 根据idx获取char
    inline char getCharByIdx(size_t idx);

    // 计算年龄
    int calcAge();
};




#endif //CHINA_ID_NUMBER_H
