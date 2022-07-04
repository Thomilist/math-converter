#include "CharacterSet.hpp"

namespace mcon
{
    CharacterSet::CharacterSet() 
    { }
    
    CharacterSet::~CharacterSet() 
    { }

    nlohmann::json CharacterSet::LoadJSON(std::string a_file_path) 
    {
        std::ifstream json_file(a_file_path);

        if (!json_file.is_open())
        {
            throw std::runtime_error("Unable to open JSON file.");
        }

        // Parse JSON file, allowing JavaScript-style comments
        nlohmann::json json_object = nlohmann::json::parse(json_file, nullptr, true, true);

        json_file.close();

        return json_object;
    }
    
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

        std::set<std::pair<std::string, std::set<String>*>> character_sets = {
            {"whitespace", &whitespace},
            {"letter", &letter},
            {"number", &number},
            {"symbol", &symbol}
        };

        #ifdef WIN32
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        #endif

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
                    #ifdef WIN32
                        set.second->insert(converter.from_bytes(character.value()));
                    #else
                        set.second->insert(character.value());
                    #endif
                }
            }
        }
    }
}