# CSC
`CSC` is simple and easy extensible C Subset Compiler. It's not realy for production, it was made JUST FOR FUN.
It contains lexer, parser, llvm-ir-codegenerator and SSA-form-builder.

### Tech

`CSC` uses a number of open source projects to work properly:

* [LLVM](https://llvm.org/) - Framework for building compilers and codegenerators.

And of course `CSC` itself is open source with a [public repository][dill]
 on GitHub.

### Installation

`CSC` requires [LLVM](https://llvm.org/) v5.0+ to run.

Install the dependencies and build compiler.

```sh
$ sudo apt-get install llvm
$ sudo apt-get install clang
$ git clone https://github.com/masyagin1998/CSC.git
$ cd CSC
$ make
```

### HowTo

`CSC` supports simplified version of `C89` with:
 - `int` variables definitions;
 - `for`, `while` and `do-while` loops;
 - `if-then` and `if-then-else` statements;
 - all arithmetical and logical operations;
 - inner compound statements;
 - and many other;
