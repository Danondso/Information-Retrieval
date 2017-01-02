#!/bin/bash

# Locate current directory
CURRENT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Remove (reset) file if exists
if [ -f $CURRENT_DIR/query ];
then
   rm query
fi

# Write queried words in a file for tokenizing
for word in "$@"
do
   # Write to file
   echo "$word" >> query
done

#flex query.lex
#g++ -o retrieve hashtable.cpp template_taillist.h posting.cpp lex.yy.c -lfl 

# Run
./retrieve

# Clean up
rm query
