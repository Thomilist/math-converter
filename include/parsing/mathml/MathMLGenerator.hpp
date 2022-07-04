#ifndef __MATHMLGENERATOR_H__
#define __MATHMLGENERATOR_H__

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
    class MathMLGenerator : public Generator
    {
        public:
            MathMLGenerator(std::shared_ptr<CharacterSet> a_character_set);
            ~MathMLGenerator();

            void Generate(std::shared_ptr<ParsingTree> a_parsing_tree);
            const std::unordered_map<String, String>& GetSubstitutionList();
            String ApplyTemplates(std::shared_ptr<Node> a_node);
            String GenerateMatrix(std::shared_ptr<Node> a_node);
            String GenerateCompositeText(std::shared_ptr<Node> a_node);

        private:
            const std::unordered_map<NodeType, String> math_templates =
            {
                {NodeType::Void,                STR("")},
                {NodeType::Number,              STR("<mn>#0</mn>")},
                {NodeType::Text,                STR("<mi mathvariant=\"italic\">#0</mi>")},
                {NodeType::Label,               STR("#1")},
                {NodeType::Parentheses,         STR("<mrow><mo>&lpar;</mo>#0<mo>&rpar;</mo></mrow>")},
                {NodeType::EqualityEvaluation,  STR("<mrow>#0<mo>&equals;</mo>#1</mrow>")},
                {NodeType::EqualityComparison,  STR("<mrow>#0<mo>&equals;</mo>#1</mrow>")},
                {NodeType::EqualityDefinition,  STR("<mrow>#0<mo>&Assign;</mo>#1</mrow>")},
                {NodeType::NotEqual,            STR("<mrow>#0<mo>&ne;</mo>#1</mrow>")},
                {NodeType::SymbolicEvaluation,  STR("#0<mover><mo>&RightArrow;</mo>#1</mover>#2")},
                {NodeType::Definition,          STR("<mrow>#0<mo>&equiv;</mo>#1</mrow>")},
                {NodeType::KeywordStack,        STR("#0")},
                {NodeType::Separator,           STR("<mrow>#0<mi>,&ensp;</mi>#1</mrow>")},
                {NodeType::Addition,            STR("<mrow>#0<mo>&plus;</mo>#1</mrow>")},
                {NodeType::Subtraction,         STR("<mrow>#0<mo>&minus;</mo>#1</mrow>")},
                {NodeType::Multiplication,      STR("<mrow>#0<mo>&sdot;</mo>#1</mrow>")},
                {NodeType::Division,            STR("<mfrac>#0 #1</mfrac>")},
                {NodeType::ScaledField,         STR("<mrow>#0<mi>&ensp;</mi><mi mathvariant=\"normal\">#1</mi></mrow>")},
                {NodeType::ScaledResult,        STR("<mrow>#0<mi>&ensp;</mi><mi mathvariant=\"normal\">#1</mi></mrow>")},
                {NodeType::Radix,               STR("<mroot>#1 #0</mroot>")},
                {NodeType::Exponentiation,      STR("<msup>#0 #1</msup>")},
                {NodeType::TextSubscript,       STR("<msub>#0 #1</msub>")},
                {NodeType::Factorial,           STR("<mrow>#0<mo>!</mo></mrow>")},
                {NodeType::LessThan,            STR("<mrow>#0<mo>&lt;</mo>#1</mrow>")},
                {NodeType::GreaterThan,         STR("<mrow>#0<mo>&gt;</mo>#1</mrow>")},
                {NodeType::LessThanOrEqual,     STR("<mrow>#0<mo>&leq;</mo>#1</mrow>")},
                {NodeType::GreaterThanOrEqual,  STR("<mrow>#0<mo>&geq;</mo>#1</mrow>")},
                {NodeType::Absolute,            STR("<mrow><mo>|</mo>#0<mo>|</mo></mrow>")},
                {NodeType::VectorProduct,       STR("<mrow>#0<mo>&Cross;</mo>#1</mrow>")},
                {NodeType::Range,               STR("<mrow>#0<mi>..&ensp;</mi>#1</mrow>")},
                {NodeType::StepRange,           STR("<mrow>#0<mi>,&ensp;</mi>#1<mi>..&ensp;</mi>#2</mrow>")},
                {NodeType::Arguments,           STR("<mrow><mo>&lpar;</mo>#0<mo>&rpar;</mo></mrow>")},
                {NodeType::Function,            STR("<mrow><mi>#0</mi>#1</mrow>")},
                {NodeType::Negative,            STR("<mrow><mo>&minus;</mo>#0</mrow>")},
                {NodeType::LogicalNOT,          STR("<mrow><mo>&not;</mo>#0</mrow>")},
                {NodeType::LogicalAND,          STR("<mrow>#0<mo>&and;</mo>#1</mrow>")},
                {NodeType::LogicalOR,           STR("<mrow>#0<mo>&or;</mo>#1</mrow>")},
                {NodeType::Sum,                 STR("<mrow><munderover><mo>&Sigma;</mo>#0 #1</munderover>#2<mrow>")},
                {NodeType::Product,             STR("<mrow><munderover><mo>&Pi;</mo>#0 #1</munderover>#2<mrow>")},
                {NodeType::Polar,               STR("<mrow>#0<mo>&angle;</mo>#1</mrow>")},
                {NodeType::Derivative,          STR("<mrow><mfrac><msup><mi mathvariant=\"italic\">d</mi>#1</msup><msup><mrow><mi mathvariant=\"italic\">d</mi>#0</mrow>#1</msup></mfrac>#2</mrow>")},
                {NodeType::Integral,            STR("<mrow><munderover><mo>&int;</mo>#0 #1</munderover>#2<mi mathvariant=\"italic\">&ensp;d</mi>#3<mrow>")},
                {NodeType::Percent,             STR("<mrow>#0<mo>%</mo></mrow>")},
                {NodeType::ElementOf,           STR("<mrow>#0<mo>&Element;</mo>#1</mrow>")},
                {NodeType::Degree,              STR("<mrow>#0<mo>&deg;</mo></mrow>")},
            };

            const std::unordered_map<String, String> substitution_list =
            {
                // Escape sequences
                {STR("\\#"), STR("#")},
                {STR("\\&"), STR("&amp;")},
                {STR("\\_"), STR("_")},

                // Redundant code cleanup
                {STR("<mi>&ensp;</mi><mi mathvariant=\"normal\"></mi>"), STR("")},

                // Special characters
                {STR("∞"), STR("&#x221E;")},
            };
    };
}

#endif // __MATHMLGENERATOR_H__