add_subdirectory(parsing)

add_library(CharacterStream CharacterStream.cpp)

add_library(Token Token.cpp)

add_library(CharacterSet CharacterSet.cpp)
target_link_libraries(CharacterSet nlohmann_json::nlohmann_json)

add_library(Lexer Lexer.cpp)
target_link_libraries(Lexer Token)
target_link_libraries(Lexer CharacterStream)
target_link_libraries(Lexer CharacterSet)

add_library(MconHelpers MconHelpers.cpp)
target_link_libraries(MconHelpers nlohmann_json::nlohmann_json)
target_link_libraries(MconHelpers Settings)

add_library(Settings Settings.cpp)
target_link_libraries(Settings Lexer)
target_link_libraries(Settings ParsingTree)

IF(WIN32)
ELSE()
    add_library(LibFakeKey libfakekey.c)
    target_link_libraries(MconHelpers LibFakeKey)
    target_link_libraries(LibFakeKey ${X11_LIBRARIES})
    target_link_libraries(LibFakeKey ${XTEST_LIBRARIES})
ENDIF()