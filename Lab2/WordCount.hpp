#ifndef WORD_COUNT_HPP
#define WORD_COUNT_HPP

#include <string>

//----------------------------------
// File: WordCount.hpp
// By: Nolan LaDuke
// Date: 1/26/2021
//------------------------------------------------------------------------------
// Function: Header file for 'WordOccurrence' and 'WordList' classes
//
// Note: 'WordList' class is dependent on 'WordOccurrence' class
//------------------------------------------------------------------------------



class WordOccurrence {
public:
  // Constructors:
    WordOccurrence(const std::string& word="", int num=0)
                  : word_(word), num_(num) {};

  // Accessors:
    bool matchWord(const std::string &) const; // returns true if word matches stored

   // Getters:
    std::string getWord() const;  // word_ getter
    int getNum() const;           // num_ getter

  // Mutators:
    void increment(); // increments number of occurrences of a word
    void swap(WordOccurrence&);

private:
    std::string word_;
    int num_;
};

class WordList{
public:
  // Constructors:
    WordList();
    ~WordList();
    WordList(const WordList&);

  // Overloaded Operators:
    WordList& operator=(WordList);

  // Accessors:
    void print() const; // Prints each element in the format [ (num_)\t word_ \n ]

  // Friend Functions:
    friend bool equal(const WordList&, const WordList&);
      //Note: 'equal' will only return true if both WordLists are IDENTICAL
      //     if two lists with identical elements in different orders are passed
      //     the function will return FALSE

  // Mutators:
    void swap(WordList&);
    void addWord(const std::string &); // Adds element to list || increments if present
    void insertionSort(); // Sorts the elements in the list Least → Most occurences

private:
    WordOccurrence *wordArray_; // a dynamically allocated array of WordOccurrences
                                // may or may not be sorted
    int size_;
};

#include "WordCount.tpp"
#endif
