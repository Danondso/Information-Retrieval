//-----------------------------------------
// Filename: posting.h
// Class to implement a very simple Posting class
//-----------------------------------------
#include <fstream>
using namespace std;

class Posting
{
public:
  // constructors and destructors
  // default constructor
  Posting();
  Posting(const int DocId, const float TF);

  // getters and setters
  int GetDocId() const;
  float GetTF() const;
  void SetPosting(const int DocId, const float TF);

  // useful methods
  void Print();
  void Print(ofstream &Dout) const;
  void Print(ofstream &Dout, const float Idf,
             const int DocIdWidth, const int WtWidth) const;

   string cutString(string input);


   bool operator> (const Posting p) const;
   bool operator< (const Posting p) const;
private:
  int docid;
  float tf; 
};
