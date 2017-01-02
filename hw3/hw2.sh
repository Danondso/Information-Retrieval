#!/bin/bash

# Remove the output directory for a fresh start
if [ -e "$2" ]
then
   /bin/rm -r "$2"
fi

# Compile and run lex file
flex invert.lex
g++ -o invert posting.cpp globalhashtable.cpp hashtable.cpp lex.yy.c -lfl

./invert $1 $2

# Add a slash to output directory if not already there
output=$2
if [[ "$2" != *\/* ]] 
then
  output+="/"
fi
