# Xpression - A self-documenting expression evaluator

The idea of this little library is to express complex boolean expressions as an expression tree.
The expression tree can evaluate itself, print a description of all the conditions, and explain why the expression evaluates to false.

The fundamental class `Expression` has overloaded logical operators `&&`, `||` and `!`, making the code  expressive and easy to read.

The implementation with no virtual functions in the interface is inspired by a CppCon 2021 talk by Klaus Iglberger, "Breaking Dependencies: Type Erasure - A Design Analysis".
