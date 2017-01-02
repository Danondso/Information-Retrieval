/* Filename:  globalhashtable.cpp
 * Author:    Susan Gauch
 * Date:      10/15/2014
 * Purpose:   The implementation file for a hash table of words, numdocs
 *            and the postings list.
*/

#include <assert.h>
#include <iostream>
#include <fstream>
#include "globalhashtable.h"
#include "math.h"
#include <cstdlib>
#include <string>
#include <sstream>

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
 *              currently, only prints non-null entries
 * Returns:     nothing
*/
void GlobalHashTable::PrintDictPost(const string DictFilename, 
                                    const string PostFilename) const
{
   ofstream Dict;
   ofstream Post;
   unsigned long Start = 0;
   
   Dict.open(DictFilename.c_str());
   Post.open(PostFilename.c_str());

   // Print out the non-zero contents of the hashtable
   for ( unsigned long i=0; i < size; i++ )
   {  

      if ( !(hashtable[i].token == ""))
      {
          
               string tokentemp = hashtable[i].token.substr(0, 20);
             
               
                 if(tokentemp.length() < 20)
                 tokentemp.append(20 - tokentemp.length(), ' ');
               


          stringstream nm;
          nm << hashtable[i].numdocs;
          string numtemp = nm.str();


          if(numtemp.length() >= 5)
               numtemp.substr(0, 5);
             else
               {
                 if(numtemp.length() < 5)
                 numtemp.append(5 - numtemp.length(), ' ');
               }

              stringstream st;
          st << Start;
          string stemp = st.str();


          if(stemp.length() >= 10)
               stemp.substr(0, 10);
             else
               {
                 if(stemp.length() < 10)
                 stemp.append(10 - stemp.length(), ' ');
               }




          Dict << tokentemp << " "
                << numtemp << " "
                << stemp << endl;

       
            float idf = (log(total_docs/hashtable[i].numdocs)/log(2));
        

          (hashtable[i].postings).Print(Post, idf);
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
 * 		TF: Total frequency count
 * Purpose: 	insert or add a word with its frequency count in hashtable
 * Return:	nothing
*/
void GlobalHashTable::Insert (const string Token, const int DocId, const int TF, unsigned long numtokens)
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
   Posting Temp(DocId, TF, numtokens);
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
   }
}

void GlobalHashTable::IncTotalDocs()
{
  total_docs++;
}




