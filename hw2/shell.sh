#!/bin/bash

# Remove the output directory for a fresh start
if [ -e "$2" ]
then
   /bin/rm -r "$2"
fi

# Compile and run lex file
flex tokenize.lex
g++ -c hashtable.cpp
g++ -o tokenize lex.yy.c -lfl hashtable.o

./tokenize $1 $2

# Add a slash to output directory if not already there
output=$2
if [[ "$2" != *\/* ]] 
then
  output+="/"
fi
