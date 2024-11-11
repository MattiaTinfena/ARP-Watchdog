#!/bin/bash

# Compile all .c files in the current directory with ncurses library
for file in *.c; do
    gcc -o "${file%.c}" "$file" -lncurses -ltinfo
done

echo "Compilation finished."