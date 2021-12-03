#ifndef __MCONHELPERS_H__
#define __MCONHELPERS_H__

// Standard libraries
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

// JSON header
#include <nlohmann\json.hpp>

namespace mcon
{
    nlohmann::json LoadJSON(std::string a_file_path);
}

#endif // __MCONHELPERS_H__