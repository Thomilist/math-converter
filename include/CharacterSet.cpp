#include "CharacterSet.hpp"

namespace mcon
{
    CharacterSet::CharacterSet() 
    { }
    
    CharacterSet::~CharacterSet() 
    { }
    
    void CharacterSet::LoadFromFolder(std::string a_folder_path) 
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(a_folder_path))
        {
            if (entry.path().extension() == ".json")
            {
                LoadFromFile(entry.path().string());
            }
        }
    }
    
    void CharacterSet::LoadFromFile(std::string a_file_path)
    {
        nlohmann::json json_character_set = LoadJSON(a_file_path);

        std::set<std::pair<std::string, std::set<std::wstring>*>> character_sets = {
            {"whitespace", &whitespace},
            {"letter", &letter},
            {"number", &number},
            {"symbol", &symbol}
        };

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

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
                    set.second->insert(converter.from_bytes(character.value()));
                }
            }
        }
    }
}