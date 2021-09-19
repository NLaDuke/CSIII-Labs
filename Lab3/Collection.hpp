#ifndef LIST_HPP_
#define LIST_HPP_

#include <iostream>

//-----------------------
// File: Collection.hpp
// By: Nolan LaDuke
// Date: 2/2/2021
//------------------------------------------------------------------
// Based on a template provided by Professor Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------
// Function: Header file for templated 'node' and 'Collection' classes
//
// Note: 'Collection' class is dependent on 'node' class and its functions
//------------------------------------------------------------------------------
// Restrictions:
// - all initialized 'node' objects must have an associated 'data_' value
//------------------------------------------------------------------------------

template <typename T>
class node{
public:

  // Constructors:
    node()  : next_(nullptr)    {} // Default Constructor

    node(T item) : node() {data_ = item;} // Data Constructor


  // Accessors:
    T        getData() const  {return data_;}
    node<T>* getNext() const  {return next_;}


  // Mutators:
    void setData(const T& data)   {data_=data;}
    void setNext(node<T> *const next)  {next_=next;}

private:
  // Private Elements:
    T data_;
    node<T> *next_;
};

// Forward Declarations (To allow for friend function to work)

template<typename T>
class Collection;

template<typename T>
bool equal(const Collection<T>&, const Collection<T>&);


template<typename T>
class Collection{
public:
  // Constructors:
    Collection() : first_(nullptr) {} // Default Constructor
    Collection(const Collection& );   // Copy Constructor
    ~Collection();                    // Destructor

  // Accessors:
    void print  () const;
    T    last   () const;

  // Mutators:
    void swap (Collection&);
    void add    (T);
    void remove (const T&);

   // Overloaded Operators:
    Collection& operator=(Collection rhs) {swap(rhs); return *this;};


  // Friend Functions:
    friend bool equal<T>(const Collection&, const Collection&);
        // Note: 'equal' will only return 'True' if both Collections are IDNETICAL
        //        they must contain the exact same elements in the same order

private:
  // Private Elements:
    node<T>* first_; // Pointer to the first element in the Collection

};

//=======================Implementation for 'Collection' class==================


  // Constructors:
//------------------------------------------------------------------------------

// Copy Constructor:
template<typename T>
Collection<T>::Collection(const Collection& rhs) : Collection() {
 // Pointers to track current location in each Collection
  node<T>* trackRhs = rhs.first_;

 // Check for empty-Collection passed
  if(trackRhs == nullptr) return;

  node<T>* trackThis = first_ = new node<T>(trackRhs->getData());
  trackRhs = trackRhs->getNext();

 // Iterate through 'rhs' and copy into this
  while(trackRhs != nullptr){
    trackThis->setNext(new node<T>(trackRhs->getData()));
    trackRhs = trackRhs->getNext();
    trackThis = trackThis->getNext();
  }
}


// Destructor:
template<typename T>
Collection<T>::~Collection(){
  node<T>* nextNode;
  while(first_ != nullptr){
    nextNode = first_->getNext();
    delete first_;
    first_ = nextNode;
  }
}

//______________________________________________________________________________


  // Accessors:
//------------------------------------------------------------------------------

// Print Function:
// Prints the elements stored in the Collection, separated by a space, ending in a new line
// Note: requires that type 'T' has a '<<' operator function
template<typename T>
void Collection<T>::print() const{
  node<T>* nextNode = first_;

 // Check for empty Collection:
  if(first_ == nullptr) return;

 // Iterate through Collection, printing one at a time til there are no more elements
  do{
    std::cout << nextNode->getData() << " ";
  } while ((nextNode = nextNode->getNext()) != nullptr);

  std::cout << std::endl;
}

// 'Last' Function:
// Returns a copy of the last item in the Collection
template<typename T>
T Collection<T>::last() const{
 // Check for empty Collection:
  if(first_ == nullptr){
    std::cerr << "ERROR: Attempted to access element in an empty Collection\n"
              << "       In function call of 'last'"
              << std::endl;
    return T(); // Return a default constructed T
  }

 // Set up variables:
  node<T>* nextNode = first_;

 // Iterate through the Collection until the last element is reached, then return
  while(nextNode->getNext() != nullptr){
    nextNode = nextNode->getNext();
  }

  return nextNode->getData();
}

//______________________________________________________________________________


  // Mutators:
//------------------------------------------------------------------------------

// Swap Function
template<typename T>
void Collection<T>::swap(Collection& rhs){
  node<T>* tempPtr = first_;
  first_ = rhs.first_;
  rhs.first_ = tempPtr;
}

// 'Add' function
// Adds a copy of a given element to the end of the Collection
// Does not check for duplicates
template<typename T>
void Collection<T>::add(T newElem){
 // Check for empty Collection:
  if(first_ == nullptr){
    first_ = new node<T>(newElem);
    return;
  }

 // Set up variables:
  node<T>* nextNode = first_;

 // Iterate through the Collection until the last element is reached
  while(nextNode->getNext() != nullptr){
    nextNode = nextNode->getNext();
  }
  nextNode->setNext(new node<T>(newElem));
  return;
}

// 'Remove' function
// Removes all elements from the Collection that are equal to a given element
template<typename T>
void Collection<T>::remove(const T& givenElem){
  // Check for empty Collection:
   if(first_ == nullptr){
     return;
   }

  // Remove first element until it is not equal to 'givenElem'
  // or there are no more elements in the Collection
   while(first_ != nullptr && first_->getData() == givenElem){
     node<T>* deleteFirst = first_;
     first_ = first_->getNext();
     delete deleteFirst;
   }

  // Set up variables:
   node<T>* prevNode = first_;
   node<T>* currentNode = first_->getNext();
   node<T>* deleteNode;

  // Iterate through, removing matching elements
   while(currentNode != nullptr){
    // Not equal: iterate both pointers
     if(currentNode->getData() != givenElem){
       prevNode = prevNode->getNext();
       currentNode = currentNode->getNext();
     }
    // Equal: remove node, iterate currentNode
     else{
       deleteNode = currentNode;
       currentNode = currentNode->getNext();
       prevNode->setNext(currentNode);
       delete deleteNode;
     }

   }
}


//______________________________________________________________________________



  // Friend Functions:
//------------------------------------------------------------------------------



// 'Equal' function
// Returns a Boolean based on if two Collections are identical
template<typename T>
bool equal(const Collection<T>& lhs, const Collection<T>& rhs){
 // Pointers to track location in each list
  node<T>* trackLHS = lhs.first_;
  node<T>* trackRHS = rhs.first_;

 // Iterate through each Collection until one ends
  while(trackLHS != nullptr && trackRHS != nullptr){
   // Compare data
    if(trackLHS->getData() != trackRHS->getData()) return false;
   // Iterate
    trackLHS = trackLHS->getNext();
    trackRHS = trackRHS->getNext();
  }

 // Check if one ended prematurely
  if(trackLHS != trackRHS) return false;

  return true;
}

//______________________________________________________________________________




//=============================END OF IMPLEMENTATION============================




#endif // LIST_HPP_
