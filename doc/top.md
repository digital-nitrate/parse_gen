# parse\_gen

## Usage

To use the `parse_gen` application, use the following command:

`parse_gen <src> <outc> <outh>?`

Note that should the application not be added to the path it may be necessary to use the absolute or relative path to the executable.

* `<src>` is the path to the grammar file used.

* `<outc>` is the path to the output c file.
  This file shall be overwritten with the c parser implementation.

* `<outh>` is the path to the output c header file.
  This field is optional and when specified shall be overwritten.

After running the application, one can check the return code for whether a parser implementation was successfully generated (nonzero return code for failure).

One may then compile the parser implementation with the rest of some project.
The header file contains the token enumerations with their defined codes, along with the definition of the `YYSTYPE` union.
Furthermore, additional code specified via the `/coderequies` and `/codeprovides` commands will be placed in this header file.

## Links

[Top](top.md)

[README](README.md)

[Generated Code](code.md)

[Grammar File Format](grammar.md)
