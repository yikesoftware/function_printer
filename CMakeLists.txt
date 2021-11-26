# CMakeList.txt: function_printer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("function_printer")

#include_directories("./")

# 将源代码添加到此项目的可执行文件。

aux_source_directory(. DIR_SRCS)

add_executable (parsertest ${DIR_SRCS})

#add_executable (function_printer "function_printer.cpp" "function_printer.h")

# TODO: 如有需要，请添加测试并安装目标。

#add_subdirectory("scanner")

#add_subdirectory("parser")

#add_subdirectory("semantic")
