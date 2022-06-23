#ifndef __PARSER_H__
#define __PARSER_H__

// Standard libraries
#include <string>
#include <memory>

// Definitions and forward declarations
#include "../Definitions.hpp"
#include "../ForwardDeclarations.hpp"

// Custom headers
#include "../Lexer.hpp"
#include "intermediate/Node.hpp"
//#include "intermediate/ParsingTree.hpp"
#include "intermediate/NodeTypes.hpp"

namespace mcon
{
    class Parser
    {
        public:
            Parser(std::shared_ptr<Lexer> a_lexer);
            ~Parser();

            virtual void Parse(std::shared_ptr<ParsingTree> a_parsing_tree);
            virtual void Clean(std::shared_ptr<Node> a_node);

            std::shared_ptr<Lexer> lexer;

        private:
            
    };
}

#endif // __PARSER_H__