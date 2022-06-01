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

// Custom headers
#include "include/CharacterStream.hpp"
#include "include/CharacterSet.hpp"
#include "include/Lexer.hpp"
#include "include/parsing/mathcad/MathcadParser.hpp"
#include "include/parsing/latex/LatexGenerator.hpp"
#include "include/parsing/intermediate/ParsingTree.hpp"


int main()
{
    auto character_stream = std::make_unique<mcon::CharacterStream>();
    character_stream->ReadFromClipboard();
    
    auto character_set = std::make_shared<mcon::CharacterSet>();
    character_set->LoadFromFolder(".\\resources\\character-sets");

    auto lexer = std::make_unique<mcon::Lexer>(std::move(character_stream), character_set);
    lexer->Scan();

    // Print tokens from lexer (for debugging)
    mcon::Token t(mcon::TokenType::OutOfBounds);
    do
    {
        t = lexer->Consume(0);
        std::cout << "\'" << t.content;
    } while (t.type != mcon::TokenType::EndOfStream);
    std::cout << "\'\n";
    lexer->Reset();
    
    auto mathcad_parser = std::make_unique<mcon::MathcadParser>(std::move(lexer));
    auto latex_generator = std::make_unique<mcon::LatexGenerator>(character_set);

    auto parsing_tree = std::make_shared<mcon::ParsingTree>(std::move(mathcad_parser), std::move(latex_generator));

    parsing_tree->parser->Parse(parsing_tree);
    parsing_tree->generator->Generate(parsing_tree);
    std::cout << parsing_tree->output << "\n";

    return 0;
}