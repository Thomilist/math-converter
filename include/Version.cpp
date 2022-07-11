#include "Version.hpp"


namespace mcon
{
    Version::Version(std::string a_current_version):
        current_version(a_current_version),
        lexer(std::move(character_stream), character_set)
    {
        character_set->LoadFromFolder("./resources/character-sets");
        current_version_digits = ToDigits(current_version);
    }
    
    Version::~Version()
    { }
    
    void Version::Check()
    {
        std::string latest_release_json = PollGithub();
        std::string latest_release_tag = ReadFromJSON(latest_release_json);

        if (IsNewer(latest_release_tag))
        {
            std::cout
                << "A new version of math-converter is available online "
                << "(version " << latest_release_tag << ").\n"
                << "Use the command \"download\" (without quote marks) to open the download page and see what's new.\n"
                << std::endl;
        }
        else
        {
            std::cout
                << "You are running the latest version of math-converter.\n"
                << std::endl;
        }

        return;
    }
    
    size_t Version::WriteCallback(char* a_contents, size_t a_size, size_t a_nmemb, void* a_userdata)
    {
        ((std::string*)a_userdata)->append(a_contents, a_size * a_nmemb);
        return a_size * a_nmemb;
    }
    
    std::string Version::PollGithub()
    {
        CURL* curl = curl_easy_init();
        std::string buffer;

        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/repos/thomilist/math-converter/releases/latest");
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Thomilist/math-converter");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
            curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }

        return buffer;
    }
    
    std::string Version::ReadFromJSON(std::string a_json_string)
    {
        nlohmann::json json_object = nlohmann::json::parse(a_json_string);
        return json_object["tag_name"];
    }
    
    std::vector<int> Version::ToDigits(std::string a_version)
    {
        #ifdef WIN32
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        lexer.character_stream->Read(converter.from_bytes(a_version));
        #else
        lexer.character_stream->Read(a_version);
        #endif

        lexer.Scan();
        Token current_token = Token(TokenType::StartOfStream);

        while (current_token.type == TokenType::StartOfStream)
        {
            current_token = lexer.Consume(0);
        }

        std::vector<int> digits;

        while (current_token.type != TokenType::EndOfStream)
        {
            switch (current_token.type)
            {
                case TokenType::Number:
                {
                    digits.push_back(std::stoi(current_token.content));
                    break;
                }
                default:
                {
                    break;
                }
            }

            current_token = lexer.Consume(0);
        }

        return digits;
    }
    
    bool Version::IsNewer(std::string a_version)
    {
        std::vector<int> version_digits = ToDigits(a_version);

        for (int i = 0; i < 3; ++i)
        {
            if (version_digits.at(i) > current_version_digits.at(i))
            {
                return true;
            }
        }

        return false;
    }
}