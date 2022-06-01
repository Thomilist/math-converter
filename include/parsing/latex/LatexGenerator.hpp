#ifndef __LATEXGENERATOR_H__
#define __LATEXGENERATOR_H__

// Standard libraries
#include <string>
#include <memory>
#include <unordered_map>

// Custom headers
#include "../Generator.hpp"

namespace mcon
{
    class LatexGenerator : public Generator
    {
        public:
            LatexGenerator(std::shared_ptr<CharacterSet> a_character_set);
            ~LatexGenerator();

            void Generate(std::shared_ptr<ParsingTree> a_parsing_tree);
            std::string Substitute(std::shared_ptr<Node> a_node);

            std::shared_ptr<CharacterSet> character_set;
        
        private:

            const std::unordered_map<NodeType, std::string> math_operators =
            {
                {NodeType::Void, ""},
                {NodeType::Number, " "},
                {NodeType::Text, " "},
                {NodeType::Label, "{#1}"},
                {NodeType::Parentheses, "\\left({#0}\\right)"},
                {NodeType::EqualityEvaluation, "{#0}={#1}"},
                {NodeType::EqualityComparison, "{#0}={#1}"},
                {NodeType::EqualityDefinition, "{#0}:={#1}"},
                {NodeType::Addition, "{#0}+{#1}"},
                {NodeType::Subtraction, "{#0}-{#1}"},
                {NodeType::Multiplication, "{#0}\\cdot{#1}"},
                {NodeType::Division, "\\frac{#0}{#1}"},
                {NodeType::Unit, "{#0}{\\;\\mathrm{#1}}"},
            };
    };
}

#endif // __LATEXGENERATOR_H__