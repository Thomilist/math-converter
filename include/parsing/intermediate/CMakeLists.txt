add_library(Node Node.cpp)
add_library(ParsingTree ParsingTree.cpp)

target_link_libraries(ParsingTree Node)
target_link_libraries(ParsingTree Parser)
target_link_libraries(ParsingTree Generator)