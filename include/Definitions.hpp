#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#include <iostream>

namespace mcon
{
    #ifdef WIN32
        using String = std::wstring;
        #define STRING_INPUT std::wcin
        #define STRING_OUTPUT std::wcout
        #define ERROR_OUTPUT std::wcerr
        #define INPUT_FILE_STREAM std::wifstream
        #define OUTPUT_FILE_STREAM std::wofstream
        #define STR(s) L##s
    #else
        using String = std::string;
        #define STRING_INPUT std::cin
        #define STRING_OUTPUT std::cout
        #define ERROR_OUTPUT std::cerr
        #define INPUT_FILE_STREAM std::ifstream
        #define OUTPUT_FILE_STREAM std::ofstream
        #define STR(s) s
    #endif
    
    #define MCON_VERSION              "1.5.3"
    #define MCON_VERSION_WRAPPED STR( "1.5.3" )
}

#endif // __DEFINITIONS_H__