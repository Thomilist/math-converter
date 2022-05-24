add_subdirectory(intermediate)
add_subdirectory(mathcad)

add_library(Parser Parser.cpp)
target_link_libraries(Parser MathcadParser)
target_link_libraries(Parser Lexer)
target_link_libraries(Parser Node)

add_library(Generator Generator.cpp)
target_link_libraries(Generator MathcadGenerator)