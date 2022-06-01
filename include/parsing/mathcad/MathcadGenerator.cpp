#include "MathcadGenerator.hpp"
namespace mcon
{
    MathcadGenerator::MathcadGenerator(std::unique_ptr<CharacterSet> a_character_set)
        : Generator(std::move(a_character_set))
    { }
    
    MathcadGenerator::~MathcadGenerator()
    { }
}