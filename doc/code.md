# Generated Code

## Header File

The generated header file shall include type and constant definitions, along with code from the `/coderequires` and `/codeprovides` commands.
`/coderequies` is pasted before generated implemenetations, and `/codeprovides` is pasted afterwards.

The generated implementations include an enumeration associated every token with their associated code from where they were defined.
Additionally, two special tokens of `YYEOF = 0` and `YYUNDEF = -1` are added to represent end of file and lexical error respectively.
An union type named `YYSTYPE` is also included and is the union of all forms of the semantic values for the contex free grammar's tokens and nonterminals.

## Parser Implementation

The parser implementation shall have code in the following order:

1. `/codetop`

2. `/coderequires`

3. enumeration and union definitions

4. `/codeprovides`

5. `/code`

6. interior parser code

7. **epilogue**

The interior parser code shall include a tranform on the token id codes into a continuous space, a table mapping nonterminals with the associated rule based upon predict sets, basic error message generation, and an interior method for requesting a nonterminal.

The error message generation works by building a null terminated message in a string buffer, and then passing the buffer along with any parser parameters to a user defined `yyerror` function.
Given that this file does not provide a signature for `yyerror`, a signature should be provided by the grammar file in one of the `/code` variant commands.

Nonterminal construction works by using the table to figure out which rule to use, and then after requesting all of the symbols needed by a rule, running the user defined semantic action.
Given that this usually defined via a switch statement, `goto`s and `longjmp`s are not advised.
During the nonterminal construction, calls to `yylex` are made without a generated signature definition, so this must be provided by the user in the grammar definition file in one of the `/code` variants. The signature assumed by the generated code is as follows:

`int yylex(YYSTYPE*, <lexer args>);`

## Interface Summary

* `YYSTYPE` union for the types for tokens and nonterminals

* enumeration associating a int with every token

* `void yyerror(char const* msg, <parser args>);` user defined function called upon syntax error, where msg is provided as a basic explanation

* `int yylex(YYSTYPE* yylvalp, <lexer args>);` user defined lexer used by the parser for accessing the token stream

* `int yyparse(<parser args>);` function defined by generated code for running the parser.
   Returns 0 on success, nonzero on failure.

## Links

[Top](top.md)

[README](README.md)

[Generated Code](code.md)

[Grammar File Format](grammar.md)
