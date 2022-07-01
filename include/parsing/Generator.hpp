#ifndef __GENERATOR_H__
#define __GENERATOR_H__

// Standard libraries
#include <string>
#include <memory>

// Definitions and forward declarations
#include "../Definitions.hpp"
#include "../ForwardDeclarations.hpp"

// Custom headers
#include "../CharacterStream.hpp"
#include "../CharacterSet.hpp"
#include "../Lexer.hpp"
#include "intermediate/ParsingTree.hpp"
#include "intermediate/Node.hpp"
#include "intermediate/NodeTypes.hpp"

namespace mcon
{
    class Generator
    {
        public:
            Generator(std::shared_ptr<CharacterSet> a_character_set);
            ~Generator();
            
            virtual void Generate(std::shared_ptr<ParsingTree> a_parsing_tree);
            void Substitute(std::shared_ptr<ParsingTree> a_parsing_tree);
            virtual const std::unordered_map<String, String>& GetSubstitutionList();

            std::shared_ptr<CharacterSet> character_set;

        private:
            const std::unordered_map<String, String> substitution_list;
    };
}

#endif // __GENERATOR_H__