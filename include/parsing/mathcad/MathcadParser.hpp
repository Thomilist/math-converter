#ifndef __MATHCADPARSER_H__
#define __MATHCADPARSER_H__

// Standard libraries
#include <string>
#include <memory>
#include <unordered_map>

// Definitions and forward declarations
#include "../../Definitions.hpp"
#include "../../ForwardDeclarations.hpp"

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
            MathcadParser(std::shared_ptr<Lexer> a_lexer);
            ~MathcadParser();

            void Parse(std::shared_ptr<ParsingTree> a_parsing_tree);
            void ParseExpression(std::shared_ptr<ParsingTree> a_parsing_tree);
            void Clean(std::shared_ptr<Node> a_node);
            void EscapeCharacter(std::shared_ptr<Node> a_node, String a_find, String a_replace);
        
        private:
            ParserState state = ParserState::LookingForExpression;
            Token current_token = Token(TokenType::OutOfBounds);

            const std::unordered_map<String, NodeType> math_operators = 
            {
                {STR("@LABEL"),         NodeType::Label},
                {STR("@PARENS"),        NodeType::Parentheses},
                {STR("="),              NodeType::EqualityEvaluation},
                {STR("@EQ"),            NodeType::EqualityComparison},
                {STR(":="),             NodeType::EqualityDefinition},
                {STR("@NEQ"),           NodeType::NotEqual},
                {STR("@SYM_EVAL"),      NodeType::SymbolicEvaluation},
                {STR("@IS"),            NodeType::EqualityComparison},
                {STR("@GLOBAL_DEF"),    NodeType::Definition},
                {STR("@KW_STACK"),      NodeType::KeywordStack},
                {STR("@SEP"),           NodeType::Separator},
                {STR("+"),              NodeType::Addition},
                {STR("-"),              NodeType::Subtraction},
                {STR("*"),              NodeType::Multiplication},
                {STR("/"),              NodeType::Division},
                {STR("@SCALE"),         NodeType::ScaledField},
                {STR("@RSCALE"),        NodeType::ScaledResult},
                {STR("@PLACEHOLDER"),   NodeType::Void},
                {STR("@RPLACEHOLDER"),  NodeType::Void},
                {STR("@NTHROOT"),       NodeType::Radix},
                {STR("^"),              NodeType::Exponentiation},
                {STR("@ID"),            NodeType::TextComposite},
                {STR("@SUB"),           NodeType::TextSubscript},
                {STR("!"),              NodeType::Factorial},
                {STR("<"),              NodeType::LessThan},
                {STR(">"),              NodeType::GreaterThan},
                {STR("@LEQ"),           NodeType::LessThanOrEqual},
                {STR("@GEQ"),           NodeType::GreaterThanOrEqual},
                {STR("@ABS"),           NodeType::Absolute},
                {STR("@CROSS"),         NodeType::VectorProduct},
                {STR("@RANGE"),         NodeType::Range},
                {STR("@STEPRANGE"),     NodeType::StepRange},
                {STR("@ARGS"),          NodeType::Arguments},
                {STR("@APPLY"),         NodeType::Function},
                {STR("@FUNCTION"),      NodeType::Function},
                {STR("@NEG"),           NodeType::Negative},
                {STR("@NOT"),           NodeType::LogicalNOT},
                {STR("@AND"),           NodeType::LogicalAND},
                {STR("@OR"),            NodeType::LogicalOR},
                {STR("@SUM"),           NodeType::Sum},
                {STR("@PRODUCT"),       NodeType::Product},
                {STR("@POLAR"),         NodeType::Polar},
                {STR("@DERIV"),         NodeType::Derivative},
                {STR("@INTEGRAL"),      NodeType::Integral},
                {STR("%"),              NodeType::Percent},
                {STR("@ELEMENT_OF"),    NodeType::ElementOf},
                {STR("@DEG"),           NodeType::Degree},
                {STR("@MATRIX"),        NodeType::Matrix},
            };
    };
}

#endif // __MATHCADPARSER_H__