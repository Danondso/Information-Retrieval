#!/bin/bash

# Compile and run lex file
flex query.lex
g++ -std=c++0x -o query posting.cpp globalhashtable.cpp Agregator.cpp hashtable.cpp lex.yy.c -lfl -pthread

./query $1 $2 $3 $4 $5 $6 $7 $8 $9

