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
#include "hashtable.h"
using namespace std;

char Ch;
bool InScript = false;
HashTable LocalHT (3000);
GlobalHashTable GlobalHT (40000);
HashTable list (250);


void Downcase (char *Token)
{
   // run over characters in yytext, downcasing
   for (int i = 0; i < strlen(Token); i++)
       if (('A' <= yytext[i]) && ('Z' >= yytext[i]))
          yytext[i] = 'a' + yytext[i] - 'A'; 
   
    if(list.GetData(yytext) == -1)
     LocalHT.Insert (yytext);
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

{DIGIT}{3}"-"{DIGIT}{3}"-"{DIGIT}{4} { LocalHT.Insert(yytext);}                      /* Phone numbers */
({LETTER}|{DIGIT})+@({LETTER}|{DIGIT})+".com" { LocalHT.Insert(yytext);}             /* Email */
("http://"|"www.")({LETTER}|{DIGIT}|"/"|"."|"_"|"~")+ { LocalHT.Insert(yytext);}     /* URL */
{DIGIT}+"."{DIGIT}+ ;              /* Remove decimal numbers */
{DIGIT}+(","{DIGIT}+)+ { LocalHT.Insert(yytext);}            /* Large numbers with commas */

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

// This is called once per file.
int yywrap()
{
struct dirent* InputDirEntryPtr;

char InFilename[1000];

   // if there was an input file open 
   // close it to get ready for the next file 
   if (yyin != NULL)
   {
      fclose (yyin);
      yyin = NULL;
   
      // Finished with the file, copy data to global hashtable
      LocalHT.TransferData(DocId, GlobalHT);

      // get ready for the next document
      LocalHT.Reset();
      DocId++;
   }

   // skip over the hidden filenames that begin with dot
   do
   {
      InputDirEntryPtr = readdir (InputDirPtr);
   } while ((InputDirEntryPtr != NULL) &&
            (InputDirEntryPtr->d_name[0] == '.'));

   // if there are still files to process
   if(InputDirEntryPtr !=NULL)
   {
      // open the next file in the list as yyin
      Map << InputDirEntryPtr->d_name << endl;  // write the filename to the map file
      strcpy (InFilename, InputDirname);
      strcat (InFilename, "/");
      strcat (InFilename, InputDirEntryPtr->d_name);
      // fprintf (stderr, "parsing file %s\n", InFilename);
      yyin = fopen (InFilename, "r");

      // if file open failed, print an error
      if(yyin == NULL)
         perror(InFilename);
   }

   // if yyin is NULL, return 1, else return 0
   return (yyin==NULL);
}

int main(int argc, char **argv)
{
DIR *OutputDirPtr = NULL;
char Command[509];
int ReturnVal;
string MapFilename;
string DictFilename;
string PostFilename;


   ifstream din;
   string stopword;

   din.open("stoplist.txt");
   
   while(!din.eof())
    {
     din >> stopword;
     list.Insert(stopword);
    }
   
   din.close();

   if (argc != 3)
   {
      fprintf (stderr, "Incorrect number of arguments.\n");
      return (1);
   }

   strcpy (InputDirname, argv[1]);
   strcpy (OutputDirname, argv[2]);
   InputDirPtr = opendir (InputDirname);

   // open the input directory
   if (!InputDirPtr)
      fprintf (stderr, "Unable to open input directory: %s\n", InputDirname);
   else
   {
      // open or create the output directory
      OutputDirPtr = opendir (OutputDirname);
      if (!OutputDirPtr)
      {
         strcpy (Command, "mkdir -p ");
         strcat (Command, OutputDirname);
         ReturnVal = system (Command);
      }

      // initialize the input and output file pointers
      yyin = NULL;
      yyout = NULL;
  
      // name and create the inverted files
      MapFilename = (string)OutputDirname+"/map";
      DictFilename = (string)OutputDirname+"/dict";
      PostFilename = (string)OutputDirname+"/post";
      Map.open (MapFilename.c_str());

      // call yywrap and yylex to process the files
      yywrap();
      yylex();
    
      // We are done getting the input, now produce the output

      // close the directories and files
      (void) closedir (InputDirPtr);
      (void) closedir (OutputDirPtr);
      Map.close();
      GlobalHT.PrintDictPost( DictFilename, PostFilename);
   }
}


