#include "Agregator.h"


void Agregator::Zero(){
   //Method to run through the array, zeroing everything out.
    for(int i = 0; i < 2000; i++)
       Agregator[i] = 0;
}

//Method to compound the weight of the documents.  
void Agregator::Insert(int docid, int wt){

// if(Agregator[docid] > 0) 
//   cout << Agregator[docid] << " + " << wt << " = ";
   Agregator[docid] += wt;
  // cout << Agregator[docid] << endl;

}
//Gets the non zero accumulator entries and pushes them to a function that stores them in the vector.
void Agregator::FindResults(){

   //Loops through the array, grabbing and pushing data
   for(int i = 0; i < 2000; i++)
      {
       if(Agregator[i] != 0)
        {
        // cout << "Doc ID: " << i << " Weight: " << Agregator[i] << endl;
         SetDW(i, Agregator[i]);
        }
      }

      // cout << "Wts size before sort: " << Wts.size() << endl;
        //Sorts the filled vector
        std::sort(Wts.begin(),Wts.end());

        

//       for(int i = Wts.size() - 1; i >= 0; i--)
  //        cout << "DocID: " << Wts[i].DocID << " Weight " << Wts[i].Wt << endl;

}

//Method that fills the vector
void Agregator::SetDW(int docid, int wt){
    
    DocWt input;
  //Sets the input to the struct
   input.DocID = docid;
   input.Wt = wt;
 //Adds it to the vector
   Wts.push_back(input);

}

void Agregator::Print(){

  //Opens up the map file 
    ifstream din;
    din.open("./output/map");

  //String used for the file name
   string docName;

   // cout << "You're gonna carry that weight, a long time." << endl;

   // cout << Wts.size() << endl;

    for(int i = Wts.size() - 1; i >= Wts.size() - 10; i--)
     {
      //Seeks to the top
      din.seekg(0);
      //Seeks to the line
       din.seekg(Wts[i].DocID * 55);
       //Gets the filename string 
        din >> docName;
       //slices the file name   
       docName = cutString(docName);        

      cout << docName << " Weight:   " << Wts[i].Wt << endl;
      
      }

}

  string Agregator::cutString(string input)
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




