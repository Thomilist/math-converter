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
#include <windows.h>

// Custom headers
#include "include/CharacterStream.hpp"
#include "include/CharacterSet.hpp"
#include "include/Lexer.hpp"
#include "include/parsing/mathcad/MathcadParser.hpp"
#include "include/parsing/latex/LatexGenerator.hpp"
#include "include/parsing/intermediate/ParsingTree.hpp"

// Definitions
#define MCON_HOTKEY_SEND 1


int main()
{
    // Support wide characters
    //https://stackoverflow.com/a/50055280/17557793
    std::ios_base::sync_with_stdio(false);
    std::locale utf8( std::locale(), new std::codecvt_utf8_utf16<wchar_t> );
    std::wcout.imbue(utf8);

    auto character_stream = std::make_unique<mcon::CharacterStream>();
    auto character_set = std::make_shared<mcon::CharacterSet>();
    character_set->LoadFromFolder(".\\resources\\character-sets");
    auto lexer = std::make_unique<mcon::Lexer>(std::move(character_stream), character_set);
    auto mathcad_parser = std::make_unique<mcon::MathcadParser>(std::move(lexer));
    auto latex_generator = std::make_unique<mcon::LatexGenerator>(character_set);
    auto parsing_tree = std::make_shared<mcon::ParsingTree>(std::move(mathcad_parser), std::move(latex_generator));

    // Register hotkey ALT + G or print error if it fails to register
    if (RegisterHotKey(NULL, MCON_HOTKEY_SEND, MOD_CONTROL | MOD_SHIFT, 'V'))
    {
        std::wcout << L"Hotkey registered successfully." << std::endl;
    }
    else
    {
        std::wcerr << L"Hotkey registration error." << std::endl;
        return 1;
    }

    // Poll the message queue for hotkey triggers
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    MSG msg = {0};
    #pragma GCC diagnostic push
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        // Hotkeys trigger the WM_HOTKEY message with the hotkey ID in wParam
        if (msg.message == WM_HOTKEY)
        {
            parsing_tree->Reset();
            parsing_tree->parser->lexer->character_stream->ReadFromClipboard();
            parsing_tree->parser->lexer->Reset();
            parsing_tree->parser->lexer->Scan();
            parsing_tree->parser->Parse(parsing_tree);
            parsing_tree->parser->Clean(parsing_tree->root_node);
            parsing_tree->Clean(parsing_tree->root_node);
            parsing_tree->generator->Generate(parsing_tree);
            parsing_tree->generator->Substitute(parsing_tree);
            std::wcout << L"Input: " << parsing_tree->parser->lexer->character_stream->buffer << std::endl;
            std::wcout << L"Output: " << parsing_tree->output << std::endl;
            mcon::SendInputString(parsing_tree->output);
        }
    }

    return 0;
}