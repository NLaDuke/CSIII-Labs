//---------------------------------
// File: diffCheck.cpp
// Written by: Nolan LaDuke
// Date: 1/19/2020
//------------------------------------------------------
// Function: Takes in two files and prints the lines
//           where they are different in the form:
//
//           file1.txt: 20: hello
//           file2.txt: 20: hallo
//                           ^
//------------------------------------------------------

// Authors note: I know a few lines go past 80 characters, but long strings are
//               inevitable here. If you want me to change this in the future
//               please let me know.

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]){

  //Check for correct number of inputs
  if(argc != 3){
    std::cout << "ERROR: Incorrect number of inputs" << std::endl;
    return 0;
  }

  //Establish and open files:
  std::string file1Name = argv[1];
  std::string file2Name = argv[2];
  std::ifstream file1;
  file1.open(file1Name);
  std::ifstream file2;
  file2.open(file2Name);

  //Check to make sure the files opened properly:
  if(!file1.is_open() || !file2.is_open()){
    std::cout << "ERROR: Issue when opening a file, name likely incorrect" << std::endl;
    return 0;
  }

  //Strings to store current line in each file
  std::string file1Line;
  std::string file2Line;

  //Read in first lines
  std::getline(file1, file1Line);
  std::getline(file2, file2Line);

  //String of spaces for formatting
  int offset = 4; //Extra spaces for formatting
  int numberOfSpaces = file1Name.length() + offset;
  std::string spaceStringBase(numberOfSpaces, ' ');

  //Tracker for current line number
  int lineNumber = 1;

  //Loop through files, printing differences until one file ends
  while (!file1.eof() || !file2.eof()){

    //Check for equality, if so then print to terminal
    if (file1Line != file2Line){

      //Calculate position of first difference
      int differencePosition;
      bool differenceFound = false;
      for(int i = 0; i < file1Line.length(); ++i){
        if(file1Line[i] != file2Line[i]){
          //Once difference is found, set up variables and leave loop
          differencePosition = i;
          differenceFound = true;
          break;
        }
      }

      //If it didn't find the difference, it must be a difference in length
      //In that case, the '^' should be at the end of 'file1Line'
      if(!differenceFound){
        differencePosition = file1Line.length();
      }

      //Calculate the number of spaces needed to compensate for the line number
      int numberTracker = lineNumber;
      int numberOfDigits = 0;
      while(numberTracker > 0){
        numberTracker = numberTracker / 10;
        ++numberOfDigits;
      }
      differencePosition += numberOfDigits;

      //Set up a string with all the spaces needed for formatting
      std::string spaceString(differencePosition, ' ');
      spaceString += spaceStringBase;

      //Print with formatting
      std::cout << file1Name << ": " << lineNumber << ": " << file1Line << std::endl;
      std::cout << file2Name << ": " << lineNumber << ": " << file2Line << std::endl;

      //This line adds extra spaces in proportion to the number of digits in 'lineNumber'
      std::cout << spaceString << '^' << std::endl;
    }

    //Increment line counter, read in next lines, and loop back
    ++lineNumber;
    std::getline(file1, file1Line);
    std::getline(file2, file2Line);

  }


  return 0;
}
