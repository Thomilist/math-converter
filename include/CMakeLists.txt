add_library(CharacterStream CharacterStream.cpp)

add_library(Token Token.cpp)

add_library(Lexer Lexer.cpp)
target_link_libraries(Lexer Token)
target_link_libraries(Lexer CharacterStream)

add_library(CharacterSet CharacterSet.cpp)
target_link_libraries(CharacterSet nlohmann_json::nlohmann_json)
target_link_libraries(CharacterSet MconHelpers)

add_library(MconHelpers MconHelpers.cpp)
target_link_libraries(MconHelpers nlohmann_json::nlohmann_json)