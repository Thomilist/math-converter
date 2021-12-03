#ifndef __CHARACTERSET_H__
#define __CHARACTERSET_H__

// Standard libraries
#include <string>
#include <set>
#include <vector>
#include <utility>
#include <filesystem>

// JSON header
#include <nlohmann\json.hpp>

// Custom headers
#include "MconHelpers.hpp"

namespace mcon
{
    class CharacterSet
    {
        public:
            CharacterSet();
            ~CharacterSet();

            bool LoadFromFolder(std::string a_folder_path);
            bool LoadFromFile(std::string a_file_path);

            std::set<std::string> end_of_stream = {"\0"};
            std::set<std::string> whitespace;
            std::set<std::string> letter;
            std::set<std::string> number;
            std::set<std::string> symbol;
        
        private:

    };
}

#endif // __CHARACTERSET_H__