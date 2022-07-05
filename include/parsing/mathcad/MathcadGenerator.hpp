#ifndef __MATHCADGENERATOR_H__
#define __MATHCADGENERATOR_H__

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
    class MathcadGenerator : public Generator
    {
        public:
            MathcadGenerator(std::shared_ptr<CharacterSet> a_character_set);
            ~MathcadGenerator();

        private:
            const std::unordered_map<NodeType, String>& GetMathTemplates();
            const std::unordered_map<String, String>& GetSubstitutionList();
            String GenerateMatrix(std::shared_ptr<Node> a_node);
            String GenerateCompositeText(std::shared_ptr<Node> a_node);
            
            const std::unordered_map<NodeType, String> math_templates =
            {
                {NodeType::Void,                STR(" ")},
                {NodeType::Number,              STR(" ")},
                {NodeType::Text,                STR(" ")},
                {NodeType::Label,               STR("(@LABEL #0 #1)")},
                {NodeType::Parentheses,         STR("(@PARENS #0)")},
                {NodeType::EqualityEvaluation,  STR("(= #0 #1)")},
                {NodeType::EqualityComparison,  STR("(@EQ #0 #1)")},
                {NodeType::EqualityDefinition,  STR("(:= #0 #1)")},
                {NodeType::NotEqual,            STR("(@NEQ #0 #1)")},
                {NodeType::SymbolicEvaluation,  STR("(@SYM_EVAL #0 #1 #2)")},
                {NodeType::Definition,          STR("(@GLOBAL_DEF #0 #1)")},
                {NodeType::KeywordStack,        STR("(@KW_STACK #0)")},
                {NodeType::Separator,           STR("(@SEP #0 #1)")},
                {NodeType::Addition,            STR("(+ #0 #1)")},
                {NodeType::Subtraction,         STR("(- #0 #1)")},
                {NodeType::Multiplication,      STR("(* #0 #1)")},
                {NodeType::Division,            STR("(/ #0 #1)")},
                {NodeType::ScaledField,         STR("(@SCALE #0 #1)")},
                {NodeType::ScaledResult,        STR("(@RSCALE #0 #1)")},
                {NodeType::Radix,               STR("(@NTHROOT #0 #1)")},
                {NodeType::Exponentiation,      STR("(^ #0 #1)")},
                {NodeType::TextComposite,       STR("(@ID #0 #1)")},
                {NodeType::TextSubscript,       STR("(@SUB #0)")},
                {NodeType::Factorial,           STR("(! #0)")},
                {NodeType::LessThan,            STR("(< #0 #1)")},
                {NodeType::GreaterThan,         STR("(> #0 #1)")},
                {NodeType::LessThanOrEqual,     STR("(@LEQ #0 #1)")},
                {NodeType::GreaterThanOrEqual,  STR("(@GEQ #0 #1)")},
                {NodeType::Absolute,            STR("(@ABS #0)")},
                {NodeType::VectorProduct,       STR("(@CROSS #0 #1)")},
                {NodeType::Range,               STR("(@RANGE #0 #1)")},
                {NodeType::StepRange,           STR("(@STEPRANGE #0 #1 #2)")},
                {NodeType::Arguments,           STR("(@ARGS #0)")},
                {NodeType::Function,            STR("(@APPLY #0 #1)")},
                {NodeType::Negative,            STR("(@NEG #0)")},
                {NodeType::LogicalNOT,          STR("(@NOT #0)")},
                {NodeType::LogicalAND,          STR("(@AND #0 #1)")},
                {NodeType::LogicalOR,           STR("(@OR #0 #1)")},
                {NodeType::Sum,                 STR("(@SUM #0 #1 #2)")},
                {NodeType::Product,             STR("(@PRODUCT #0 #1 #2)")},
                {NodeType::Polar,               STR("(@POLAR #0 #1)")},
                {NodeType::Derivative,          STR("(@DERIV #0 #1 #2)")},
                {NodeType::Integral,            STR("(@INTEGRAL #0 #1 #2 #3)")},
                {NodeType::Percent,             STR("(% #0)")},
                {NodeType::ElementOf,           STR("(@ELEMENT_OF #0 #1)")},
                {NodeType::Degree,              STR("(@DEG #0)")},
                {NodeType::MatrixRowCount,      STR(" ")},
                {NodeType::MatrixCollumnCount,  STR(" ")},
            };

            const std::unordered_map<String, String> substitution_list =
            {
                // Escape sequences
                {STR("\\#"), STR("#")},
                {STR("\\_"), STR("_")},
                {STR("\\&"), STR("&")},
            };
    };
}

#endif // __MATHCADGENERATOR_H__