/*
math-converter
version 0.1.0
https://github.com/Thomilist/math-converter

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2021 Thomas Emil Jensen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// Standard libraries
#include <iostream>
#include <memory>
#include <fcntl.h>
#include <thread>
#include <mutex>

// Forward declarations
#include "include/Definitions.hpp"
#include "include/ForwardDeclarations.hpp"

// OS-specific libraries
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif

// Custom headers
#include "include/MconHelpers.hpp"
#include "include/Settings.hpp"
#include "include/Version.hpp"

int main()
{
    #ifdef WIN32
    // Support wide characters
    //https://stackoverflow.com/a/50055280/17557793
    std::ios_base::sync_with_stdio(false);
    std::locale utf8(std::locale(), new std::codecvt_utf8_utf16<wchar_t>);
    std::wcout.imbue(utf8);
    #endif

    // Version output and check
    STRING_OUTPUT << STR("math-converter-") << MCON_VERSION_WRAPPED << std::endl;

    try
    {
        auto version = mcon::Version(MCON_VERSION);
        version.Check();
    }
    catch(const std::exception& e)
    {
        ERROR_OUTPUT
            << "An error occured while checking for an updated version.\n"
            << std::endl;
    }

    // Settings initialisation
    auto settings_mutex = std::make_shared<std::mutex>();
    auto settings = std::make_shared<mcon::Settings>();

    // hotkey_thread to catch hotkey triggers and convert math expressions
    // config_thread to handle input from the console
    std::thread hotkey_thread(mcon::MathConversionHotkey, settings, settings_mutex);
    std::thread config_thread(mcon::ConfigInput, settings, settings_mutex);
    hotkey_thread.join();
    config_thread.join();

    return 0;
}