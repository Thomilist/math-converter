#include "Generator.hpp"



namespace mcon
{
    Generator::Generator(std::shared_ptr<CharacterSet> a_character_set)
        : character_set(a_character_set)
    { }
    
    Generator::~Generator()
    { }
    
    // Virtual function implementations are intentionally left blank
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    void Generator::Generate(std::shared_ptr<ParsingTree> a_parsing_tree)
    { }
    
    void Generator::Substitute(std::shared_ptr<ParsingTree> a_parsing_tree)
    { }
    #pragma GCC diagnostic pop
}