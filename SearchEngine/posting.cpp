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

void Posting::Print()
{

    ifstream din;
    string temp;
    string docName;
    din.open("map");
    
    din.seekg(0);
    din.seekg(docid * 55);

    din >> temp;

    docName = cutString(temp);

    cout << "<br><a href=http://www.csce.uark.edu/~sgauch/4013-IR/files/" << docName << ">" << docName << "</a> " << "<p1>Weight: </p1>" << tf  << "<br>";

//   cout << docName << endl;
}

 string Posting::cutString(string input)
    {
     int count = 0;
    
    for(int i = input.size(); i > 0; i--)
         {
           if(input[i] == '/')
              return input.substr((i + 1),count);

            count++;
         }
      return input;

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

bool Posting::operator> (const Posting p) const
{
  return (tf > p.tf);
}

bool Posting::operator< (const Posting p) const
{
  return (tf < p.tf);
}
