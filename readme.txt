Joshua Baker                                           jkb150030
Dr. Gupta
CS 4386.001
4/25/18


================================================================
                          INTRODUCTION
================================================================

This project is a compiler for the TL13 language according to
the specification given by Dr. Gupta. If the input code file is
valid, the compiler outputs C which can then be compiled to
machine code with a C compiler, such as gcc.

Below is information about how to compile this project and use
the resulting compiler. Then follows a detailed description of
each component of the compiler and how it was designed.



================================================================
                             USAGE
================================================================

DEPENDENCIES
    make
        Used to build each component of the project.
    flex
        Used to construct the lexical analyser.
    bison
        Used to construct the parser.
    gcc
        Used to compile both the compiler and the output C from
        the compiler (from the TL13 source).


BUILDING THE COMPILER
    To create the compiler executable, from the project root
    folder execute 'make'. This will populate the intermediate
    folder with all the object files and the outputs from flex
    and bison, and the compiler executable will be placed in
    the project root folder and called 'tl13compiler'.

    To delete the contents of the intermediate folder and the
    compiled folder as well as tl13compiler, from the project
    root folder execute 'make clean'.


USING THE COMPILER
    The compiler executable reads TL13 input code from the
    standard input and prints the corresponding C code on the
    standard output. If an error is encountered, that will be
    printed to the standard error and the compiler will exit
    with a non-zero status.

    If compiling succeeds, the resulting C code can be
    compiled to machine code with simply
    'gcc <file containing output from tl13compiler>'.

    To simplify using the compiler, I've created a helper script
    called compileTL13CodeFile.sh. Note that this script relies
    on bash as a dependency. Using compileTL13CodeFile.sh, you
    can compile a TL13 codefile with the following command:
    './compileTL13CodeFile.sh <code file>'.

    If an error occurs along the way, the helper script will
    print an error message to the standard error stream.
    
    Otherwise, if compilation succeeds, the output C code
    is printed to the standard output along with a message
    explaining where to find the compiled executable binary.


================================================================
                       PROJECT STRUCTURE
================================================================

DIRECTORIES
    In an effort to stay organized, I created mutliple top-level
    directories.

    src
        This folder contains all of the C code files as well as
        the definitions for the lexical analyser and the parser.
    headers
        This folder contains all the header files except for the
        one that is created by bison during build. Almost every
        C code file in src has a corresponding header file
        declaring its functions and structures.
    intermediate
        This folder exists to keep the temporary mid-build files
        (object files from compiling C code files and C files
        from bison and flex) away from everything else.
    examples
        This folder contains all the TL13 code files I wrote to
        test the compiler. It contains 2 subdirectories: good
        and bad.
        good contains the TL13 code files that are valid and
        ought to compile successfully.
        bad contains code files that have been malformed in
        specific ways (corresponding to their names) to test
        that the compiler notices and reports errors properly.
    compiled
        This folder contains both the C output from tl13compiler
        as well as the executable binary after compiling a TL13
        code file using compileTL13CodeFile.sh.



LEXICAL ANALYSER
    The lexical analyser is created by flex according to the
    flex code file src/lexer.l. This file contains definitions
    for each valid token, storing values where appropriate and
    returning the corresponding enum value every time a token is
    recognized.

    A few of the tokens require storing additional data before
    flex returns. The NUM token stores the integer value
    corresponding to the recognized text; the BOOLLIT token
    stores the boolean value; the IDENT token stores the string
    value; and the OP tokens store the enum value of the
    particular operator that was recognized.

    If a character is fed to the standard input that is not in
    TL13's alphabet, flex simply ignores that character.
    The only error that can arise from this module is the int
     out of bounds error, which is thrown if an integer literal
    is greater than 2147483647.


PARSER
    The parser defines the structure of the language's context-
    free grammar and constructs the parse tree. It is defined
    by src/parser.y

    The grammar is defined in exact congruence with the language
    specification provided by Dr. Gupta.

    To improve readability, most of the code that constructs
    each node in the tree was written in functions inside
    src/makeStructs.c. The code for rules which do not involve
    creating new node structures is written plainly in parser.y.

    Once the whole parse tree has been constructed, the parser
    initiates compilation.

    Most of the errors that can be encountered during execution
    are seen here (or in src/makeStructs.c). In the case of a
    syntax error, the bison-generated parser will call yyerror
    which outputs an error message and immediately stops the
    program execution. All the other errors are discovered
    while creating the parse tree, and yyerror is then called
    manually.


SYMBOL TABLE
    The symbol table keeps track of currently defined variables
    in the TL13 code, storing each variable's name and type.
    Its code consists of sdbm.c and symbolTable.c in src.

    The symbol table is implemented with a hash table
    (hash map) which makes use of the sdbm hash function. I
    copied this hash function from
    http://www.cse.yorku.ca/~oz/hash.html.

    The symbol table is a simple array of pointers to symbol
    structures. Its default length is 16, but it expands by
    doubling in size every time it runs out of space to store
    new variables. Each symbol structure contains the string
    variable name and the type of that variable.

    The hash table is an array of pointers to nodes, 31 long,
    and uses linked lists to handle hash collisions. Each node
    contains an integer which refers to the index of its
    variable in the symbol table as well as a pointer to the
    next node in the linked list. Each node is placed at the
    end of the linked list corresponding to the index of the
    hash table that matches the hash value of the variable
    name.

    The index of each variable in the symbol table is used as
    a handle to refer to that variable throughout the parse
    tree. Information about a variable can be quickly found
    using its handle, and a variable's handle can be quickly
    derived by taking the hash value of the variable's name
    and searching the hash table.


COMPILATION
    All the functionality that produces the output C code from
    the parse tree is defined in src/compile.c. Each type of
    node in the parse tree has its own compilation function,
    which prints out just the C code for that type of node.
    Each of these functions also frees the memory associated
    with its node.

    compile
        This function is called by the parser to initiate
        compilation. It calls printHeader, declareVars,
        printStatements, and printFooter.
    printHeader
        This function prints include statements (to get
        stdlib.h, which is needed for free(), size_t, NULL, and
        atoi(); stdio.h, which provides getline() and printf();
        and stdbool.h, which allows me to use the keywords
        "bool", "true," and "false" in the C code (to improve
        readability).
        This function also defines int readInt(), which reads
        an integer value from the stdin at runtime (no error
        handling functionality is built in).
        Finally, printHeader prints the beginning of the main
        function so that the program can get started.
    declareVars
        This function has the simple task of defining each
        variable that will be used in the program. The names
        given in the TL13 code file are preserved, but the
        prefix "tl13_" is added to each variable name to avoid
        collisions with terms in the output C, such as "NULL".
    printStatements
        This function walks through the list of statements
        and calls the proper function to print the C code
        associated with each one.
    printFooter
        This function prints the end of the main method so the
        output C compiles correctly.
    printTabs
        This function is declared in utility.h. It helps make
        The output C more readable by keeping track of proper
        indentation.

