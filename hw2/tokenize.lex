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
#include "hashtable.h"
using namespace std;

char Ch;


//Local and global hash tables and their respective sizes.

const unsigned long L_SIZE = 1000;
const unsigned long G_SIZE = 10000;
HashTable Local_HT(L_SIZE);
HashTable Global_HT(G_SIZE);

int doc_counter = 0;

void Downcase (char *Token)
{
   for (int i = 0; i < strlen(Token); i++)
       if (('A' <= yytext[i]) && ('Z' >= yytext[i]))
          {char c = 'a' + yytext[i] - 'A'; fputc (c, yyout);} 
       else 
          fputc (yytext[i], yyout);
}
%}

UPPERCASE [A-Z]
LOWERCASE [a-z]
LETTER [A-Za-z]
DIGIT [0-9]

%%
[\n\t ]  ;                                      /* White space, consume it */
\<script[^>]*>(.|\n)*<\/script>  ;              /* Remove Scripts */
\<[^>]*\> ;                                     /* Remove HTML tags */
({DIGIT}|{LETTER}){1}            ;              /* Remove single characters */

{DIGIT}{3}"-"{DIGIT}{3}"-"{DIGIT}{4} { Local_HT.Insert(yytext, 0); }                      /* Phone numbers */
({LETTER}|{DIGIT})+@({LETTER}|{DIGIT})+".com" { Local_HT.Insert(yytext, 0); }             /* Email */
("http://"|"www.")({LETTER}|{DIGIT}|"/"|"."|"_"|"~")+ { Local_HT.Insert(yytext, 0); }     /* URL */
{DIGIT}+"."{DIGIT}+ { Local_HT.Insert(yytext, 0); }               /* Decimal numbers */
{DIGIT}+(","{DIGIT}+)+ { Local_HT.Insert(yytext, 0); }            /* Large numbers with commas */

({LETTER}|{DIGIT})+ { Downcase (yytext); Local_HT.Insert(yytext, 0); } /* String */
.              ;   /* Throw away everything else */

%%
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>


//Within the lex rules the tokens are inserted into the hash table.




// global variables for the input directory pointer
// and output directory name for use in yywrap
DIR *InputDirPtr = NULL;
char OutputDirname[500];
char InputDirname[500];

// This is called once per file.
int yywrap()
{
struct dirent* InputDirEntryPtr;

char InFilename[1000];
char OutFilename[1000];

   // if there was an input file open 
   // close it to get ready for the next file 
   if (yyin != NULL)
   {
      fclose (yyin);
      yyin = NULL;
   }

   // if there was an output file open
   // close it to get ready for the next file 
   if (yyout != NULL)
   {
      fclose (yyout);
      yyout = NULL;
   
       for(int i = 0; i < L_SIZE * 3; i++)
        {

          string key = Local_HT.GetKey(i);
          if(key != "")
           Global_HT.InsertGlobal(key, doc_counter, Local_HT.GetData(key));
        } 
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
      strcpy (InFilename, InputDirname);
      strcat (InFilename, "/");
      strcat (InFilename, InputDirEntryPtr->d_name);
      // fprintf (stderr, "parsing file %s\n", InFilename);
      yyin = fopen (InFilename, "r");

      // if file open failed, print an error
      if(yyin == NULL)
         perror(InFilename);

      // open a matching output file and set it to yyout
      strcpy (OutFilename, OutputDirname);
      strcat (OutFilename, "/");
      strcat (OutFilename, InputDirEntryPtr->d_name);
      // fprintf (stderr, "opening output file %s\n", OutFilename);
      yyout = fopen (OutFilename, "w");

//----------------------------------------------------
     //This is where the map file is created
    //Takes the pointer from the file name and outputs it to map
       ofstream dout;
       dout.open("map.txt");
       dout << InputDirEntryPtr->d_name << endl;
       dout.close();
    //-----------------------------------------------------
      // if file open failed, print an error
      if(yyout == NULL)
         perror(OutFilename);
   }

   doc_counter++;

   // if yyin is NULL, return 1, else return 0
   return (yyin==NULL);

}

int main(int argc, char **argv)
{
DIR *OutputDirPtr = NULL;
char Command[509];
int ReturnVal;

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

      // call yywrap and yylex to process the files
      yywrap();
      yylex();

      // close the input and output directories
      (void) closedir (InputDirPtr);
      (void) closedir (OutputDirPtr);


      //Globals print outs for the dict and post files.    

 	Global_HT.PrintPost();     
        Global_HT.PrintDict();

   }
}


