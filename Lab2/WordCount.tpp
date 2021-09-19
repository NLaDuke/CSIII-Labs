#ifndef WORD_COUNT_TPP
#define WORD_COUNT_TPP

#include <iostream>
#include <string>

//----------------------------------
// File: WordCount.tpp
// By: Nolan LaDuke
// Date: 1/26/2021
//------------------------------------------------------------------------------
// Function: Implementation file for 'WordOccurrence' and 'WordList' classes
//
// Note: 'WordList' class is dependent on 'WordOccurrence' class
//------------------------------------------------------------------------------


//===============Implementation for 'WordOccurrence' class======================


// Match-Word function
// Returns whether or not the given word matches 'word_'
bool WordOccurrence::matchWord(const std::string& newWord) const{
  return newWord == word_;
}

// Getters and Setters:
//------------------------------------------------------------------------------
std::string WordOccurrence::getWord() const{
  return word_;
}

int WordOccurrence::getNum() const{
  return num_;
}
//______________________________________________________________________________

// Increment Function
// Adds to the running number of occurences for the current word
void WordOccurrence::increment(){
  ++num_;
}

// Swap Function
void WordOccurrence::swap(WordOccurrence& rhs){
 // Swap 'word_'
  std::string tempStr = word_;
  word_ = rhs.word_;
  rhs.word_ = tempStr;

 // Swap 'num_'
  int tempInt = num_;
  num_ = rhs.num_;
  rhs.num_ = tempInt;
}

//========================END OF IMPLEMENTATION=================================



//====================Implementation for 'WordList' class=======================

// Constructors:
//------------------------------------------------------------------------------

  // Default:
WordList::WordList() : size_(0){
  wordArray_ = new WordOccurrence[size_];
}

  // Destructor:
WordList::~WordList(){
  delete[] wordArray_;
}

  // Copy:
WordList::WordList(const WordList& rhs){
  size_ = rhs.size_;
  wordArray_ = new WordOccurrence[size_];

  // Iterate through and copy all elements from rhs's array
  for(int i = 0; i < size_; i++){
    wordArray_[i] = rhs.wordArray_[i];
  }
}

//______________________________________________________________________________

// Overloaded Assignment Operator:
// Uses Copy-Swap method
WordList& WordList::operator=(WordList rhs){
  swap(rhs);
  return *this;
}

// Print Function
// Prints each element in the format [ (num_)\t word_ ] separated by new-lines
void WordList::print() const{
  for(int i = 0; i < size_; i++){
    WordOccurrence currentWord = wordArray_[i]; // Temp variable to track current word
    std::cout << "(" << currentWord.getNum() << ")\t" << currentWord.getWord()
              << std::endl;
  }
}

// Equal Friend Function
// Returns 'true' if both WordLists contain identical elements in the same order
bool equal(const WordList& lhs, const WordList& rhs){
 // Check for matching sizes
  if(lhs.size_ != rhs.size_){
    return false;
  }

 // Check that each element in the arrays match
  for(int i = 0; i < lhs.size_; i++){
   //Temp variables to hold current 'words'
    WordOccurrence currentWord1 = lhs.wordArray_[i];
    WordOccurrence currentWord2 = rhs.wordArray_[i];
    if(currentWord1.getNum() != currentWord2.getNum()){
      return false;
    }
    if(currentWord1.getWord() != currentWord2.getWord()){
      return false;
    }
  }
  return true;
}

// Swap Function
void WordList::swap(WordList& rhs){
 // Swap 'size_'
  int temp = size_;
  size_ = rhs.size_;
  rhs.size_ = temp;

 // Swap 'wordArray_'
  WordOccurrence* tempPtr = wordArray_;
  wordArray_ = rhs.wordArray_;
  rhs.wordArray_ = tempPtr;
}

// Add-Word Function
// Takes a word and checks if it is already present in the list
// If it is, increments the count of that word and returns
// Otherwise, resizes array and adds the given word to it with a count of 1
void WordList::addWord(const std::string& newWord){
 // Check if word already contained in list
  for(int i = 0; i < size_; i++){
    if (wordArray_[i].matchWord(newWord)){
      // If we find the word: increment its count and leave
      wordArray_[i].increment();
      return;
    }
  }
 // If the word isn't already in the list, add it
  ++size_;
  WordOccurrence* tempPtr = wordArray_;  //Pointer to track old data
  wordArray_ = new WordOccurrence[size_]; //Set up new array

 // Loop to copy data to new array:
  for(int i = 0; i < size_-1; i++){
    wordArray_[i] = tempPtr[i];
  }

  delete[] tempPtr;                                 //Delete old data
  wordArray_[size_-1] = WordOccurrence(newWord, 1); //Add new element to array

}

// Insertion-Sort Function
// Sorts the elements in the list from Least → Most 'num_' using the
// Insertion Sort algorithm
void WordList::insertionSort(){
 // Loop through each element in 'wordArray_'
  for(int i = 1; i < size_; ++i){
    int j = i;
   // Find location where current element 'fits'
    while (j > 0 && wordArray_[j-1].getNum() > wordArray_[i].getNum()){
      --j;
    }
   // Swap elements to 'Insert' the current element into slot 'j'
    int temp = i;
    while(temp > j){
      wordArray_[temp].swap(wordArray_[temp-1]);
      --temp;
    }
  }
}

//===========================END OF IMPLEMENTATION==============================





#endif
