# CMakeList.txt: function_printer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("function_printer")

include_directories("SDL2/include")

if (CMAKE_HOST_WIN32)
    set(WINDOWS 1)
elseif(CMAKE_HOST_APPLE)
    set(MACOS 1)
elseif(CMAKE_HOST_UNIX)
    set(LINUX 1)
endif()

IF(CMAKE_CL_64)
    set(platform x64)
ELSE(CMAKE_CL_64)
    set(platform x86)
ENDIF(CMAKE_CL_64)


if(WINDOWS)
    link_directories("${PROJECT_SOURCE_DIR}/lib/win/x64") 
    file(COPY ${PROJECT_SOURCE_DIR}/lib/win/${platform}/SDL2.dll DESTINATION ${PROJECT_SOURCE_DIR}/bin)
elseif(LINUX)
    link_directories("${PROJECT_SOURCE_DIR}/lib/linux") 
endif()

SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/")

add_executable (function_printer 
    "function_printer.cpp" "function_printer.h"
    "scanner.cpp" "scanner.h"
    "parser.cpp" "parser.h"
    "semantic.cpp" "semantic.h"
)
target_link_libraries(function_printer SDL2.lib)

#add_executable (semantictest "semantictest.cpp" "semantic.cpp" "semantic.h")
#target_link_libraries(semantictest SDL2.lib)

#add_executable (parsertest 
#    "parsertest.cpp" "parser.cpp" "parser.h"
#    "scanner.cpp" "scanner.h"
#    "semantic.cpp" "semantic.h"
#)
#target_link_libraries(parsertest SDL2)

