#ifndef __VERSION_H__
#define __VERSION_H__

// Standard libraries
#include <iostream>
#include <string>
#include <vector>

// Definitions and forward declarations
#include "Definitions.hpp"
#include "ForwardDeclarations.hpp"

// Custom headers
#include "Lexer.hpp"

// JSON header
#include <nlohmann/json.hpp>

// cURL header
#include <curl/curl.h>

namespace mcon
{
    class Version
    {
        public:
            Version(std::string a_current_version);
            ~Version();

            void Check();

        private:
            static size_t WriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
            std::string PollGithub();
            std::string ReadFromJSON(std::string a_json_string);
            std::vector<int> ToDigits(std::string a_version);
            bool IsNewer(std::string a_version);

            std::string current_version;
            std::vector<int> current_version_digits;
            std::unique_ptr<CharacterStream> character_stream = std::make_unique<CharacterStream>();
            std::shared_ptr<CharacterSet> character_set = std::make_shared<CharacterSet>();
            Lexer lexer;
    };
}

#endif // __VERSION_H__