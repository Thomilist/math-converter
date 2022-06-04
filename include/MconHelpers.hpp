#ifndef __MCONHELPERS_H__
#define __MCONHELPERS_H__

// Standard libraries
#include <string>
#include <iostream>
#include <stdexcept>
#include <windows.h>
#include <memory>
#include <mutex>

// JSON header
#include <nlohmann/json.hpp>

// Custom headers
#include "CharacterStream.hpp"
#include "CharacterSet.hpp"
#include "Lexer.hpp"
#include "parsing/mathcad/MathcadParser.hpp"
#include "parsing/latex/LatexGenerator.hpp"
#include "Settings.hpp"

// Definitions
#define MCON_HOTKEY_SEND 1

namespace mcon
{
    class CharacterSet;
    class CharacterStream;
    class Token;
    class Node;
    class Lexer;
    class Parser;
    class Generator;
    
    void SendInputString(std::wstring str);
    void MathConversionHotkey(std::shared_ptr<Settings> a_settings, std::shared_ptr<std::mutex> a_settings_mutex);
    void ConfigInput(std::shared_ptr<Settings> a_settings, std::shared_ptr<std::mutex> a_settings_mutex);
}

#endif // __MCONHELPERS_H__