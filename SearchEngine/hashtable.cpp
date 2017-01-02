/*
 * Author:    Susan Gauch
 * Purpose:   The implementation file for a hash table of words and numbers.
*/

#include <assert.h>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "hashtable.h"
#include "posting.h"
#include "template_taillist.h"

using namespace std;

//---------------------- Constants Added for HW4 ----------------------
const int MAP_REC_SIZE = 32;
const int DICT_REC_SIZE = 59;
const int POST_REC_SIZE = 12;
const int DICT_FILE_RECORDS = 114000;

//---------------------- Functions Added for HW4 ----------------------
// Given an open dict file and a token to look for
// Fill in the Start location in the post file and the
// Number of postings
// Sets Start to -1 and NumDocs to 0 if Key is not found in dict
void GetDictEntry (ifstream &Dict, const string Key, 
                   int &Start, int &NumDocs)
{
unsigned long Sum = 0;
unsigned long Index;
string Token;

   // Calculate the hashvalue for the Key in the Dict file
   for (int i=0; i < Key.length(); i++)
      Sum = (Sum * 19) + Key[i];  // Mult sum by 19, add byte value of char
  
   Index = Sum % DICT_FILE_RECORDS;

   // seek to the top of the dict file
   Dict.seekg(0);
   Dict.seekg(Index * DICT_REC_SIZE);
   Dict >> Token >> NumDocs >> Start;
   while ((Token != Key) && (Token != "EMPTY"))
   {
     // Linear probing in file
     Index = (Index + 1) % DICT_FILE_RECORDS;

     // Wrap around if you hit the end of file 
     if (Index == 0)
        Dict.seekg(0);

     // Read next record
     Dict >> Token >> NumDocs >> Start;
   }
}

//---------------------- Methods Added for HW4 ----------------------

// Name: NumUsed
// Author: S. Gauch
// Parameters: none
// Purpose:	find out how many buckets are used
// Return:	return an int 
int HashTable::NumUsed()
{
    return (used);
}

// Fill the Accumulator HT from the contents of a query HT
// Added for HW4 
void HashTable::SumWeights(HashTable &Accumulator,
                           ifstream &Dict, ifstream &Post) const
{
int Start;
int NumDocs;
string DocId;
float Wt;

   // loop over query HT finding non-empty query terms
   for ( unsigned long i=0; i < size; i++ )
   {  
      if (hashtable[i].key != "")
      {
        // Look the key up in the Dict file
        GetDictEntry (Dict, hashtable[i].key, Start, NumDocs);

        // Add the DocumentWeights from the Post file
        Post.seekg(0);
        Post.seekg(Start*POST_REC_SIZE);
        for (unsigned long j=0; j < NumDocs; j++)
        {
           // Read a posting file entry
           Post >> DocId >> Wt;

           // Multiply the posting docWt by the term frequency in the query
           Accumulator.Insert(DocId, Wt * hashtable[i].data);
        }
      }
   }
}

// Name:  PresentResults
// Author: Susan Gauch
// Parameters:  none
// Purpose:     print the contents of the hash table to screen
//              sorts by weight, shows top N 
// Returns:     nothing
// 
void HashTable::PresentResults(const int NumResults) const
{
List <Posting> ResultsList;
Posting Result;

   // Sort the Results by Weight by adding to a linked list in sorted order
   // Must first convert Docid from string to int to use Posting class unchanged
   for (int i = 0; i < size; i++)
   {
      if (hashtable[i].key != "")
      {
         Result.SetPosting (atoi((hashtable[i].key).c_str()), hashtable[i].data);
         ResultsList.AddSorted(Result, false);  // sort in decreasing order
      }
   }

   ResultsList.Print(10);
}

/*-------------------------- Constructors/Destructors ----------------------*/

/* : HashTable
 * Author: seg
 * Parameters:  ht: the hashtable to copy
 * Purpose:     copy a hashtable 
 *              NOTE:  this is crucial to include since it is invoked
 *              by call-by-value parameter passing
 * Returns:     nothing
*/
HashTable::HashTable( const HashTable &ht )
{
   size = ht.size;                    // set the size of the array
   if((hashtable = new StringFloatPair[size]) == NULL)
       cout << "Out of memory at HashTable::HashTable(const HashTable)" << endl;
   assert( hashtable != 0 );

   for (unsigned long i=0; i < size; i++)     // make a _copy_ of the array elements
   {
      hashtable[i].key = ht.hashtable[i].key;
      hashtable[i].data = ht.hashtable[i].data;
   }
   
}
           
/* Name:  HashTable
 * Author: seg
 * Parameters:  none
 * Purpose:     allocate a hashtable for an expected number of keys
 *              initializes all values to null (0)
 * Returns:     pointer to the created HashTable or 0 if out of memory
*/
HashTable::HashTable(const unsigned long NumKeys)
{
   // allocate space for the table, init to null key
   size = NumKeys * 3;   // we want the hash table to be 2/3 empty
   if((hashtable = new StringFloatPair[size]) == NULL)
      cout << "Out of memory at HashTable::HashTable(unsigned long)" << endl;
   assert( hashtable != 0 );
   Reset();
}

/* Name:  ~HashTable
 * Author: seg
 * Parameters:  none
 * Purpose:     deallocate a hash table
 * Returns:     nothing
*/
HashTable::~HashTable()
{
   delete [] hashtable;
}

/*-------------------------- Accessors ------------------------------------*/


/* Name:  Print
 * Author: seg
 * Parameters:  none
 * Purpose:     print the contents of the hash table
 *              currently, only prints non-null entries
 * Returns:     nothing
*/
void HashTable::Print(const char *filename) const
{
   ofstream fpout(filename); 

   // Print out the non-zero contents of the hashtable
   for ( unsigned long i=0; i < size; i++ )
   {  
      if ( !(hashtable[i].key == ""))
          fpout << hashtable[i].key << " "
                << hashtable[i].data << endl;
   }
   fpout.close();
   cout << "Collisions: " << collisions << ", Used: " << used
        <<  ", Lookups: " << lookups << endl;
}

/* Name:  PrintToScreen
 * Author: Susan Gauch
 * Parameters:  none
 * Purpose:     print the contents of the hash table to screen
 *              currently, only prints non-null entries
 * Returns:     nothing
*/
void HashTable::PrintToScreen() const
{
   // Print out the non-zero contents of the hashtable
   for ( unsigned long i=0; i < size; i++ )
   {  
      if (hashtable[i].key != "")
        cout << hashtable[i].key << " " << hashtable[i].data << endl;
   }

   cout << "Total tokens: " << totaltokens << endl;
}

/* Name: Insert
 * Author: sgauch
 * Parameter:
 *      key : The target of context words to be stored
 * Purpose: Add a word to the hashtable, increment counter
 * Return:  nothing
*/
void HashTable::Insert (const string Key)
{
unsigned long Index;

 if (used >= size)
    cerr << "The hashtable is full; cannot insert.\n";
 else
 {
    totaltokens++;   // HW3

    Index = Find(Key);

    // If not already in the table, insert it
    // Increment the count by 1
    if (hashtable[Index].key == "")
    {
       hashtable[Index].key = Key;
       hashtable[Index].data += 1.0;
       used++;
    }
    // else increment count
    else
       (hashtable[Index].data)++;
 }
}

/* Name: Insert
 * Author: sgauch
 * Parameter:
 * 		key : The target of context words to be stored
 * 		frequency: Total frequency count
 * Purpose: 	insert or add a word with its frequency count in hashtable
 * Return:	nothing
*/
void HashTable::Insert (const string Key, const float Data)
{
unsigned long Index;

 if (used >= size)
    cerr << "The hashtable is full; cannot insert.\n";
 else
 {
    Index = Find(Key);

    // If not already in the table, insert it
    if (hashtable[Index].key == "")
    {
       hashtable[Index].key = Key;
       hashtable[Index].data = Data;
       used++;
    }
    //else add the value to the total so far (hw4)
    else
       hashtable[Index].data += Data;
 }
}

/* Name: Found
 * Author: sgauch
 * Parameters:	key: the string
 * Purpose:	check to see if the key is in the table
 * Return:	return an bool (true if found) 
*/
bool HashTable::Found(const string Key)
{
unsigned long Index;

 lookups++; 
 Index = Find(Key);
 return (hashtable[Index].key != "");
}

/* Name: GetUsage
 * Author: S. Gauch
 * Parameters:	key: the string
 * Purpose:	return the data or -1 if Key is not found
 * Return:	return an int 
*/
int HashTable::GetData(const string Key)
{
unsigned long Index;

 lookups++; 
 Index = Find(Key);
 if (hashtable[Index].key == "")
    return -1;
 else   
    return (hashtable[Index].data);
}

/* Name: GetUsage
 * Author: S. Gauch
 * Parameters:	None
 * Purpose:	return the number of collisions
 * Return:	return a char *
*/
void HashTable::GetUsage(int &Used, int &Collisions, int &Lookups) const
{
   Used = used;
   Collisions = collisions; 
   Lookups = lookups;
}


/*-------------------------- Private Functions ----------------------------*/
/* Name:  Find
 * Author: seg
 * Parameters:  key: the word to be located
 * Purpose:     return the index of the word in the table, or
 *              the index of the free space in which to store the word
 * Returns:     index of the word's actual or desired location
*/
unsigned long HashTable::Find (const string Key) 
{
unsigned long Sum = 0;
unsigned long Index;

   // add all the characters of the key together
   for (int i=0; i < Key.length(); i++)
      Sum = (Sum * 19) + Key[i];  // Mult sum by 19, add byte value of char
  
   Index = Sum % size;

   // Check to see if word is in that location
   // If not there, do linear probing until word found
   // or empty location found.
   while (((hashtable[Index].key) != Key) &&
          ((hashtable[Index].key) != "") ) 
   {
      Index = (Index+1) % size;
      collisions++;
   }
   
   return Index;
}

// Make hashtable empty
void HashTable::Reset()
{
   // initialize the hashtable
   used = 0;
   collisions = 0;
   lookups = 0;
   totaltokens = 0;

   for (unsigned long i=0; i < size; i++)
   {
      hashtable[i].key = "";
      hashtable[i].data = 0;
   }
}

