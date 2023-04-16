# Grammar File Format (`.cfg`)

## Token Types

The grammar file is read via a steam of tokens sourced from a `flex` generated lexer.
As such each of the tokens that may be mentioned below will have both a general description along with the regular expression used. Some tokens may shortcut into a lexical error (i.e. a command token with no associated command, or is in the wrong section).

* Unsigned Int:

  Unsigned ints are non zero sequence of digits such that the value is bounded by \[0, 65535\].

  `[0-9]+`

* Commands:

  Commands are a slash followed by an letter and any number of alphanumerical characters along with underscore.

  `\/[a-zA-Z][a-zA-Z0-9_]*`

* Identifiers:

  Identifiers are a letter followed by any number of alphanumerical characters along with underscores.

  `[a-zA-Z][a-zA-Z0-9_]*`

* Type Specifier:

  Type Specifiers are a left angle followed by any non new line character until the first right angle.

  `\<[^\>\n]*\>`

* Left Block Bracket:

  Left block brackets are a slash followed by a left curly bracket.  

  `\/\{`

* Right Block Bracket:

  Right block brackets are a slash followed by a right curly bracket.

  `\/\}`

* Simple Block:

  Simple blocks are a left block bracket followed by any character until the first right block bracket not in a c comment or c string/character literal.

  `\/\{(\*|\}|\/*([^\*\/\"\'\}]|\/\*([^\*]|\*+[^\*\/])*\*+\/|\"([^\"\\]|\\(.|\n))*\"|\'([^\'\\]|\\(.|\n))*\'))*\/\}`

* Whitespace:

  Whitespace is any continuous sequence of spaces and tabs.

  `[ \t]+`

* Comments:

  Comments are a slash star followed by all characters until the first start slash.

  `\/\*([^\*]|\*+[^\*\/])*\*+\/`

## General Structure

A grammar file consists of 3 main sections, each separated by a single line consisting of `%%`.
The sections in order are the **definitions** section, **rules** section, and **epilogue** section.
The **definitions** section contains the command sequences along with declarations and definition which are used by the rest of the grammar for variables, rules, and actions.
The **rules** section defines all of the rules for the context free grammar along with the *optional* semantic action associated with each rule.
The **epilogue** section is code to be pasted at the end of the generated c file.
The **epilogue** section and its separator may be omitted by a grammar file.

### Definitions

The **definitions** section defines all tokens and nonterminals used by a context free grammar.
Furthermore, it controls which parameters will be passed into the parser and lexer in the generated code and other adjustments. This section consists or either empty lines (can have whitespace/comments), and line seperated commands (minor exceptions for new line separation shall be noted below).

List of Commands:

* `/token <type specification>? <identifier> <unsigned int>`

  This defines a token with optional type along with the associated value for lexer return code under the given name.
  Should the type be specified, then a field to the union `YYSTYPE` with the same name as the identifier shall be added.
  For this reason, it is not recommended to have identifier be c keyword or identifier already used at some point in the code blocks.
  An identifier can only be passed into the `/token` or `/nterm` command once.

* `/nterm <type specification>? <identifier>`

  This defines a nonterminal with optional type under the given name.
  Should the type be specified, then a field to the union `YYSTYPE` with the same name as the identifier shall be added.
  For this reason, it is not recommended to have identifier be c keyword or identifier already used at some point in the code blocks.
  An identifier can only be passed into the `/token` or `/nterm` command once.

* `/start <identifier>`

  This sets the goal symbol for the context free grammar.
  Doing so shall effectivly add the `YYEOF` to this nonterminal's follow set and have this symbol be the top level call for the parser.
  One may only use the `/start` command once in a grammar.
  This command **must** be placed after the declaration of the nonterminal used.

* `/parseparam <type specification> <identifier>`

  This adds a parameter to the call signature of `yyparse` with specified type.
  The parameter shall be available to all code within semantic actions.
  Should this command be used multiple times, then multiple parameters shall be added to `yyparse` in the order they appeared.

* `/lexparam <type specification> <identifier>`

  This adds a parameter to interior calls of `yylex` be the parser.
  The parameter name should be either a global variable, or one of the parser parameters.
  Should this command be used multiple times, then multiple parameters shall be passed to calls of `yylex` in the order they appeared.

* `/codetop <simple block>`

  This adds code to be placed at the top of the generated c file before anything else.
  This command may only be used once in the grammar file.

* `/coderequires <simple block>`

  This adds code to be placed in both the generated c file and c header file *before* the definition of the `YYSTYPE` union.
  This command may only be used once in the grammar file.

* `/codeprovides <simple block>`

  This adds code to be placed in both the generated c file and c header file *after* the definition of the  `YYSTYPE` union.
  This command may only be used once in the grammar file.

* `/code <simple block>`

  This adds code to be placed in the generated c file after all header code but before all interior c code for the parser and tables.
  This command may only be used once in the grammar file.

* `/prefix <identifier>`

  This sets the prefix for all interior variable and functions to be the identifier specified.
  Note that the prefixes shall either by the uppercase or lowercase verions of the passed identifier.
  Should this command not be used, the default prefixes of `yy` and `YY` shall be used.
  This command may only be used once in the grammar file.

Note that every grammar must specify at least one nonterminal, along with a nonterminal that shall be the goal of the grammar.

### Rules

The **rules** section defines all of the rules and semantic actions for the context free grammar used.
It consists solely of rule group lists and comments.
Newlines and whitespace outside of code blocks are ignored.
The format of a rule group list is as follows:

`<identifier>: <patterns>;`

The identifier on the left hand side is the target nonterminal.
Patterns are a `|` delimited list of the rhs of the target along with their semantic action.
The rhs is represent as a potentially empty list of tokens and nonterminals.
For empty rhs, it is good practice to use `/empty` to show that this was intentional.
The optional action of the rule is c code wrapped be the left block and right block brackets.
Within the action, the code shall have access to the values of each of the rhs symbols along with the lhs target.
Additionally, all of the parameters for `yyparse` and any declared global variables shall also be avaiable.
It is not recommended to have `goto` within any semantic action, as in implementation these a placed in a switch statement and `goto`s may allow semantic actions to interfier with one another.

See examples of `.cfg` files should there be any confusion about rule definition.

### Epilogue

The **epilogue** section is simply c code that shall be pasted to end of the generated c implemenetation file after everything else. This section would be good location to place definitions of interior functions declared earlier via `/code` or its variant commands.

## Links

[Top](docs/top.md)

[README](docs/README.md)

[Generated Code](docs/code.md)

[Grammar File Format](docs/grammar.md)
