
#include <iostream>
#include <thread>
#include <algorithm>
#include <fstream>
#include <string>
using namespace std;

//Agregator class to accumulate the weights

class Agregator{

public:
 void Zero(); //Clears the array
 void Insert(int docid, int wt); //Inserts the weight into the array, using the index as the doc id
 void FindResults(); //Finds the results from the accumulator array, pushes them into a vector, and sorts the vector
 void SetDW(int docid, int wt); //Sets the doc id and weight within the DocWt struct 
 void Print(); //Prints out the top ten results from the vector
 string cutString(string input);

 //struct to hold the doc id and the weight 
struct DocWt{
  
  int DocID;
  int Wt;
  //overloaded method used to compare values in the struct
   //used by the sort method. 
  bool operator <(const DocWt &a) const
     { return Wt < a.Wt; }
}; 

private:
int Agregator[2000]; //initial accumulation location, uses indexes as the doc id and 
 			//stores the weight of the document
std::vector<DocWt> Wts; //Used to store the struct that holds the doc id and the weight. 
};
