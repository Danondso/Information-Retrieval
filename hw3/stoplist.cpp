#include "stoplist.h"
#include "hashtable.h"
#include <fstream>


long STOPLIST_SIZE = 250;

stoplist::stoplist(){

  table = new Hashtable(STOPLIST_SIZE);

}


void stoplist::ReadWordList(string file){

   ifstream din;
   string input;
   din.open(file);

	while(!din.eof())
         {
           
          din >> input;
          table.Insert(input);

         }


}
