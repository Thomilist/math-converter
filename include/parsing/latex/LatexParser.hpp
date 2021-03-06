#ifndef __LATEXPARSER_H__
#define __LATEXPARSER_H__

// Standard libraries
#include <string>
#include <memory>

// Definitions and forward declarations
#include "../../Definitions.hpp"
#include "../../ForwardDeclarations.hpp"

// Custom headers
#include "../Parser.hpp"

namespace mcon
{
    class LatexParser : public Parser
    {
        public:
            LatexParser(std::unique_ptr<Lexer> a_lexer);
            ~LatexParser();

            virtual void Parse(std::shared_ptr<ParsingTree> a_parsing_tree);

            std::unique_ptr<Lexer> lexer;
        
        private:

    };
}

#endif // __LATEXPARSER_H__