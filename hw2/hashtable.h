/* Filename:  hashtable.h
 * Author:    Susan Gauch
 * Date:      2/25/10
 * Purpose:   The header file for a hash table of strings and ints. 
*/

#include "template_taillist.h"
using namespace std;

class HashTable {
public:
   HashTable (const HashTable& ht );       // constructor for a copy
   HashTable(const unsigned long Size);          // constructor of hashtable 
   ~HashTable();                           // destructor
   void Print (const char *filename) const;       
  
   //Print dict and post methods  

   void PrintPost();
   void PrintDict(); 
   void Insert (const string Key, const int Data); 
   int GetData (const string Key); 
   void GetUsage (int &Used, int &Collisions, int &Lookups) const;
   void InsertGlobal(const string Key, const int id, const int freq);
   string GetKey(int i);

//struct that is inserted into the linked list 
//This hold the document id and frequency

protected:
  struct DocIDTerm
   {
     int doc_ID;
     int Term;
   
   };

protected:
   struct StringIntPair // the datatype stored in the hashtable`
   {
      string key;
      int data;
      //The linked list that is used for the global hash table
      //Local has it too, but it's not used, this goes for data when used for global
      List <DocIDTerm> items;
   };
   unsigned long Find (const string Key); // the index of the ddr in the hashtable
private:
   StringIntPair *hashtable;        // the hashtable array itself
   unsigned long size;              // the hashtable size
   unsigned long used;
   unsigned long collisions;
   unsigned long lookups;
};

