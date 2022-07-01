#ifndef __MCONHELPERS_H__
#define __MCONHELPERS_H__

// Standard libraries
#include <string>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <mutex>

// JSON header
#include <nlohmann/json.hpp>

// Definitions and forward declarations
#include "Definitions.hpp"
#include "ForwardDeclarations.hpp"

#ifdef WINDOWS
#include <windows.h>
#endif

// Custom headers
#include "CharacterStream.hpp"
#include "CharacterSet.hpp"
#include "Lexer.hpp"
#include "Settings.hpp"
#include "parsing/Parser.hpp"
#include "parsing/Generator.hpp"
#include "parsing/mathcad/MathcadParser.hpp"
#include "parsing/mathcad/MathcadGenerator.hpp"
#include "parsing/latex/LatexParser.hpp"
#include "parsing/latex/LatexGenerator.hpp"
#include "parsing/mathml/MathMLParser.hpp"
#include "parsing/mathml/MathMLGenerator.hpp"

// Definitions
#define MCON_HOTKEY_SEND 1

namespace mcon
{
    void SendInputString(String a_string);
    void SetClipboardString(String a_string);
    void MathConversionHotkey(std::shared_ptr<Settings> a_settings, std::shared_ptr<std::mutex> a_settings_mutex);
    void ConfigInput(std::shared_ptr<Settings> a_settings, std::shared_ptr<std::mutex> a_settings_mutex);
}

#endif // __MCONHELPERS_H__