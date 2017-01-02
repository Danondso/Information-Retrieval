//-------------------------------------------
// Filename: posting.cpp
// Implementation of the Time class
//-------------------------------------------
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "posting.h"
using namespace std;

Posting::Posting()
{
   docid = -1;
   tf = -1;
}

Posting::Posting(const int DocId, const float TF)
{
   docid = DocId;
   tf = TF;
}

int Posting::GetDocId() const
{
   return docid;
}

float Posting::GetTF() const
{
   return tf;
}

void Posting::SetPosting(const int DocId, const float TF)
{
   docid = DocId;
   tf = TF;
}

void Posting::Print() const
{
   cout << docid << " " << tf << endl;
}

void Posting::Print(ofstream &Dout) const
{
   Dout << docid << " " << tf << endl;
}

void Posting::Print(ofstream &Dout, const float Idf,
                    const int DocidWidth, const int WtWidth) const
{
int Wt;
    
   Wt = (int) (tf * Idf * 10000);  // produces numbers in range 0 .. 25000

   Dout << setw(DocidWidth) << left << docid << " " 
        << setw(WtWidth) << fixed << right << Wt << endl;
}

