//----------------------------------
// File: WordCount.cpp
// By: Nolan LaDuke
// Date: 1/26/2021
//------------------------------------------------------------------------------
// Function: Takes in a text file from the command line and prints each
//           word in the file based on rarity (least → most occurences)
//
// Node: This function is CASE SENSITIVE
//        ('Test' and 'test' are considered different words)
//------------------------------------------------------------------------------

#include "WordCount.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

int main(int argc, char* argv[]){
 // Check for correct number of inputs
  if(argc != 2){
    std::cout << "ERROR: Incorrect number of inputs" << std::endl;
    return 0;
  }

 // Establish and Open file
  std::string fileName = argv[1];
  std::ifstream inputFile;
  inputFile.open(fileName);

 // Check to make sure the file opened properly:
  if (!inputFile.is_open()){
    std::cout << "ERROR: Issue when opening a file, name likely incorrect" << std::endl;
    return 0;
  }

 // Set up variables
  WordList occCounts = WordList(); // Wordlist to track counts of words
  std::string currentLine;         // String to contain current line in file

 // Loop through the file 1 line at a time
 // At each line, isolate the words and 'add' them to the WordList
  while(!inputFile.eof()){
   // Read in next line
    std::getline(inputFile, currentLine);

    std::string currentWord = ""; //Empty string to contain the current word;

    for(char i : currentLine){
      if(isalnum(i)){
       // If the character is alphanumeric, add it to the running 'word'
        currentWord += i;
      }
      else if(currentWord != ""){
       // Otherwise, If the running 'word' is not empty, add the word to the WordList...
        occCounts.addWord(currentWord);
       // ...and reset the current word tracker
        currentWord = "";
      }
    }

    //Check for words at the end of a line (otherwise won't get accounted for)
    if(currentWord != ""){
      occCounts.addWord(currentWord);
    }

  }

  // Once the file is iterated through, sort the WordList and print it
  std::cout << "The Words in the file and their counts are as follows: "
            << std::endl;
  occCounts.insertionSort();
  occCounts.print();

  return 0;
}
