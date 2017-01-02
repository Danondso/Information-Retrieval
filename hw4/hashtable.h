/* Filename:  hashtable.h
 * Author:    Susan Gauch
 * Date:      2/25/10
 * Purpose:   The header file for a hash table of strings and ints. 
*/

#include "globalhashtable.h"
using namespace std;

class HashTable {
public:
   HashTable (const HashTable& ht );       // constructor for a copy
   HashTable(const unsigned long NumKeys);          // constructor of hashtable 
   ~HashTable();                           // destructor
   void Print (const char *filename) const;
   void PrintToScreen () const; //print the hashtable to the screen       
   void Insert (const string Key); 
   void Insert (const string Key, const int Data); 
   void Reset ();  // Clear out the hashtable data
   void TransferData(const int DocId, GlobalHashTable &GlobalHT) const;
   int GetData (const string Key); 
   void GetUsage (int &Used, int &Collisions, int &Lookups) const;
   bool Found (const string Key);   // returns true if Key is found  // HW3
protected:
   struct StringIntPair // the datatype stored in the hashtable`
   {
      string key;
      float data;
   };
   unsigned long Find (const string Key); // the index of the key in the hashtable
private:
   StringIntPair *hashtable;        // the hashtable array itself
   unsigned long size;              // the hashtable size
   unsigned long used;
   unsigned long collisions;
   unsigned long lookups;
   unsigned long totaltokens;       //stores total number of tokens
};

