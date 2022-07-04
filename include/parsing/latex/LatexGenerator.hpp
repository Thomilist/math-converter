#ifndef __LATEXGENERATOR_H__
#define __LATEXGENERATOR_H__

// Standard libraries
#include <string>
#include <memory>
#include <unordered_map>

// Definitions and forward declarations
#include "../../Definitions.hpp"
#include "../../ForwardDeclarations.hpp"

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
            const std::unordered_map<String, String>& GetSubstitutionList();
            String ApplyTemplates(std::shared_ptr<Node> a_node);
            String GenerateMatrix(std::shared_ptr<Node> a_node);
            String GenerateCompositeText(std::shared_ptr<Node> a_node);
            String FormatComplexNumber(String a_number);
            
        
        private:

            const std::unordered_map<NodeType, String> math_templates =
            {
                // LaTeX expression placeholders are denoted #i, with #0 being the first
                // Provide a string containing only a space to use the content of a node as-is
                // Provide an empty string to not use the content of a node at all
                {NodeType::Void,                STR("")},
                {NodeType::Number,              STR(" ")},
                {NodeType::Text,                STR(" ")},
                {NodeType::Label,               STR("{#1}")},
                {NodeType::Parentheses,         STR("\\left({#0}\\right)")},
                {NodeType::EqualityEvaluation,  STR("{#0}={#1}")},
                {NodeType::EqualityComparison,  STR("{#0}={#1}")},
                {NodeType::EqualityDefinition,  STR("{#0}:={#1}")},
                {NodeType::NotEqual,            STR("{#0}\\neq{#1}")},
                {NodeType::SymbolicEvaluation,  STR("{#0}\\xrightarrow{#1}{#2}")},
                {NodeType::Definition,          STR("{#0}\\equiv{#1}")},
                {NodeType::KeywordStack,        STR("{#0}")},
                {NodeType::Separator,           STR("{#0},\\:{#1}")},
                {NodeType::Addition,            STR("{#0}+{#1}")},
                {NodeType::Subtraction,         STR("{#0}-{#1}")},
                {NodeType::Multiplication,      STR("{#0}\\cdot{#1}")},
                {NodeType::Division,            STR("\\frac{#0}{#1}")},
                {NodeType::ScaledField,         STR("{#0}{\\:\\mathrm{#1}}")},
                {NodeType::ScaledResult,        STR("{#0}{\\:\\mathrm{#1}}")},
                {NodeType::Radix,               STR("\\sqrt[#0]{#1}")},
                {NodeType::Exponentiation,      STR("{#0}^{#1}")},
                {NodeType::TextSubscript,       STR("_{#0}")},
                {NodeType::Factorial,           STR("{#0}!")},
                {NodeType::LessThan,            STR("{#0}<{#1}")},
                {NodeType::GreaterThan,         STR("{#0}>{#1}")},
                {NodeType::LessThanOrEqual,     STR("{#0}\\leq{#1}")},
                {NodeType::GreaterThanOrEqual,  STR("{#0}\\geq{#1}")},
                {NodeType::Absolute,            STR("|{#0}|")},
                {NodeType::VectorProduct,       STR("{#0}\\times{#1}")},
                {NodeType::Range,               STR("{#0}\\:..\\:{#1}")},
                {NodeType::StepRange,           STR("{#0},\\:{#1}\\:..\\:{#2}")},
                {NodeType::Arguments,           STR("\\left({#0}\\right)")},
                {NodeType::Function,            STR("\\mathrm{#0}{#1}")},
                {NodeType::Negative,            STR("-{#0}")},
                {NodeType::LogicalNOT,          STR("\\neg{#0}")},
                {NodeType::LogicalAND,          STR("{#0}\\land{#1}")},
                {NodeType::LogicalOR,           STR("{#0}\\lor{#1}")},
                {NodeType::Sum,                 STR("\\displaystyle\\sum_{#0}^{#1}{#2}")},
                {NodeType::Product,             STR("\\displaystyle\\prod_{#0}^{#1}{#2}")},
                {NodeType::Polar,               STR("{#0}\\angle{#1}")},
                {NodeType::Derivative,          STR("{\\frac{d^{#1}}{d{#0}^{#1}}}{#2}")},
                {NodeType::Integral,            STR("\\displaystyle\\int_{#0}^{#1}{#2}d{#3}")},
                {NodeType::Percent,             STR("{#0}\\%")},
                {NodeType::ElementOf,           STR("{#0}\\in{#1}")},
                {NodeType::Degree,              STR("{#0}^{\\circ}")},
            };

            const std::unordered_map<String, String> substitution_list =
            {
                // Redundant code cleanup
                {STR("{\\:\\mathrm{}}"), STR("")},

                // Special characters
                {STR("∞"), STR("\\infty")},
                {STR("{\\:\\mathrm{{deg}}}"), STR("^{\\circ}")},
                {STR("{\\:\\mathrm{deg}}"), STR("^{\\circ}")},

                // Trigonometric functions
                {STR("\\mathrm{sin}"), STR("\\sin")},
                {STR("\\mathrm{cos}"), STR("\\cos")},
                {STR("\\mathrm{tan}"), STR("\\tan")},
                {STR("\\mathrm{cot}"), STR("\\cot")},
                {STR("\\mathrm{asin}"), STR("\\arcsin")},
                {STR("\\mathrm{acos}"), STR("\\arccos")},
                {STR("\\mathrm{atan}"), STR("\\arctan")},
                {STR("\\mathrm{acot}"), STR("\\arccot")},
                {STR("\\mathrm{sinh}"), STR("\\sinh")},
                {STR("\\mathrm{cosh}"), STR("\\cosh")},
                {STR("\\mathrm{tanh}"), STR("\\tanh")},
                {STR("\\mathrm{coth}"), STR("\\coth")},
                {STR("\\mathrm{sec}"), STR("\\sec")},
                {STR("\\mathrm{csc}"), STR("\\csc")},
                
                // Logarithm
                {STR("\\mathrm{log}"), STR("\\log")},
                {STR("\\mathrm{ln}"), STR("\\ln")},

                // Greek letters
                {STR("α"), STR("{\\alpha}")},
                {STR("Α"), STR("{A}")},
                {STR("β"), STR("{\\beta}")},
                {STR("Β"), STR("{B}")},
                {STR("γ"), STR("{\\gamma}")},
                {STR("Γ"), STR("{\\Gamma}")},
                {STR("δ"), STR("{\\delta}")},
                {STR("Δ"), STR("{\\Delta}")},
                {STR("ε"), STR("{\\varepsilon}")},
                {STR("Ε"), STR("{E}")},
                {STR("ζ"), STR("{\\zeta}")},
                {STR("Ζ"), STR("{Z}")},
                {STR("η"), STR("{\\eta}")},
                {STR("Η"), STR("{H}")},
                {STR("θ"), STR("{\\theta}")},
                {STR("Θ"), STR("{\\Theta}")},
                {STR("ϑ"), STR("{\\vartheta}")},
                {STR("ι"), STR("{\\iota}")},
                {STR("Ι"), STR("{I}")},
                {STR("κ"), STR("{\\kappa}")},
                {STR("Κ"), STR("{K}")},
                {STR("λ"), STR("{\\lambda}")},
                {STR("Λ"), STR("{\\Lambda}")},
                {STR("μ"), STR("{\\mu}")},
                {STR("Μ"), STR("{M}")},
                {STR("ν"), STR("{\\nu}")},
                {STR("Ν"), STR("{N}")},
                {STR("ξ"), STR("{\\xi}")},
                {STR("Ξ"), STR("{\\Xi}")},
                {STR("ο"), STR("{o}")},
                {STR("Ο"), STR("{O}")},
                {STR("π"), STR("{\\pi}")},
                {STR("Π"), STR("{\\Pi}")},
                {STR("ρ"), STR("{\\rho}")},
                {STR("Ρ"), STR("{P}")},
                {STR("σ"), STR("{\\sigma}")},
                {STR("Σ"), STR("{\\Sigma}")},
                {STR("τ"), STR("{\\tau}")},
                {STR("Τ"), STR("{T}")},
                {STR("υ"), STR("{\\upsilon}")},
                {STR("Υ"), STR("{\\Upsilon}")},
                {STR("ϕ"), STR("{\\phi}")},
                {STR("Φ"), STR("{\\Phi}")},
                {STR("φ"), STR("{\\varphi}")},
                {STR("χ"), STR("{\\chi}")},
                {STR("Χ"), STR("{X}")},
                {STR("ψ"), STR("{\\psi}")},
                {STR("Ψ"), STR("{\\Psi}")},
                {STR("ω"), STR("{\\omega}")},
                {STR("Ω"), STR("{\\Omega}")},
            };
    };
}

#endif // __LATEXGENERATOR_H__