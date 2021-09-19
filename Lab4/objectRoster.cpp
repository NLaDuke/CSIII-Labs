#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>

using std::string; using std::getline;
using std::list; using std::vector;
using std::move;


//--------------------------
// File: objectRoster.cpp
// Date: 2/9/2021
// Written by: Nolan LaDuke
//------------------------------------------------------------------------------
// Based on a template provided by Professor Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------
// Function: Takes in roster files (txt) from the command line, and then forms
//            a list of the form Student Name, Class1, Class2..., ClassN
//            using object-oriented programming
//------------------------------------------------------------------------------
// Requirements: Different Students must have different names
//------------------------------------------------------------------------------


class Student{
public:
// Constructors:
   Student(string firstName, string lastName):
      firstName_(firstName), lastName_(lastName) {}

   // move constructor, not really needed, generated automatically
   Student(Student && org):
      firstName_(move(org.firstName_)),
      lastName_(move(org.lastName_)),
      classes_(move(org.classes_))
   {}

   // force generation of default copy constructor
   Student(const Student & org) = default;

// Print Function
  string print() const {
    string ret;                                        // Running string to return
    string studentName = firstName_ + " " + lastName_; // Student's printed name
   // Print Name w/ special formatting:
   ret += studentName + std::string(40-studentName.length(), ' ');

   // Iterate through classes one at a time, adding them to 'ret'
    for(const string& names : classes_){
      ret += names + " ";
    }

    return ret;
  }

// Function Checks if the given name matches this student's name
  bool compareName(string givenName){
    return givenName == firstName_ + " " + lastName_;
  }

 // Function adds given class to 'classes_'
  void addClass(string newClass){
    classes_.push_back(newClass);
  }


// Functions / Operations for auto-generated functionality:
// Artifact of given file, not used
   // needed for sort()
    friend bool operator< (Student left, Student right){
      return left.lastName_ < right.lastName_ ||
	       (left.lastName_ == right.lastName_ &&
	         left.firstName_ < right.firstName_);
    }

   // needed for unique() and for remove()
    friend bool operator== (Student left, Student right){
      return left.lastName_ == right.lastName_ &&
      left.firstName_ == right.firstName_;
    }


private:
   string firstName_;
   string lastName_;
   list<string> classes_;
};

//========================END OF CLASS AND IMPLEMENTATION=======================


// reading a list from a fileName
void readRoster(list<Student>& roster, string fileName);
// printing a list out
void printRoster(const list<Student>& roster);

int main(int argc, char* argv[]){

 // Check at least 1 file is passed
  if(argc < 2){
    std::cout << "Usage for " << argv[0] << ": "
      << "Lists students in the form: \n Student  Class1 Class2 ... ClassN"
      << std::endl;
    return 0;
  }

 // Declare Variables:
  list<Student> studentRoster; // Running Student Roster

 // Call function to read 1 file at a time, inserting data into 'studentRoster'
  for(int i = 1; i < argc; i++){
    readRoster(studentRoster, argv[i]);
  }

  printRoster(studentRoster);
}


//======================Function Definitions====================================


void readRoster(list<Student>& roster, string fileName){
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
    // Set up variables:
     bool foundStudent = false; // Tracks if student was found in existing roster

    // Iterate through roster one student at a time
     for(Student& student : roster){
      // If the student matches, set up variables, add class to that student, break
       if(student.compareName(first + " " + last)){
         foundStudent = true;
         student.addClass(fileName);
         break;
       }
     }

    // If the student wasn't found, make a new one and add it to the list
     if(!foundStudent){
       Student newStudent(first, last);
       newStudent.addClass(fileName);
       roster.push_back(move(newStudent));
     }
   }

   course.close();
}

// printing a list out
void printRoster(const list<Student>& roster){
   for(const auto& student : roster)
      std::cout << student.print() << std::endl;
}
