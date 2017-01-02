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
  Posting(const int DocId, const int TF, const int numtokens);

  // getters and setters
  int GetDocId() const;
  int GetTF() const;
  int GetNumTokens();
  void SetPosting(const int DocId, const int TF, const int numtokens);

  // useful methods
  void Print() const;
  void Print(ofstream &Dout, float idf, int totaltermfreq, int numtokens) const;

private:
  int docid, tf, numtokens; 
};
