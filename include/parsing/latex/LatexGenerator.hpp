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
            const std::unordered_map<std::wstring, std::wstring> substitution_list =
            {
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
                {L"H", L"{H}"},
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