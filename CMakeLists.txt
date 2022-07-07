cmake_minimum_required(VERSION 3.22.0)
project(math-converter VERSION 1.3.0)

add_compile_options(-Wall -Wextra -pedantic -Werror)

# Installer packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Main program
IF(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/icon/icon.rc")
    add_executable(
        ${PROJECT_NAME}
        main.cpp
        ${APP_ICON_RESOURCE_WINDOWS}
    )
ELSE()
    add_executable(
        ${PROJECT_NAME}
        main.cpp
    )
ENDIF()

# JSON for Modern C++
include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_PROGRESS TRUE
  GIT_SHALLOW TRUE
  GIT_TAG v3.10.5)

FetchContent_MakeAvailable(json)

target_link_libraries(math-converter PRIVATE nlohmann_json::nlohmann_json)

# X11
IF(WIN32)
ELSE()
    find_package(X11 REQUIRED)
    link_libraries(${X11_LIBRARIES})
    include_directories(${X11_INCLUDE_DIR})

    find_path(  XTEST_INCLUDE_DIR NAMES X11/extensions/XTest.h
                PATH_SUFFIXES X11/extensions
                DOC "The XTest include directory"
    )

    find_library(   XTEST_LIBRARY NAMES Xtst
                    DOC "The XTest library"
    )

    find_package(PackageHandleStandardArgs REQUIRED)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(XTest DEFAULT_MSG XTEST_LIBRARY XTEST_INCLUDE_DIR)

    if(XTEST_FOUND)
        set(XTEST_LIBRARIES ${XTEST_LIBRARY})
        set(XTEST_INCLUDE_DIRS ${XTEST_INCLUDE_DIR})
    endif()

    include_directories(/user/include/fakekey)
ENDIF()

# Custom headers
add_subdirectory(include)
target_link_libraries(math-converter PRIVATE CharacterStream)
target_link_libraries(math-converter PRIVATE Token)
target_link_libraries(math-converter PRIVATE Lexer)
target_link_libraries(math-converter PRIVATE CharacterSet)
target_link_libraries(math-converter PRIVATE ParsingTree)
target_link_libraries(math-converter PRIVATE Node)
target_link_libraries(math-converter PRIVATE MconHelpers)
target_link_libraries(math-converter PRIVATE Parser)
target_link_libraries(math-converter PRIVATE Generator)
target_link_libraries(math-converter PRIVATE MathcadParser)
target_link_libraries(math-converter PRIVATE MathcadGenerator)
target_link_libraries(math-converter PRIVATE LatexParser)
target_link_libraries(math-converter PRIVATE LatexGenerator)
target_link_libraries(math-converter PRIVATE MathMLParser)
target_link_libraries(math-converter PRIVATE MathMLGenerator)
target_link_libraries(math-converter PRIVATE UnicodeMathParser)
target_link_libraries(math-converter PRIVATE UnicodeMathGenerator)
target_link_libraries(math-converter PRIVATE Settings)

IF(WIN32)
ELSE()
    target_link_libraries(math-converter PRIVATE LibFakeKey)
ENDIF()