#!/bin/bash

# Compile and run lex file
flex query.lex
g++ -std=c++0x -o search posting.cpp globalhashtable.cpp Agregator.cpp hashtable.cpp lex.yy.c -lfl -pthread

OUTPUT="$(./search $*)";

echo "${OUTPUT}"



