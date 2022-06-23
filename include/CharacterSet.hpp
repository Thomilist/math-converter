#ifndef __CHARACTERSET_H__
#define __CHARACTERSET_H__

// Standard libraries
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <utility>
#include <filesystem>
#include <locale>
#include <codecvt>

// Definitions and forward declarations
#include "Definitions.hpp"
#include "ForwardDeclarations.hpp"

// JSON header
#include <nlohmann\json.hpp>

namespace mcon
{
    class CharacterSet : public std::enable_shared_from_this<CharacterSet>
    {
        public:
            CharacterSet();
            ~CharacterSet();

            nlohmann::json LoadJSON(std::string a_file_path);
            void LoadFromFolder(std::string a_folder_path);
            void LoadFromFile(std::string a_file_path);

            std::set<String> end_of_stream = {STR("\0"), STR(""), String(1, char(0))};
            std::set<String> whitespace;
            std::set<String> letter;
            std::set<String> number;
            std::set<String> symbol;
        
        private:

    };
}

#endif // __CHARACTERSET_H__