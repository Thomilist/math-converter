#ifndef __CHARACTERSET_H__
#define __CHARACTERSET_H__

// Standard libraries
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <utility>
#include <filesystem>
#include <locale>
#include <codecvt>

// JSON header
#include <nlohmann\json.hpp>

// Custom headers
#include "MconHelpers.hpp"

namespace mcon
{
    class CharacterSet : public std::enable_shared_from_this<CharacterSet>
    {
        public:
            CharacterSet();
            ~CharacterSet();

            void LoadFromFolder(std::string a_folder_path);
            void LoadFromFile(std::string a_file_path);

            std::set<std::wstring> end_of_stream = {L"\0", L"", std::wstring(1, char(0))};
            std::set<std::wstring> whitespace;
            std::set<std::wstring> letter;
            std::set<std::wstring> number;
            std::set<std::wstring> symbol;
        
        private:

    };
}

#endif // __CHARACTERSET_H__