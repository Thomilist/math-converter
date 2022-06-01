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
            std::wstring Substitute(std::shared_ptr<Node> a_node);

            std::shared_ptr<CharacterSet> character_set;
        
        private:

            const std::unordered_map<NodeType, std::wstring> math_operators =
            {
                {NodeType::Void, L""},
                {NodeType::Number, L" "},
                {NodeType::Text, L" "},
                {NodeType::Label, L"{#1}"},
                {NodeType::Parentheses, L"\\left({#0}\\right)"},
                {NodeType::EqualityEvaluation, L"{#0}={#1}"},
                {NodeType::EqualityComparison, L"{#0}={#1}"},
                {NodeType::EqualityDefinition, L"{#0}:={#1}"},
                {NodeType::Addition, L"{#0}+{#1}"},
                {NodeType::Subtraction, L"{#0}-{#1}"},
                {NodeType::Multiplication, L"{#0}\\cdot{#1}"},
                {NodeType::Division, L"\\frac{#0}{#1}"},
                {NodeType::Unit, L"{#0}{\\;\\mathrm{#1}}"},
                {NodeType::Root, L"\\sqrt[#0]{#1}"},
            };
    };
}

#endif // __LATEXGENERATOR_H__