//-------------------------------------------
// Filename: posting.cpp
// Implementation of the Time class
//-------------------------------------------
#include <iostream>
#include <iomanip>
#include "posting.h"
#include <sstream>
using namespace std;

Posting::Posting()
{
   docid = -1;
   tf = -1;
   numtokens = -1;
}

Posting::Posting(const int DocId, const int TF, const int totaltokens)
{
   docid = DocId;
   tf = TF;
   numtokens = totaltokens;
}

int Posting::GetDocId() const
{
   return docid;
}

int Posting::GetTF() const
{
   return tf;
}

int Posting::GetNumTokens(){

return numtokens;

}


void Posting::SetPosting(const int DocId, const int TF, const int totaltokens)
{
   docid = DocId;
   tf = TF;
   numtokens = totaltokens;
}


void Posting::Print() const
{
   cout << docid << " " << tf << endl;
}

void Posting::Print(ofstream &Dout, float idf, int totaltermfreq, int numtokens) const
{

   stringstream di;
          di << docid;
          string doctemp = di.str();

  stringstream ft;
          ft << docid;
          string fttemp = ft.str();

   if(doctemp.length() > 10)
     doctemp.substr(0, 10);
    else
     if(doctemp.length() < 10)
      doctemp.append(10 - doctemp.length(),' ');
   if(fttemp.length() > 10)
     fttemp.substr(0, 10);
    else
     if(fttemp.length() < 10)
       fttemp.append(10 - fttemp.length(), ' ');

   Dout << doctemp << " " << fttemp << " " << std::fixed << std::setprecision(5) << ((idf * totaltermfreq)/numtokens) << endl;



}
