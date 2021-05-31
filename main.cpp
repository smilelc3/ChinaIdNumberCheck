#include <iostream>
#include <string>
#include "ChinaIdNumber.h"

int main(int argc, char *argv[]) {
    std::string id;
    std::wcout.imbue(std::locale("chs"));
    // 第一个参数argv[0]一定是程序的名称，并且包含了程序所在的完整路径
    if (argc > 2) {
        std::wcout << L"仅支持*18位身份证号*作为单参数" << std::endl;
        return 0;
    }

    if (argc == 1) {
        //没有附加参数
        std::wcout << L"请输入18位身份证号：";
        std::cin >> id;
    }
    if (argc == 2) {
        id = argv[1];
        std::wcout << L"身份证号：";
        std::cout << id << std::endl;
    }

    auto chinaIdNumber = new ChinaIdNumber(id);
    std::wcout << L"地区代码：";
    std::cout << chinaIdNumber->GetAreaCode() << std::endl;
    std::wcout << L"地区：" << chinaIdNumber->GetAreaInfo() << std::endl;
    std::wcout << L"出生日期：" << chinaIdNumber->GetBirthInfo() << std::endl;
    std::wcout << L"性别：" << chinaIdNumber->GetGenderInfo() << std::endl;
    std::wcout << L"年龄：" << chinaIdNumber->GetAgeInfo() << std::endl;
    std::wcout << L"合法性：" << (chinaIdNumber->isLegal == true ? L"是" : L"否") << std::endl;
    delete chinaIdNumber;
    return 0;
}
