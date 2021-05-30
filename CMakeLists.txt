cmake_minimum_required(VERSION 3.0)
project(ChinaIdNumberCheck)


# 使用Visual Studio编译额外的修改
if (MSVC)
    # 指定编译器的标准一致性模式(支持关键词：and or not)
    add_compile_options(/permissive-)
    # 禁用warning C4819警告(utf-8文件警告)
    add_compile_options(/wd4819)
endif()

set(CMAKE_CXX_STANDARD 14)

add_library(ChinaIdNumber STATIC ChinaIdNumber.cpp)

add_executable(ChinaIdNumberCheck main.cpp)

target_link_libraries(ChinaIdNumberCheck ChinaIdNumber)