#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>
#include <map>

#include "student.hpp"

using std::string; using std::getline;
using std::list; using std::vector;
using std::move; using std::map;


//--------------------------
// File: associativeRoster.cpp
// Date: 2/16/2021
// Written by: Nolan LaDuke
//------------------------------------------------------------------------------
// Based on a template provided by Professor Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------
// Function: Takes in roster files (txt) from the command line, and then forms
//            a map relating Students to their Classes (a list of strings)
//
//           Then prints that map in the form
//              $ StudentName  _dynamicSpacing_  class1 class2 ... classN
//------------------------------------------------------------------------------
// Requirements: Different Students must have different names
//------------------------------------------------------------------------------


// reading a list from a fileName
void readRoster(map<Student, list<string>>& roster, string fileName);
// printing a list out
void printRoster(const map<Student, list<string>>& roster);

int main(int argc, char* argv[]){

 // Check at least 1 file is passed
  if(argc < 2){
    std::cout << "Usage for " << argv[0] << ": "
      << "Lists students in the form: \n Student  Class1 Class2 ... ClassN"
      << std::endl;
    return 0;
  }

 // Declare Variables:
  map<Student, list<string>> studentRoster; // Running Student Roster

 // Call function to read 1 file at a time, inserting data into 'studentRoster'
  for(int i = 1; i < argc; i++){
    readRoster(studentRoster, argv[i]);
  }

  printRoster(studentRoster);
}


//======================Function Definitions====================================


void readRoster(map<Student, list<string>>& roster, string fileName){
  // Set up variables:
   std::ifstream course(fileName); // Input File
   string first, last;             // Student Name components

 // Check if file opened:
  if(!course.is_open()){
    std::cerr << "ERROR: Issue when opening file " << fileName;
    std::cerr << ", name likely incorrect" << '\n';
    std::cerr << "The program will continue, but the file will be ignored" << '\n';
    return;
  }

 // Trim Suffix from File Name:
  fileName = fileName.substr(0, fileName.length()-4);

 // Read from file until no more non-whitespace characters:
   while(course >> first >> last){
    // Set up list for insert
     list<string> classes;
     classes.push_back(fileName);

    // Attempt to insert into map using Safe_Insert
     auto inst = roster.insert(make_pair(Student(first, last), move(classes)));

    // If the insert failed, add the class name to the list of the existing Student
     if(!inst.second){
        auto itr = roster.find(Student(first, last));
        itr->second.push_back(fileName);
     }
   }

   course.close();
}


// Print out roster of students in the form
// StudentName  _dynamicSpacing_  class1 class2 ... classN
void printRoster(const map<Student, list<string>>& roster){
  // Iterate through map one pair at a time
   for(const auto& stud : roster){
    // Print name of student with special formatting
     string name = stud.first.print();
     std::cout << name << string(40-name.length(), ' ');

    // Iterate through each student's class list one string at a time
     for(string classes : stud.second){
       std::cout << classes << " ";
     }
     std::cout << std::endl;
   }
}
