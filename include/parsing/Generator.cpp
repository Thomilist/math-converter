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
    #pragma GCC diagnostic pop

    void Generator::Substitute(std::shared_ptr<ParsingTree> a_parsing_tree)
    {
        for (auto& substitution_item : GetSubstitutionList())
        {
            while (true)
            {
                std::size_t position = a_parsing_tree->output.find(substitution_item.first);

                if (position == String::npos)
                {
                    break;
                }

                a_parsing_tree->output.replace(position, substitution_item.first.length(), substitution_item.second);
            }
        }

        return;
    }

    
    const std::unordered_map<String, String>& Generator::GetSubstitutionList()
    {
        return substitution_list;
    }
}