#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>

using std::string; using std::getline;
using std::list; using std::vector;
using std::move;

//---------------------
// File: roster.cpp
// Date: 2/9/2021
// Written by: Nolan LaDuke
//------------------------------------------------------------------------------
// Based on a template provided by Professor Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------
// Function: Takes in roster files (txt) from the command line, and then forms
//            a list of the form Student Name, Class1, Class2..., ClassN
//------------------------------------------------------------------------------
// Requirements: Different Students must have different names
//------------------------------------------------------------------------------



// reading a list from a fileName
void readRoster(list<list<string>>& roster, string fileName);

// printing a list out
void printRoster(const list<list<string>>& roster);



int main(int argc, char* argv[]){

 // Check at least 1 file is passed
  if(argc < 2){
    std::cout << "Usage for " << argv[0] << ": "
      << "Lists students in the form: \n Student  Class1 Class2 ... ClassN"
      << std::endl;
    return 0;
  }

 // Declare variables:
  list<list<string>> studentRoster; // Running student roster

 // Call function to read 1 file at a time, inserting data into 'studentRoster'
  for(int i = 1; i < argc; i++){
    readRoster(studentRoster, argv[i]);
  }

  printRoster(studentRoster);

  return 0;
}


//======================Function Definitions====================================


// reading in a file of names into a roster
void readRoster(list<list<string>>& roster, string fileName){
 // Set up variables:
  std::ifstream course(fileName); //Input File
  string first, last;             //Student Name components

 // Check if file opened:
  if(!course.is_open()){
    std::cerr << "ERROR: Issue when opening file " << fileName;
    std::cerr << ", name likely incorrect" << '\n';
    std::cerr << "The program will continue, but the file will be ignored" << '\n';
    return;
  }

 // Trim Suffix from File Name:
  fileName = fileName.substr(0, fileName.length()-4);

 // Loop through file until no more non-whitespace characters
  while(course >> first >> last){

   // Iterate through roster, adding course name to matching student or adding student if not present
    bool foundStudent;  // Tracks if the student was found in the running roster
    string studentName = first + " " + last;
    for(list<string>& student : roster){
      foundStudent = false;

     // If the student matches, set up variables, add class to that student, break
      if(student.front() == studentName){
        foundStudent = true;
        student.push_back(fileName);
        break;
      }
    }

   // If the student wasn't found, add it to the list
    if(!foundStudent){
      list<string> newStudent;
      newStudent.push_back(move(studentName));
      newStudent.push_back(fileName);
      roster.push_back(move(newStudent));
    }
  }
  course.close();
}

// printing a list out
void printRoster(const list<list<string>>& roster){
 // Iterate through roster 1 student at a time
  for(const list<string>& classList : roster){
   // Iterate through students, 1 class at a time
    for(const string& names : classList){
     // Special formatting for student name
      if(names == classList.front()){
        std::cout << names << std::string(40-names.length(), ' ');
      }
     // General case formatting
      else{
        std::cout << names << " ";
      }
    }
    std::cout << std::endl;
  }
}
