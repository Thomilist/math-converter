cmake_minimum_required(VERSION 3.22.0)
project(math-converter VERSION 0.1.0)

add_compile_options(-Wall -Wextra -pedantic -Werror)

# Automated testing
include(CTest)
enable_testing()

# Installer packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Main program
add_executable(math-converter main.cpp)

# JSON for Modern C++
include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.10.4)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

target_link_libraries(math-converter PRIVATE nlohmann_json::nlohmann_json)

# Custom headers
add_subdirectory(include)
target_link_libraries(math-converter PRIVATE CharacterStream)
target_link_libraries(math-converter PRIVATE Token)
target_link_libraries(math-converter PRIVATE Lexer)
target_link_libraries(math-converter PRIVATE CharacterSet)
target_link_libraries(math-converter PRIVATE ParsingTree)
target_link_libraries(math-converter PRIVATE Node)
target_link_libraries(math-converter PRIVATE Parser)
target_link_libraries(math-converter PRIVATE Generator)
target_link_libraries(math-converter PRIVATE MathcadParser)
target_link_libraries(math-converter PRIVATE MathcadGenerator)
target_link_libraries(math-converter PRIVATE LatexParser)
target_link_libraries(math-converter PRIVATE LatexGenerator)
