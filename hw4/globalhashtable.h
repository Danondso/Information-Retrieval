/* Filename:  globalhashtable.h
 * Author:    Susan Gauch
 * Date:      10/15/14
 * Purpose:   The header file for a hash table of string + int + list. 
*/

#include "posting.h"
#include "template_taillist.h"

using namespace std;

class GlobalHashTable {
public:
   GlobalHashTable (const GlobalHashTable& ht );       // constructor for a copy
   GlobalHashTable(const unsigned long NumTokens);          // constructor of hashtable 
   ~GlobalHashTable();                           // destructor
   void PrintDictPost (const string DictFilename, 
        const string PostFilename, const int DocumentCount) const;     
   void Insert (const string Token, const int DocId, const float TF); 
   void Reset ();  // Clear out the hashtable data
   void GetUsage (int &Used, int &Collisions, int &Lookups) const;
   bool isEmpty();
   
   //Made the find method public, so the tokens can be hashed and located. 
   unsigned long Find(const string Token); //index of the token in the hashtable
protected:
   struct StringIntList // the datatype stored in the hashtable
   {
      string token;
      int numdocs;
      List <Posting> postings;
   };
  // unsigned long Find (const string Token); // the index of the token in the hashtable
private:
   StringIntList *hashtable;        // the hashtable array itself
   unsigned long size;              // the hashtable size
   unsigned long used;
   unsigned long collisions;
   unsigned long lookups;
};

