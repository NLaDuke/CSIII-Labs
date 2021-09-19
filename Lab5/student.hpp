#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>
#include <map>

using std::string; using std::getline;
using std::list; using std::vector;
using std::move;


//--------------------------
// File: student.hpp
// Date: 2/16/2021
// Written by: Nolan LaDuke
//------------------------------------------------------------------------------
// Based on a template provided by Professor Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------
// Function: Header and Implementation for Student Class + Functions
//------------------------------------------------------------------------------

class Student{
public:
// Constructors:
   Student(string firstName, string lastName):
      firstName_(firstName), lastName_(lastName) {}

   // move constructor, not really needed, generated automatically
   Student(Student && org):
      firstName_(move(org.firstName_)),
      lastName_(move(org.lastName_))
   {}

   // force generation of default copy constructor
   Student(const Student & org) = default;

// Print Function
  string print() const {
    return firstName_ + " " + lastName_;
  }

// Function Checks if the given name matches this student's name
  bool compareName(string givenName){
    return givenName == firstName_ + " " + lastName_;
  }


// Functions / Operations for auto-generated functionality:
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
};



#endif
