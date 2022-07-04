#ifndef __UNICODEMATHGENERATOR_H__
#define __UNICODEMATHGENERATOR_H__

// Standard libraries
#include <string>
#include <memory>

// Definitions and forward declarations
#include "../../Definitions.hpp"
#include "../../ForwardDeclarations.hpp"

// Custom headers
#include "../Generator.hpp"

namespace mcon
{
    class UnicodeMathGenerator : public Generator
    {
        public:
            UnicodeMathGenerator(std::shared_ptr<CharacterSet> a_character_set);
            ~UnicodeMathGenerator();

            void Generate(std::shared_ptr<ParsingTree> a_parsing_tree);
            const std::unordered_map<String, String>& GetSubstitutionList();
            String ApplyTemplates(std::shared_ptr<Node> a_node);
            String GenerateMatrix(std::shared_ptr<Node> a_node);
            String GenerateCompositeText(std::shared_ptr<Node> a_node);

        private:
            const std::unordered_map<NodeType, String> math_templates =
            {
                {NodeType::Void,                STR("")},
                {NodeType::Number,              STR(" ")},
                {NodeType::Text,                STR(" ")},
                {NodeType::Label,               STR("#1")},
                {NodeType::Parentheses,         STR("\\open(#0\\close)")},
                {NodeType::EqualityEvaluation,  STR("#0=#1")},
                {NodeType::EqualityComparison,  STR("#0=#1")},
                {NodeType::EqualityDefinition,  STR("#0:=#1")},
                {NodeType::NotEqual,            STR("#0/=#1")},
                {NodeType::SymbolicEvaluation,  STR("#0->\\above #1 #2")},
                {NodeType::Definition,          STR("#0\\equiv #1")},
                {NodeType::KeywordStack,        STR("#0")},
                {NodeType::Separator,           STR("(#0, #1) ")},
                {NodeType::Addition,            STR("#0+#1")},
                {NodeType::Subtraction,         STR("#0-#1")},
                {NodeType::Multiplication,      STR("#0\\bullet #1")},
                {NodeType::Division,            STR("(#0)/(#1)")},
                {NodeType::ScaledField,         STR("#0 #1")},
                {NodeType::ScaledResult,        STR("#0 #1")},
                {NodeType::Radix,               STR("\\sqrt (#0&#1)")},
                {NodeType::Exponentiation,      STR("#0^(#1)")},
                {NodeType::TextSubscript,       STR("#0_(#1) ")},
                {NodeType::Factorial,           STR("#0!")},
                {NodeType::LessThan,            STR("#0<#1")},
                {NodeType::GreaterThan,         STR("#0>#1")},
                {NodeType::LessThanOrEqual,     STR("#0\\le #1")},
                {NodeType::GreaterThanOrEqual,  STR("#0\\ge #1")},
                {NodeType::Absolute,            STR("|#0|")},
                {NodeType::VectorProduct,       STR("#0\\times #1")},
                {NodeType::Range,               STR("#0.. #1")},
                {NodeType::StepRange,           STR("#0, #1.. #2")},
                {NodeType::Arguments,           STR("\\open(#0\\close)")},
                {NodeType::Function,            STR("#0 #1")},
                {NodeType::Negative,            STR("-#0")},
                {NodeType::LogicalNOT,          STR("\\neg #0")},
                {NodeType::LogicalAND,          STR("#0\\wedge #1")},
                {NodeType::LogicalOR,           STR("#0\\vee #1")},
                {NodeType::Sum,                 STR("\\sum _(#0)^(#1)\\of #2")},
                {NodeType::Product,             STR("\\prod _(#0)^(#1)\\of #2")},
                {NodeType::Polar,               STR("#0\\angle #1")},
                {NodeType::Derivative,          STR("(d^(#1))/(d#0^(#1) ) #2")},
                {NodeType::Integral,            STR("\\int _(#0)^(#1)\\of #2 d#3")},
                {NodeType::Percent,             STR("#0%")},
                {NodeType::ElementOf,           STR("#0\\in #1")},
                {NodeType::Degree,              STR("#0\\degree ")},
            };

            const std::unordered_map<String, String> substitution_list =
            {
                // Escape sequences
                {STR("\\#"), STR("#")},

                // Special characters
                {STR("∞"), STR("\\infty")},
            };
    };
}

#endif // __UNICODEMATHGENERATOR_H__