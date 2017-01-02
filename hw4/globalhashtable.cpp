/* Filename:  globalhashtable.cpp
 * Author:    Susan Gauch
 * Date:      10/15/2014
 * Purpose:   The implementation file for a hash table of words, numdocs
 *            and the postings list.
*/

#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <math.h>

#include "globalhashtable.h"

using namespace std;
/*-------------------------- Constructors/Destructors ----------------------*/

/* GlobalHashTable
 * Author: seg
 * Parameters:  ht: the hashtable to copy
 * Purpose:     copy a hashtable 
 *              NOTE:  this is crucial to include since it is invoked
 *              by call-by-value parameter passing
 * Returns:     nothing
*/
GlobalHashTable::GlobalHashTable( const GlobalHashTable &ht )
{
   size = ht.size;                    // set the size of the array
   if((hashtable = new StringIntList[size]) == NULL)
       cout << "Out of memory at GlobalHashTable::GlobalHashTable(const GlobalHashTable)" << endl;
   assert( hashtable != 0 );

   for (unsigned long i=0; i < size; i++)     // make a _copy_ of the array elements
   {
      hashtable[i].token = ht.hashtable[i].token;
      hashtable[i].numdocs = ht.hashtable[i].numdocs;
      
      (hashtable[i].postings).Copy(ht.hashtable[i].postings);
   }
   
}
           
/* Name:  GlobalHashTable
 * Author: seg
 * Parameters:  none
 * Purpose:     allocate a hashtable for an expected number of token
 *              initializes all values to null (0)
 * Returns:     pointer to the created GlobalHashTable or 0 if out of memory
*/
GlobalHashTable::GlobalHashTable(const unsigned long NumTokens)
{
   // allocate space for the table, init to null token
   size = NumTokens * 3;   // we want the hash table to be 2/3 empty
   if((hashtable = new StringIntList[size]) == NULL)
      cout << "Out of memory at GlobalHashTable::GlobalHashTable(unsigned long)" << endl;
   assert( hashtable != 0 );
   Reset();
}

/* Name:  ~GlobalHashTable
 * Author: seg
 * Parameters:  none
 * Purpose:     deallocate a hash table
 * Returns:     nothing
*/
GlobalHashTable::~GlobalHashTable()
{
   delete [] hashtable;
}

/*-------------------------- Accessors ------------------------------------*/


/* Name:  PrintDictPost
 * Author: seg
 * Parameters:  none
 * Purpose:     print the contents of the hash table to dict and post
 *              outputs fixed length records
 * Returns:     nothing
*/
void GlobalHashTable::PrintDictPost(const string DictFilename, 
                                    const string PostFilename,
                                    const int DocumentCount) const
{
   // New constants for HW3
   const int TOKEN_LEN = 45;
   const int NUMDOC_LEN = 4;
   const int START_LEN = 7;
   const int DOCID_LEN = 4;
   const int WT_LEN = 6;
   const int THRESHOLD = 2;
   float Idf;

   ofstream Dict;
   ofstream Post;
   unsigned long Start = 0;
   
   Dict.open(DictFilename.c_str());
   Post.open(PostFilename.c_str());

   // Print out the contents of the hashtable
   for ( unsigned long i=0; i < size; i++ )
   {  
      // Print out the empty records and replace low-freq entries with blank records
      if (hashtable[i].token == "") 
      {
         Dict << setw(TOKEN_LEN) << left << "EMPTY" << " " 
              << setw(NUMDOC_LEN) << right << 0 << " "
              << setw(START_LEN) << right << -1 << endl;
      }
      else if (hashtable[i].numdocs <= THRESHOLD)
      {
         Dict << setw(TOKEN_LEN) << left << "DELETED" << " " 
              << setw(NUMDOC_LEN) << right << 0 << " "
              << setw(START_LEN) << right << -1 << endl;
      }
      else  // it is a good token; print out dict and post entries
      {
         // truncate tokens that are too long
         if ((hashtable[i].token).length() > TOKEN_LEN)
            (hashtable[i].token).resize(TOKEN_LEN);
         
         // calculate idf; add 1 to avoid possible divbyzero errors later
         Idf = 1 + log (DocumentCount/hashtable[i].numdocs);

         // output the Dict data  with fixed sized records
         Dict << setw(TOKEN_LEN) << left << hashtable[i].token << " " 
              << setw(NUMDOC_LEN) << right << hashtable[i].numdocs << " "
              << setw(START_LEN) << right << Start << endl;

         // output the postings to Post with fixed size records
         // calls new Print method for HW3
         (hashtable[i].postings).Print(Post, Idf, DOCID_LEN, WT_LEN);

         Start = Start + hashtable[i].numdocs;
      }
   }

   Dict.close();
   Post.close();
   cout << "Collisions: " << collisions << ", Used: " << used
        <<  ", Lookups: " << lookups << endl;
}

/* Name: Insert
 * Author: sgauch
 * Parameter:
 * 		Token : The target of context words to be stored
 * 		DocId: The document whose data is being inserted
 * 		RTF: Relative frequency count
 * Purpose: 	insert or add a word with its frequency count in hashtable
 * Return:	nothing
*/
void GlobalHashTable::Insert (const string Token, const int DocId, const float RTF)
{
unsigned long Index;

 if (used >= size)
    cerr << "The global hashtable is full; cannot insert.\n";
 else
 {
    Index = Find(Token);

    // If not already in the table, insert it
    if (hashtable[Index].token == "")
    {
       hashtable[Index].token = Token;
       hashtable[Index].numdocs = 1;
       used++;
    }
    // else do nothing
    else
       (hashtable[Index].numdocs)++;

   // finally, add docid, tf to list
   Posting Temp(DocId, RTF);
   (hashtable[Index].postings).AddToEnd(Temp);
 }
}

/* Name: GetUsage
 * Author: S. Gauch
 * Parameters:	None
 * Purpose:	return the number of collisions
 * Return:	return a char *
*/
void GlobalHashTable::GetUsage(int &Used, int &Collisions, int &Lookups) const
{
   Used = used;
   Collisions = collisions; 
   Lookups = lookups;
}


/*-------------------------- Private Functions ----------------------------*/
/* Name:  Find
 * Author: seg
 * Parameters:  token: the word to be located
 * Purpose:     return the index of the word in the table, or
 *              the index of the free space in which to store the word
 * Returns:     index of the word's actual or desired location
*/
unsigned long GlobalHashTable::Find (const string Token) 
{
unsigned long Sum = 0;
unsigned long Index;

   // add all the characters of the token together
   for (int i=0; i < Token.length(); i++)
      Sum = (Sum * 19) + Token[i];  // Mult sum by 19, add byte value of char
  
   Index = Sum % size;

   // Check to see if word is in that location
   // If not there, do linear probing until word found
   // or empty location found.
   while (((hashtable[Index].token) != Token) &&
          ((hashtable[Index].token) != "") ) 
   {
      Index = (Index+1) % size;
      collisions++;
   }
   
   return Index;
}

//Checks to see if the HashTable is empty
 
bool GlobalHashTable::isEmpty(){

  if(used == 0)
     return true;

  return false;

}





// Make hashtable empty
void GlobalHashTable::Reset()
{
   // initialize the hashtable
   used = 0;
   collisions = 0;
   lookups = 0;

   for (unsigned long i=0; i < size; i++)
   {
      hashtable[i].token = "";
      hashtable[i].numdocs = 0;
      // Free the list (memory leak!!!)
      // and set list to NULl
   }
}
