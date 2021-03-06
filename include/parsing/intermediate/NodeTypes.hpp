#ifndef __NODETYPES_H__
#define __NODETYPES_H__

// Xlib defines Absolute to 1
#undef Absolute

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
        MatrixRowCount,
        MatrixCollumnCount,

        // Nonterminal nodes (effectively a list of mathematical operations and syntactic constructs such as matrices)
        TextComposite,
        Label,
        Variable,
        ScaledField,
        ScaledResult,
        Constant,
        Function,
        SystemVariable,
        Keyword,
        Parentheses,
        Matrix,
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