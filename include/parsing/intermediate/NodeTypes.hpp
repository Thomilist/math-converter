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

        // Nonterminal nodes (effectively a list of mathematical operations and syntactic constructs such as matrices)
        TextComposite,
        Variable,
        Unit,
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
        Differentiation,
        Integration,
        Sine,
        Cosine,
        Tangent,
        Cosecant,
        Secant,
        Cotangent,
        Absolute,
        Factorial,
        LogicalNOT,
        LogicalAND,
        LogicalOR,
        LogicalNAND,
        LogicalNOR,
        LogicalXOR,
        LogicalXNOR,
    };
}

#endif // __NODETYPES_H__