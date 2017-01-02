/*------------------------------------------------------------*/
/* Filename:  query.lex                                       */
/* To compile: flex query.lex                                 */
/*            g++ -o query lex.yy.c -lfl                      */
/* Flex can also use gcc or cc instead of g++                 */
/* Takes a file called query.txt with a query in it           */
/* Preprocesses the query terms and builds the accumulator    */
/* displays the top matching documents on the screen          */
/*------------------------------------------------------------*/

%{

#undef yywrap            // safety measure in case using old flex 

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "hashtable.h"
using namespace std;

bool InScript = false;
HashTable QueryHT (50);    // The hashtable of preprocessed query words
HashTable StopwordHT (588);         

void Downcase (char *Token)
{
   // run over characters in yytext, downcasing
   for (int i = 0; i < strlen(Token); i++)
       if (('A' <= Token[i]) && ('Z' >= Token[i]))
          Token[i] = 'a' + Token[i] - 'A'; 

   string TempStr = Token;
   if ( !StopwordHT.Found (TempStr))    // HW3
      //Insert in to the local hash table
      QueryHT.Insert (Token);
   
}
%}

UPPERCASE [A-Z]
LOWERCASE [a-z]
LETTER [A-Za-z]
DIGIT [0-9]

%%
[\n\t ]  ;                                      /* White space, consume it */
({DIGIT}|{LETTER}){1}            ;              /* Remove single characters */
\&.*\;                                          /* Remove html &nbsp; etc */

\<script[^>]*>  { InScript = true; }            /* Scripts*/
\<\/script>  { InScript = false; }              /* Scripts*/
\<[^>]*\> ;                                     /* Remove HTML tags */

{DIGIT}{3}"-"{DIGIT}{3}"-"{DIGIT}{4} { QueryHT.Insert(yytext);}                      /* Phone numbers */
({LETTER}|{DIGIT})+@({LETTER}|{DIGIT})+".com" { QueryHT.Insert(yytext);}             /* Email */
("http://"|"www.")({LETTER}|{DIGIT}|"/"|"."|"_"|"~")+ { QueryHT.Insert(yytext);}     /* URL */
{DIGIT}+"."{DIGIT}+ ;              /* Remove decimal numbers */
{DIGIT}+(","{DIGIT}+)+ { QueryHT.Insert(yytext);}            /* Large numbers with commas */

({LETTER}|{DIGIT})+ { if (!InScript) Downcase (yytext);}  /* String */
.              ;   /* Throw away everything else */

%%
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

// global variables for the input directory pointer
// and output directory name for use in yywrap
// and the output inverted file
ofstream Map;

//variable to store stopword
string stopword;

//Function to populate stopword hashtable
void StopwordHTPopulate()
{
//Open stopword file
ifstream Din;
string Stopword;

   Din.open("stoplist.txt");

   //Loop while file is valid
   while(Din >> Stopword)
      //Insert each word to stopword hashtable
      StopwordHT.Insert(Stopword);

  Din.close();
}

// HW4
void FillAcc (HashTable &Accumulator, const string DictFilename, const string PostFilename)
{
// assumes inverted file is in the same directory
ifstream Dict;
ifstream Post;

   // First, open the inverted files
   Dict.open(DictFilename.c_str());
   Post.open(PostFilename.c_str());
   if (!Dict.good())
      cerr << "Could not open " << DictFilename << endl;
   else if (!Post.good())
      cerr << "Could not open " << PostFilename << endl;

   // Loop over the query terms, filling the accumulator
   QueryHT.SumWeights (Accumulator, Dict, Post);
   Accumulator.PresentResults();
}


int main(int argc, char **argv)
{
const string QueryFilename = "query";
const string DictFilename = "dict";
const string PostFilename = "post";
HashTable Accumulator (1683);    // The hashtable of preprocessed query words

   //Populate stopword hashtable
   StopwordHTPopulate();

   if((yyin = fopen(QueryFilename.c_str(), "r")) == NULL)
      cerr << "\nQuery is empty.\n";
   else
   {
      // Tokenize the contents of QueryFilename
      // Store the preprocessed tokens in QueryHT
      // The ht data field stores the tf of the term in the query
      yylex();
      fclose(yyin);

      if (QueryHT.NumUsed() != 0)
         FillAcc (Accumulator, DictFilename, PostFilename);
      else
         cout <<"Empty Query.\n";
   }
}
