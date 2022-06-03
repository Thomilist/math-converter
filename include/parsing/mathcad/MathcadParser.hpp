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
            void ParseExpression(std::shared_ptr<ParsingTree> a_parsing_tree);
            void Clean(std::shared_ptr<Node> a_node);
            void EscapeCharacter(std::shared_ptr<Node> a_node, std::wstring a_find, std::wstring a_replace);
        
        private:
            ParserState state = ParserState::LookingForExpression;
            Token current_token = Token(TokenType::OutOfBounds);

            const std::unordered_map<std::wstring, NodeType> math_operators = 
            {
                {L"@LABEL",         NodeType::Label},
                {L"@PARENS",        NodeType::Parentheses},
                {L"=",              NodeType::EqualityEvaluation},
                {L"@EQ",            NodeType::EqualityComparison},
                {L":=",             NodeType::EqualityDefinition},
                {L"@NEQ",           NodeType::NotEqual},
                {L"@SYM_EVAL",      NodeType::SymbolicEvaluation},
                {L"@IS",            NodeType::EqualityComparison},
                {L"@GLOBAL_DEF",    NodeType::Definition},
                {L"@KW_STACK",      NodeType::KeywordStack},
                {L"@SEP",           NodeType::Separator},
                {L"+",              NodeType::Addition},
                {L"-",              NodeType::Subtraction},
                {L"*",              NodeType::Multiplication},
                {L"/",              NodeType::Division},
                {L"@SCALE",         NodeType::Unit},
                {L"@RSCALE",        NodeType::Unit},
                {L"@RPLACEHOLDER",  NodeType::Void},
                {L"@NTHROOT",       NodeType::Radix},
                {L"^",              NodeType::Exponentiation},
                {L"@ID",            NodeType::TextComposite},
                {L"@SUB",           NodeType::TextSubscript},
                {L"!",              NodeType::Factorial},
                {L"<",              NodeType::LessThan},
                {L">",              NodeType::GreaterThan},
                {L"@LEQ",           NodeType::LessThanOrEqual},
                {L"@GEQ",           NodeType::GreaterThanOrEqual},
                {L"@ABS",           NodeType::Absolute},
                {L"@CROSS",         NodeType::VectorProduct},
                {L"@RANGE",         NodeType::Range},
                {L"@STEPRANGE",     NodeType::StepRange},
                {L"@ARGS",          NodeType::Arguments},
                {L"@APPLY",         NodeType::Function},
                {L"@FUNCTION",      NodeType::Function},
                {L"@NEG",           NodeType::Negative},
                {L"@NOT",           NodeType::LogicalNOT},
                {L"@AND",           NodeType::LogicalAND},
                {L"@OR",            NodeType::LogicalOR},
                {L"@SUM",           NodeType::Sum},
                {L"@PRODUCT",       NodeType::Product},
                {L"@POLAR",         NodeType::Polar},
                {L"@DERIV",         NodeType::Derivative},
                {L"@INTEGRAL",      NodeType::Integral},
                {L"%",              NodeType::Percent},
                {L"@ELEMENT_OF",    NodeType::ElementOf},
                {L"@DEG",           NodeType::Degree},
                {L"@MATRIX",        NodeType::Matrix},
            };
    };
}

#endif // __MATHCADPARSER_H__