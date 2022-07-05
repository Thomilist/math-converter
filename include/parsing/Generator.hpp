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
            virtual ~Generator();
            
            void Generate(std::shared_ptr<ParsingTree> a_parsing_tree);
            String ApplyTemplates(std::shared_ptr<Node> a_node);

            std::shared_ptr<CharacterSet> character_set;

        private:
            void Substitute(std::shared_ptr<ParsingTree> a_parsing_tree);
            Lexer LoadMathTemplate(std::shared_ptr<Node> a_node);
            bool FoundMathPlaceholder(Lexer& a_lexer);
            String ProcessMathPlaceholder(Lexer& a_lexer, std::shared_ptr<Node> a_node);
            String ProcessNonPlaceholder(Lexer& a_lexer, std::shared_ptr<Node> a_node);
            virtual String GenerateMatrix(std::shared_ptr<Node> a_node);
            virtual String GenerateCompositeText(std::shared_ptr<Node> a_node);
            virtual const std::unordered_map<NodeType, String>& GetMathTemplates();
            virtual const std::unordered_map<String, String>& GetSubstitutionList();

            const std::unordered_map<NodeType, String> math_templates;
            const std::unordered_map<String, String> substitution_list;
    };
}

#endif // __GENERATOR_H__