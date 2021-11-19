# CMakeList.txt: function_printer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("function_printer")

# 将源代码添加到此项目的可执行文件。
add_executable (function_printer "function_printer.cpp" "function_printer.h"   )

add_executable(scanner "scanner.cpp" "scanner.h" "scannertest.cpp")

# TODO: 如有需要，请添加测试并安装目标。

#add_subdirectory("scanner")

#add_subdirectory("semantic")

#add_subdirectory("parser")
