#include "CharacterSet.hpp"

namespace mcon
{
    CharacterSet::CharacterSet() 
    { }
    
    CharacterSet::~CharacterSet() 
    { }
    
    bool CharacterSet::LoadFromFolder(std::string a_folder_path) 
    {
        
    }
    
    bool CharacterSet::LoadFromFile(std::string a_file_path)
    {
        nlohmann::json json_character_set = LoadJSON(a_file_path);

        std::set<std::pair<std::string, std::set<std::string>*>> character_sets = {
            {"whitespace", &whitespace},
            {"letter", &letter},
            {"number", &number},
            {"symbol", &symbol}
        };

        // Iterate over the four character sets
        for (auto& set : character_sets)
        {
            // Check if the JSON object contains an instance of the character set types
            if (json_character_set.find(set.first) != json_character_set.end())
            {
                // Iterate over the JSON array of characters
                for (auto& character : json_character_set.at(set.first).items())
                {
                    // Insert new character if not present already
                    set.second->insert(std::string(character.value()));
                }
            }
        }
    }
}