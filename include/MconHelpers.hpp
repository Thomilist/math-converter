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

// Clipboard
#include "../clip/clip.h"

// Definitions and forward declarations
#include "Definitions.hpp"
#include "ForwardDeclarations.hpp"

// OS-specific libraries
#ifdef WIN32
#include <windows.h>
#else
// https://git.yoctoproject.org/libfakekey/tree/src/libfakekey.c
#include "libfakekey.h"
#include <X11/Xatom.h>
#undef min // min macro from Xlibint.h, included by fakekey.h, causes errors in fstream.tcc
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
#include "parsing/unicodemath/UnicodeMathParser.hpp"
#include "parsing/unicodemath/UnicodeMathGenerator.hpp"

// Definitions
#define MCON_HOTKEY_SEND 1

namespace mcon
{
    void SendInputString(String a_string);
    void SetClipboardString(String a_string);
    void MathConversionHotkey(std::shared_ptr<Settings> a_settings, std::shared_ptr<std::mutex> a_settings_mutex);
    void ConfigInput(std::shared_ptr<Settings> a_settings, std::shared_ptr<std::mutex> a_settings_mutex);
    void ConvertMath(std::shared_ptr<Settings> a_settings, std::shared_ptr<std::mutex> a_settings_mutex, std::shared_ptr<ParsingTree> a_parsing_tree);
    
    #ifndef WIN32
    void ChangeKeyGrab(Display* a_display, Window a_window, bool a_grab, unsigned int a_keycode, unsigned int a_modifiers, unsigned int a_ignored_modifiers);
    #endif
}

#endif // __MCONHELPERS_H__