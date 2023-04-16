# parse\_gen

### Overview

`parse_gen` is a simple LL(1) C parser generator.
It takes 2 arguments for the source file and the output c file along with an optional 3rd argument specifing an output c header file.
The source file is in a syntax similar to `bison`/`yacc` however there are key differences as this application shall **not** accept bison grammar files and vis versa.
These differences include (*but are not exclusive to*) different prefixes for commands as well as a reduced command set.
Specification of the proper input file format is documented [here](grammar.md).

Once the output c file is generated, one may compile this file with any complying c compiler along with any other files used in compilation.
The outputeed file shall include a definition for a parser taking configured arguments and running symantic actions based upon the passed grammar.
This function will often be named `yyparse`, however the prefix may be modified via a command in the grammar file.

### Building

Before building this project, be sure your environment has a relatively recented version of GNU `make`, `bison`, `flex` and a c compiler with compatibility with the `gnu99` c standard and common gcc flags.

To build the `parse_gen` exectuable in release mode run either `make` or `make all`.
This shall build all with optimization towards speed and without debug information.
Should debug information be desired, run `make debug` which shall build `parse_gen` still with full optimization, but this executable shall have full dwarf debuging information.

For further modification of the build process, there are the following variables which can be passed at the end of make:

* `V=1`

  This shall have the build be run verbose mode, showing the full commands being run during building.

* `CC=<name>`

  This sets the c compiler used to be `<name>` for all actions during the build.

* `D=<...>`

  Adds custom flags to be passed for every compilation of a c file.
  For more complicated flags or multiple flags it is highly recommened to wrap the `<...>` by double quotes.

### Examples

Example projects and code that use `parse_gen` can be found in the `example` directory.
These include:

* Basic grammar files which generate code for building parse tree from a built-in token stream (`fischer-4-1` `fischer-p-140-10` `llre`)

  The grammar definition and token stream in simple text forms can be found in the `base` subdirectory for these examples.

* More complex programs with a `Makefile` which can be used to build them (`prefix_calc` `regex`).

  The building process for these are more simplified and may need some minor adjustments.
  `prefix_calc` is a basic floating point prefix calculator.
  `regex` is a mid point for regular expression compilation which reads in a regular expression, and outputs a NFA in the form of a pair of tables.
  

Be sure that `parse_gen` is built in the project root directory before attempting to compile any of the above examples.


### Documentation

Full documentation for `parse_gen` can be found [here](top.md).
