#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include "ChinaIdNumber.h"

int main(int argc, char *argv[]) {
    std::string id;
    std::locale::global(std::locale(""));
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv; //定义utf-8 -> utf-16 转换器

    // 第一个参数argv[0]一定是程序的名称，并且包含了程序所在的完整路径
    if (argc > 2) {
        std::wcout << L"仅支持*18位身份证号*作为单参数" << std::endl;
        return 0;
    }

    if (argc == 1) {
        //没有附加参数
        std::wcout << L"请输入18位身份证号：" << std::flush;
        std::cin >> id;
    }
    if (argc == 2) {
        id = argv[1];
    }
    try {

        // 初始化CSV文件
        ChinaIdNumber::InitAreaNameMap();

        // 给定CVS文件地址初始化
        //ChinaIdNumber::InitAreaNameMap(R"(C:\Users\smile\Documents\GitHub\ChinaIdNumberCheck\cmake-build-debug\address_code.csv)");

        auto chinaIdNumber = new ChinaIdNumber(id);
        std::wcout << L"身份证号：" << conv.from_bytes(id) << std::endl;
        std::wcout << L"地区代码：" << conv.from_bytes(chinaIdNumber->GetAreaCode()) << std::endl;
        std::wcout << L"地区：" << chinaIdNumber->GetAreaInfo() << std::endl;
        std::wcout << L"出生日期：" << chinaIdNumber->GetBirthInfo() << std::endl;
        std::wcout << L"性别：" << chinaIdNumber->GetGenderInfo() << std::endl;
        std::wcout << L"年龄：" << chinaIdNumber->GetAgeInfo() << std::endl;
        std::wcout << L"合法性：" << (chinaIdNumber->isLegal ? L"是" : L"否") << std::endl;
        delete chinaIdNumber;
    } catch (std::runtime_error &err) {
        std::wcout << L"运行时错误：" << conv.from_bytes(err.what()) << std::endl;
    } catch (std::length_error &err) {
        std::wcout << L"长度错误：" << conv.from_bytes(err.what()) << std::endl;
    }

    return 0;
}
