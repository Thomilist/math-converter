#ifndef __MATHCADPARSER_H__
#define __MATHCADPARSER_H__

// Standard libraries
#include <string>
#include <memory>
#include <unordered_map>

// Custom headers
#include "../Parser.hpp"

namespace mcon
{
    enum class ParserState
    {
        LookingForExpression,
        IdentifyingOperator
    };
    
    class MathcadParser : public Parser
    {
        public:
            MathcadParser(std::unique_ptr<Lexer> a_lexer);
            ~MathcadParser();

            void Parse(std::shared_ptr<ParsingTree> a_parsing_tree);
            void Clean(std::shared_ptr<Node> a_node);
        
        private:
            ParserState state = ParserState::LookingForExpression;
            int depth = 0;

            const std::unordered_map<std::wstring, NodeType> math_operators = 
            {
                {L"@LABEL", NodeType::Label},
                {L"@PARENS", NodeType::Parentheses},
                {L"=", NodeType::EqualityEvaluation},
                {L"@EQ", NodeType::EqualityComparison},
                {L":=", NodeType::EqualityDefinition},
                {L"+", NodeType::Addition},
                {L"-", NodeType::Subtraction},
                {L"*", NodeType::Multiplication},
                {L"/", NodeType::Division},
                {L"@SCALE", NodeType::Unit},
                {L"@RSCALE", NodeType::Unit},
                {L"@RPLACEHOLDER", NodeType::Void},
                {L"@NTHROOT", NodeType::Root},
            };
    };
}

#endif // __MATHCADPARSER_H__