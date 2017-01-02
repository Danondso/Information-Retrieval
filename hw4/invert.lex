/*----------------------------------------------------------------*/
/* Filename:  tokenizer.lex                                       */
/* To compile: flex tokenizer.lex                                 */
/*            g++ -o tokenizer lex.yy.c -lfl                      */
/* Flex can also use gcc or cc instead of g++                     */
/* Takes in and out directories: ./tokenizer <indir> <outdir>     */
/*----------------------------------------------------------------*/

%{

#undef yywrap            // safety measure in case using old flex 

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include "hashtable.h"
#include "Agregator.h"
#include <thread>
using namespace std;

char Ch;
bool InScript = false;
float LocalTokenCount = 0;
float DocumentCount = 0;       // HW3
HashTable LocalHT (3000);
HashTable StopwordHT (588);              // HW3
GlobalHashTable LTable (38000);

//Vectors for the Line number and String 
  //Used to search the dict file

  std::vector<long> HLines;
  std::vector<string> HString;

//Accumulator Object
  Agregator Ag;

int FilenameWidth;  // HW3

void Downcase (char *Token)
{
   // run over characters in yytext, downcasing
   for (int i = 0; i < strlen(Token); i++)
       if (('A' <= Token[i]) && ('Z' >= Token[i]))
          Token[i] = 'a' + Token[i] - 'A'; 

 string TempStr = Token;
   if (!StopwordHT.Found(TempStr)){ //the index of the token in the hashtable
      //Insert in to the global to find the index of the key
       HLines.push_back(LTable.Find(Token));
        HString.push_back(Token);
       }
   
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

{DIGIT}{3}"-"{DIGIT}{3}"-"{DIGIT}{4} {HString.push_back(yytext);  HLines.push_back(LTable.Find(yytext));}                      /* Phone numbers */
({LETTER}|{DIGIT})+@({LETTER}|{DIGIT})+".com" { HString.push_back(yytext);  HLines.push_back(LTable.Find(yytext));}             /* Email */
("http://"|"www.")({LETTER}|{DIGIT}|"/"|"."|"_"|"~")+ { HString.push_back(yytext);  HLines.push_back(LTable.Find(yytext));}     /* URL */
{DIGIT}+"."{DIGIT}+ ;              /* Remove decimal numbers */
{DIGIT}+(","{DIGIT}+)+ {  HString.push_back(yytext); HLines.push_back(LTable.Find(yytext));}            /* Large numbers with commas */

({LETTER}|{DIGIT})+ { if (!InScript) Downcase (yytext);}  /* String */
.              ;   /* Throw away everything else */

%%
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

// global variables for the input directory pointer
// and output directory name for use in yywrap
// and the output inverted file
DIR *InputDirPtr = NULL;
char OutputDirname[500];
char InputDirname[500];
ofstream Map;
int DocId = 0;


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

//Method to process the postings file.
 void PostProcess(int numdocs, int start){

    //Post seaching object  
    ifstream postSearch;

    //opens the file
    postSearch.open("./output/post");
   
   //seeks to the correct post line 
    postSearch.seekg(start * 12);

   for(int i = 0; i < numdocs; i++)
          {
           //Variables for the docid and the weight
            int docid;
             int wt;

          // postSearch.seekg(0);
            postSearch >> docid >> wt;
     
          // cout << "Post Searcing: "  << docid << " Weight: " << wt << endl;
    
           //insert into the accumulator thingy here
             //Just a giant array
             Ag.Insert(docid, wt);
             // postSearch.seekg(0);
               
          }
         postSearch.close();     
   }
  
int main(int argc, char **argv)
{
DIR *OutputDirPtr = NULL;
char Command[509];
float ReturnVal;
string MapFilename;
string DictFilename;
string PostFilename;

const int MAX_FILENAME_LEN = 15;  // HW3

   //Populate stopword hashtable
   StopwordHTPopulate();
  
   ofstream dout;
//std::vector<unsigned long> qLines;

  //Left the IF statement to make sure there's query to process.
   if(argc < 1)
    {
      fprintf(stderr, "Incorrect number of arguments.\n"); 
      return (1);
    }

  //Zeroes out the accumulator array
    Ag.Zero();

  //Opens the query text file to pass the query string into.
   dout.open("query.txt");

  //For loop to pass the tokens.
    for(int i = 1; i < argc; i++)
       dout << argv[i] << endl;

      dout.close();
   
   //Have the query file opened up to be lexed.
      yyin = fopen("query.txt","r");
      yyout = NULL;

      // call yywrap and yylex to process the files
        yylex();

   
   // for(int i = 0; i < HLines.size(); i++)
    //   cout << HLines[i] << endl;
      
    //ifstream object created to search the dict
     ifstream dictSearch;
 
    //opens the dict file
    dictSearch.open("./output/dict");
  
     

    for(int i = 0; i < HLines.size(); i++)
     {
      //variables to grab the data with.

       string token;
       int numdocs;
       int start;

     //seeks to the top of the page each time.
      dictSearch.seekg(0);

     // cout << "Vector Position " << HLines[i] << endl; 
      //Seeks the line to check to make sure the query is correct.
        dictSearch.seekg(HLines[i] * 59);
       //Grabs the data from the dict file.
        dictSearch >> token >> numdocs >> start;
    
      //  cout << token << " " << numdocs << " " << start << endl; 
       //  cout << "Num Docs " << numdocs << " Start: " << start << endl;         
    
       //Checks to make sure the token matches the dict entry, if not
         //Check further and further until there's a blank dict entry.
        while(token != HString[i] && start != -1)
          {
            dictSearch >> token >> numdocs >> start;
          }
       //Once the token matches, start the posting process.
       PostProcess(numdocs, start);      
      }
    //close
     dictSearch.close();

   //After the post process accumulation, we sort and display the results. 
    Ag.FindResults();

   //Prints them out.
    Ag.Print();

 return 0;  
}


