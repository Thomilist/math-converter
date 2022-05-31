#ifndef __MATHCADPARSER_H__
#define __MATHCADPARSER_H__

// Standard libraries
#include <string>
#include <memory>
#include <unordered_map>

// Custom headers
#include "../intermediate/ParsingTree.hpp"
#include "../Parser.hpp"
#include "../intermediate/NodeTypes.hpp"

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
        
        private:
            ParserState state = ParserState::LookingForExpression;
            int depth = 0;

            const std::unordered_map<std::string, NodeType> math_operators = 
            {
                {"@LABEL", NodeType::Label},
                {"@PARENS", NodeType::Parentheses},
                {"=", NodeType::EqualityEvaluation},
                {"@EQ", NodeType::EqualityComparison},
                {":=", NodeType::EqualityDefinition},
                {"+", NodeType::Addition},
                {"-", NodeType::Subtraction},
                {"*", NodeType::Multiplication},
                {"/", NodeType::Division},
            };
    };
}

#endif // __MATHCADPARSER_H__