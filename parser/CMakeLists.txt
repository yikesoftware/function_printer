include_directories("../scanner")
link_directories("../scanner/lib")

add_executable(parser "parser.cpp" "parser.h" "parsertest.cpp")

TARGET_LINK_LIBRARIES(parser scanner)