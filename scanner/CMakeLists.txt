project(scanner)

add_library (scanner STATIC "scanner.cpp" "scanner.h")

add_executable (scannertest "scanner.cpp" "scanner.h" "scannertest.cpp")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)