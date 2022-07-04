#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#include <iostream>

namespace mcon
{
    #ifdef WIN32
        using String = std::wstring;
        #define STRING_OUTPUT std::wcout
        #define ERROR_OUTPUT std::wcerr
        #define STR(s) L##s
    #else
        using String = std::string;
        #define STRING_OUTPUT std::cout
        #define ERROR_OUTPUT std::cerr
        #define STR(s) s
    #endif
    
    #define VERSION STR("1.5.0")
}

#endif // __DEFINITIONS_H__