//-----------------------------------------------------------------
// File:  template_taillist.h
// Author:  Susan Gauch
// Purpose:  The header file for a linked list class.
//           This list is singly-linked, with a tail pointer.
//	     It also uses the TEMPLATE mechanism for the data type
//	     stored in the node.
//-----------------------------------------------------------------

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

template <class T>	// template to provide run-time class for Item
class List
{
public:
// constructors and destructors
   List();                    // default constructor
   List (const List& L);      // copy constructor
   ~List();                   // destructor

// list operations
   void Copy(const List &Other);
   void AddToFront (const T Item);
   void AddToEnd (const T Item);
   void AddSorted (const T Item);
   void Delete(const T Item);

   bool IsEmpty() const;
   void Print() const;
   void Print(ofstream &Dout) const;
   void Print(ofstream &Dout, const float Idf, const int DocidWidth, const int WtWidth) const;
   void Write(const char Filename[]) const;

private:
   struct Node;
   typedef Node* NodePtr;
   struct Node
   {
  	    T Item;
        NodePtr Next;	
   };            
   void Find (const T Item, NodePtr &Curr, NodePtr &Prev) const;
   NodePtr Head;              // the head of the list
   NodePtr Tail;              // the tail of the list
};

//-----------------------------------------------------------------
// For some reason, must have method definitions for templated 
// classes in the same physical file as the header info
//-----------------------------------------------------------------
#include <iostream>
using namespace std;


// ------------------------ constructors and destructors ----------------

//-----------------------------------------------------------------
// Function Name:  The default constructor
// Parameters:  none
// Return Value: none
// Purpose:  Initialize the list to empty
//-----------------------------------------------------------------
template <class T>
List<T>::List()
{
   Head = NULL;
   Tail = NULL;
}

//-----------------------------------------------------------------
// Function Name:  The copy constructor
// Parameters:  none
// Return Value: none
// Purpose:  Initialize the list to be a copy of some other list
//-----------------------------------------------------------------
template <class T>
List<T>::List(const List &Other)
{
   Copy(Other);
};     

//-----------------------------------------------------------------
// Function Name:  The destructor
// Parameters:  none
// Return Value: none
// Purpose:  Delete all the nodes in the list, freeing the space
//-----------------------------------------------------------------
template <class T>
List<T>::~List()
{
NodePtr Temp;

   // loop through whole list deleting nodes
   while (Head != NULL)
   {
      Temp = Head;
      Head = Head->Next;
      delete Temp;
   }
}

// ----------------------- list operations ------------------------------

//-----------------------------------------------------------------
// Function Name: Copy 
// Parameters:  the list to copy
// Return Value: none
// Purpose:  Initialize the list to be a copy of some other list
//-----------------------------------------------------------------
template <class T>
void List<T>::Copy(const List &Other)
{
NodePtr Curr = Other.Head;        // the current node in the original list

   Head = NULL;               // copy is initially empty
   Tail = NULL;
   
   // loop through the original list, copying nodes one at a time
   while (Curr != NULL)
   {
     AddToEnd (Curr->Item);  
     Curr = Curr->Next;
   }
};     

//-----------------------------------------------------------------
// Function Name:  IsEmpty
// Parameters:  none
// Return Value:  true if the list is empty, false otherwise
// Purpose:  Check whether or not the list is empty
//-----------------------------------------------------------------
template <class T>
bool List<T>::IsEmpty() const
{
   return (Head == NULL);
}


//-----------------------------------------------------------------
// Function Name:  AddToFront
// Parameters:  Item - input - the info to be added
// Return Value: none
// Purpose:  Add the new entry to the front of the linked list.
//-----------------------------------------------------------------
template <class T>
void List<T>::AddToFront (const T Item)
{
NodePtr NewNode;

   // make the space for the new node and store the data
   NewNode = new Node;
   NewNode->Item = Item;
   
   // empty list case
   if(Head==NULL) 
      Tail=NewNode;
     
   // point from this node to the previous head of the list
   NewNode->Next = Head;

   // make the head of the list point to this node
   Head = NewNode;
}


//-----------------------------------------------------------------
// Function Name:  AddToEnd
// Parameters:  Item - input - the info to be added
// Return Value: none
// Purpose:  Add the new entry to the tail of the linked list.
//-----------------------------------------------------------------
template <class T>
void List<T>::AddToEnd (const T Item)
{
NodePtr NewNode;

   // make the space for the new node and store the data
   NewNode = new Node;
   NewNode->Item = Item;
   NewNode->Next=NULL;
   
   if(Tail==NULL) 
       Head=NewNode;
   else 
       Tail->Next=NewNode;
   Tail=NewNode;
}


//-----------------------------------------------------------------
// Function Name:  AddSorted
// Parameters:  Item - input - the info to be added
// Return Value: none
// Purpose:  Add the new entry to the linked list, sorted in
//           increasing order.
//-----------------------------------------------------------------
template <class T>
void List<T>::AddSorted (const T Item)
{
NodePtr NewNode, Curr, Prev;

   // First make the new node and store the data 
   NewNode = new Node;
   NewNode->Item = Item;

   // Find the node prior to this one in the list 
   Find (Item, Curr, Prev);

   // If Prev is NULL, we're at the head of the list 
   if (Prev == NULL)
   {
      NewNode->Next = Head;
      Head = NewNode;
   }
   // else, insert between Prev and Curr
   else
   {
      Prev->Next = NewNode;
      NewNode->Next = Curr;
   }

   // If this is the last node in the list, move the tail pointer. 
   if (NewNode->Next == NULL)
      Tail = NewNode;
}


//-----------------------------------------------------------------
// Function Name:  Print
// Parameters:  none
// Return Value: none
// Purpose:  Print the contents of the list to stdout.
//-----------------------------------------------------------------
template <class T>
void List<T>::Print() const
{
cout<<"\n\nThe present contents of the list: "<<endl;
NodePtr Temp = Head;

   // loop through whole list printing nodes
   while (Temp != NULL)
   {
      (Temp->Item).Print();
      Temp = Temp->Next;
   }
}

//-----------------------------------------------------------------
// Function Name:  Print
// Parameters:  Dout:  The file into which to print
// Return Value: none
// Purpose:  Print the contents of the list to Dout.
//-----------------------------------------------------------------
template <class T>
void List<T>::Print(ofstream &Dout) const
{
NodePtr Temp = Head;
   // loop through whole list printing nodes
   while (Temp != NULL)
   {
      (Temp->Item).Print(Dout);
      Temp = Temp->Next;
   }
}

//-----------------------------------------------------------------
// Function Name:  Print - Added for HW3
// Parameters:  Dout:  The file into which to print
//              Idf: The idf to use for term weight calculations 
//              DocIdWidth:  The width of the docid field
//              WtWidth:  The width of the term weight field
// Return Value: none
// Purpose:  Print the contents of the list to Dout.
//-----------------------------------------------------------------
template <class T>
void List<T>::Print(ofstream &Dout, const float Idf,
                    const int DocidWidth, const int WtWidth) const
{
NodePtr Temp = Head;

   // loop through whole list printing nodes
   while (Temp != NULL)
   {
      (Temp->Item).Print(Dout, Idf, DocidWidth, WtWidth);
      Temp = Temp->Next;
   }
}

//-----------------------------------------------------------------
// Function Name:  Delete
// Parameters:  none
// Return Value: none
// Purpose:  Delete a node based on the 'Item' 
//-----------------------------------------------------------------
template <class T>
void List<T>::Delete(const T Item)
{
NodePtr Curr; 
NodePtr Prev;
	
   // Find the node prior to the one that contains Item
   Find (Item, Curr, Prev);
	
   if (Curr == NULL || Curr->Item !=Item)
   { 
      cout<<"\nNo such record to be deleted!"<<endl;
   }

   // Item was found
   else 
   {
      // Reset pointer to Curr 
      if (Curr != Head)
         Prev->Next = Curr->Next;
      else
	 Head = Curr->Next;

      // If Curr was the end of the list
      if (Curr == Tail)
            Tail = Prev;

      delete Curr;
      Curr = NULL;
   }
}

//-----------------------------------------------------------------
// Function Name:  Write
// Parameters:  The file name 
// Return Value: none
// Purpose:  Write the contents back to the file
//-----------------------------------------------------------------
template <class T>
void List<T>::Write(const char FileName[]) const
{
ofstream outfile(FileName,ios::out);
NodePtr Temp = Head;

   // File couldn't be opened
   if (!outfile) 
   {
      cout<<"\nOutput file cannot be opened!"<<endl;
   }
   else
   {
      // loop through whole list printing nodes
      while (Temp != NULL)
      {
         outfile << Temp->Item << endl;
         Temp = Temp->Next;
      }
      outfile.close();
   }
}

// ----------------------- private methods ------------------------------

//-----------------------------------------------------------------
// Function Name:  Find
// Parameters:  Item - input - the Item to be found
//              Curr - output - pointer to the node with Item
//              Curr - output - pointer to the node before Item
// Return Value: none
// Purpose:  Locate a node (It is at Prev->Next)
// Note:  Assumes that > is defined for T.
//-----------------------------------------------------------------
template <class T>
void List<T>::Find (const T Item, NodePtr &Curr, NodePtr &Prev) const
{
   Curr = Head;
   Prev = NULL;

   /* find the proper position in the list */
   while ((Curr != NULL) && (Item > Curr->Item))
   {
      Prev = Curr;
      Curr = Curr->Next;
   }
}

