#ifndef __NODETYPES_H__
#define __NODETYPES_H__

namespace mcon
{
    enum class NodeType
    {
        // Root node
        Root,

        // Terminal nodes
        Number,
        Text,
        TextSubscript,
        TextSuperscript,
        Void,

        // Nonterminal nodes (effectively a list of mathematical operations and syntactic constructs such as matrices)
        TextComposite,
        Label,
        Variable,
        Unit,
        Constant,
        Function,
        SystemVariable,
        Keyword,
        Parentheses,
        Matrix,
        MatrixSize,
        MatrixRow,
        EqualityDefinition,
        EqualityComparison,
        EqualityEvaluation,
        SymbolicEvaluation,
        Definition,
        KeywordStack,
        Separator,
        LessThan,
        GreaterThan,
        LessThanOrEqual,
        GreaterThanOrEqual,
        NotEqual,
        Addition,
        Subtraction,
        Multiplication,
        Division,
        Exponentiation,
        Radix,
        VectorProduct,
        Derivative,
        Integral,
        Absolute,
        Factorial,
        LogicalNOT,
        LogicalAND,
        LogicalOR,
        LogicalNAND,
        LogicalNOR,
        LogicalXOR,
        LogicalXNOR,
        Range,
        StepRange,
        Arguments,
        Negative,
        Sum,
        Product,
        Polar,
        Percent,
        ElementOf,
        Degree,
    };
}

#endif // __NODETYPES_H__