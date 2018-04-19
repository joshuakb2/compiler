#!/bin/bash

# Get current directory of this script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Print help info
help() {
    echo "Compiles the given TL13 input file and displays the output C code on the stdout."
    echo
    echo "Usage: $(basename "$0") <tl13 code file>"
}

# There must be exactly 1 argument
if (( $# != 1 )); then
    help
    exit 1
fi

# If the user asks for help
if [[ "$1" == "--help" ]]; then
    help
    exit 0
fi

tl13filePath="$1"
tl13fileName="$(basename $tl13filePath)"
cFile="$DIR/compiled/${tl13fileName%.*}.c"
binFile="$DIR/compiled/${tl13fileName%.*}.out"
compiler="$DIR/tl13compiler"

# Make sure the compiler is there and can be executed
if ! [[ -x "$compiler" ]]; then
    echo "Expected to find an executable file at the following path: \"$compiler\""
    echo "Try executing \"make\" first."
    exit 1
fi

# If the first argument refers to a file that exists
if [[ -f "$tl13filePath" ]]; then
    # Run that file through my compiler, send output to C file and stdout
    cat "$tl13filePath" | "$compiler" | tee "$cFile"

    # tee will output 0 regardless of whether my compiler succeeded or failed,
    # so check specifically the return value from my compiler.
    # If my compiler succeeded
    if (( ${PIPESTATUS[1]} == 0 )); then
        # Then use gcc to compile the C file to a binary file
        if gcc "$cFile" -o "$binFile"; then
            echo "The TL13 file was successfully compiled as \"$binFile\"."
            exit 0
        else # My compiler succeeds but gcc fails
            echo "gcc threw an error!!! Oh no!!!!!"
            exit 1
        fi
    else # My compiler fails
        echo "The TL13 compiler rejected the input file!"
        rm "$cFile"
        exit 1
    fi
else # The input file does not exist
    echo "The given TL13 code file does not exist!"
    exit 1
fi
