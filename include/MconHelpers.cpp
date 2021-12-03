#include "MconHelpers.hpp"


namespace mcon
{
    nlohmann::json LoadJSON(std::string a_file_path) 
    {
        std::ifstream json_file(a_file_path);

        if (!json_file.is_open())
        {
            throw std::runtime_error("Unable to open JSON file.");
        }

        nlohmann::json json_object;
        json_file >> json_object;

        json_file.close();

        return json_object;
    }
}
