add_library(Node Node.cpp)
add_library(ParsingTree ParsingTree.cpp)
target_link_libraries(ParsingTree Node)