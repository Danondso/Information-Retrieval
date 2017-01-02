//-----------------------------------------------------------------
// File:  testtemplate_taillist.cpp
// Author:  Susan Gauch
// Purpose:  The testing file for a templated linked list class.
//-----------------------------------------------------------------
#include <iostream>
#include "template_taillist.h"
using namespace std;

int main()
{
List <int> IntList;
List <string> NameList;


   cout << "Original List: ";
   IntList.Print();
   cout << endl; 

   IntList.AddSorted (7);
   IntList.AddSorted (5);
   IntList.AddSorted (9);

   cout << "IntList after Adds: ";
   IntList.Print();
   cout << endl;

   NameList.AddToEnd ("Susan");
   NameList.AddToFront ("John");
   
   cout << "NameList after Adds: ";
   NameList.Print();
   cout << endl;

   IntList.Delete (7);
   cout << "Delete 7: ";
   IntList.Print();
   cout << endl;

   NameList.AddSorted ("Brian");
   NameList.Print();
   cout << endl;

   IntList.AddToFront (8);
   cout << "Add To Front 8: ";
   IntList.Print();
   cout << endl;
  
   return (0);
}
