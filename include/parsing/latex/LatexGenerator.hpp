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
            std::wstring ApplyTemplates(std::shared_ptr<Node> a_node);
            void Substitute(std::shared_ptr<ParsingTree> a_parsing_tree);

            std::shared_ptr<CharacterSet> character_set;
        
        private:

            const std::unordered_map<NodeType, std::wstring> math_operators =
            {
                // LaTeX expression placeholders are denoted #i, with #0 being the first
                // Provide a string containing only a space to use the content of a nose as-is
                // Provide an empty string to not use the content of a node at all
                {NodeType::Void,                L""},
                {NodeType::Number,              L" "},
                {NodeType::Text,                L" "},
                {NodeType::Label,               L"{#1}"},
                {NodeType::Parentheses,         L"\\left({#0}\\right)"},
                {NodeType::EqualityEvaluation,  L"{#0}={#1}"},
                {NodeType::EqualityComparison,  L"{#0}={#1}"},
                {NodeType::EqualityDefinition,  L"{#0}:={#1}"},
                {NodeType::SymbolicEvaluation,  L"{#0}\\xrightarrow{#1}{#2}"},
                {NodeType::Definition,          L"{#0}\\equiv{#1}"},
                {NodeType::KeywordStack,        L"{#0}"},
                {NodeType::Separator,           L"{#0},\\:{#1}"},
                {NodeType::Addition,            L"{#0}+{#1}"},
                {NodeType::Subtraction,         L"{#0}-{#1}"},
                {NodeType::Multiplication,      L"{#0}\\cdot{#1}"},
                {NodeType::Division,            L"\\frac{#0}{#1}"},
                {NodeType::Unit,                L"{#0}{\\:\\mathrm{#1}}"},
                {NodeType::Radix,               L"\\sqrt[#0]{#1}"},
                {NodeType::Exponentiation,      L"{#0}^{#1}"},
                {NodeType::TextComposite,       L"{#0}{#1}"},
                {NodeType::TextSubscript,       L"_{#0}"},
                {NodeType::Factorial,           L"{#0}!"},
                {NodeType::LessThan,            L"{#0}<{#1}"},
                {NodeType::GreaterThan,         L"{#0}>{#1}"},
                {NodeType::LessThanOrEqual,     L"{#0}\\leq{#1}"},
                {NodeType::GreaterThanOrEqual,  L"{#0}\\geq{#1}"},
                {NodeType::Absolute,            L"|{#0}|"},
                {NodeType::VectorProduct,       L"{#0}\\times{#1}"},
                {NodeType::Range,               L"{#0}\\:..\\:{#1}"},
                {NodeType::StepRange,           L"{#0},\\:{#1}\\:..\\:{#2}"},
                {NodeType::Arguments,           L"\\left({#0}\\right)"},
                {NodeType::Function,            L"\\mathrm{#0}{#1}"},
                {NodeType::Negative,            L"-{#0}"},
                {NodeType::LogicalNOT,          L"\\neg{#0}"},
                {NodeType::LogicalAND,          L"{#0}\\land{#1}"},
                {NodeType::LogicalOR,           L"{#0}\\lor{#1}"},
                {NodeType::Sum,                 L"\\displaystyle\\sum_{#0}^{#1}{#2}"},
                {NodeType::Product,             L"\\displaystyle\\prod_{#0}^{#1}{#2}"},
                {NodeType::Polar,               L"{#0}\\angle{#1}"},
                {NodeType::Derivative,          L"{\\frac{\\mathrm{d}^{#1}}{\\mathrm{d}{#0}^{#1}}}{#2}"},
                {NodeType::Integral,            L"\\displaystyle\\int_{#0}^{#1}{#2}\\mathrm{d}{#3}"},
                {NodeType::Percent,             L"{#0}\\%"},
                {NodeType::ElementOf,           L"{#0}\\in{#1}"},
                {NodeType::Degree,              L"{#0}^{\\circ}"},
            };

            const std::unordered_map<std::wstring, std::wstring> substitution_list =
            {
                // Redundant code cleanup
                {L"{\\:\\mathrm{}}", L""},

                // Special characters
                {L"∞", L"\\infty"},
                {L"{\\:\\mathrm{{deg}}}", L"^{\\circ}"},

                // Trigonometric functions
                {L"\\mathrm{sin}", L"\\sin"},
                {L"\\mathrm{cos}", L"\\cos"},
                {L"\\mathrm{tan}", L"\\tan"},
                {L"\\mathrm{cot}", L"\\cot"},
                {L"\\mathrm{asin}", L"\\arcsin"},
                {L"\\mathrm{acos}", L"\\arccos"},
                {L"\\mathrm{atan}", L"\\arctan"},
                {L"\\mathrm{acot}", L"\\arccot"},
                {L"\\mathrm{sinh}", L"\\sinh"},
                {L"\\mathrm{cosh}", L"\\cosh"},
                {L"\\mathrm{tanh}", L"\\tanh"},
                {L"\\mathrm{coth}", L"\\coth"},
                {L"\\mathrm{sec}", L"\\sec"},
                {L"\\mathrm{csc}", L"\\csc"},
                
                // Logarithm
                {L"\\mathrm{log}", L"\\log"},
                {L"\\mathrm{ln}", L"\\ln"},

                // Greek letters
                {L"α", L"{\\alpha}"},
                {L"Α", L"{A}"},
                {L"β", L"{\\beta}"},
                {L"Β", L"{B}"},
                {L"γ", L"{\\gamma}"},
                {L"Γ", L"{\\Gamma}"},
                {L"δ", L"{\\delta}"},
                {L"Δ", L"{\\Delta}"},
                {L"ε", L"{\\varepsilon}"},
                {L"Ε", L"{E}"},
                {L"ζ", L"{\\zeta}"},
                {L"Ζ", L"{Z}"},
                {L"η", L"{\\eta}"},
                //{L"H", L"{H}"},   // Identical characters left and right cause infinite loop
                {L"θ", L"{\\theta}"},
                {L"Θ", L"{\\Theta}"},
                {L"ϑ", L"{\\vartheta}"},
                {L"ι", L"{\\iota}"},
                {L"Ι", L"{I}"},
                {L"κ", L"{\\kappa}"},
                {L"Κ", L"{K}"},
                {L"λ", L"{\\lambda}"},
                {L"Λ", L"{\\Lambda}"},
                {L"μ", L"{\\mu}"},
                {L"Μ", L"{M}"},
                {L"ν", L"{\\nu}"},
                {L"Ν", L"{N}"},
                {L"ξ", L"{\\xi}"},
                {L"Ξ", L"{\\Xi}"},
                {L"ο", L"{o}"},
                {L"Ο", L"{O}"},
                {L"π", L"{\\pi}"},
                {L"Π", L"{\\Pi}"},
                {L"ρ", L"{\\rho}"},
                {L"Ρ", L"{P}"},
                {L"σ", L"{\\sigma}"},
                {L"Σ", L"{\\Sigma}"},
                {L"τ", L"{\\tau}"},
                {L"Τ", L"{T}"},
                {L"υ", L"{\\upsilon}"},
                {L"Υ", L"{\\Upsilon}"},
                {L"ϕ", L"{\\phi}"},
                {L"Φ", L"{\\Phi}"},
                {L"φ", L"{\\varphi}"},
                {L"χ", L"{\\chi}"},
                {L"Χ", L"{X}"},
                {L"ψ", L"{\\psi}"},
                {L"Ψ", L"{\\Psi}"},
                {L"ω", L"{\\omega}"},
                {L"Ω", L"{\\Omega}"},
            };
    };
}

#endif // __LATEXGENERATOR_H__