Constructor Tag
===============

An empty struct with a single constexpr instance declared used to disambiguate
constructor overloads.

This is a pattern frequently used in C++ to distinguish between constructors
when otherwise we would have ambiguity on overload choice. This is specially
useful on [resources](#resource), as we already define empty ctor to mean a
"invalid" state, so a creation function that takes no parameters can not be 
wrapped directly.

