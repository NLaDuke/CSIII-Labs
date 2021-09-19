#include "logger.hpp"
#include <iostream>
#include <string>

using std::string;

//----------------------------
// File: testLogger.cpp
// By: Nolan LaDuke
// Date: 3/10/2021
//------------------------------------------------------------------------------
// Function: Test file for Logger class in 'logger.hpp'
//
// Logger implemented using the Singleton design pattern
//------------------------------------------------------------------------------
// Note: 'Logger' class writes to local file 'log.txt'
//------------------------------------------------------------------------------

// Function to write a default string to file using Logger
void headerWrite(){
  Logger* logptr = Logger::instance();
  logptr->report("Testing Input to the file\n");
}

// Function to take in user input and write to file using Logger
void inputWrite(){
  string input;
  std::cout << "Input a string to write it to the log file: ";
  std::cin >> input;
  std::cout << std::endl;

  Logger* logptr = Logger::instance();
  logptr->report("User Input: ");
  logptr->report(input);
  logptr->report("\n");
}

int main(){
  // Set up pointer to logger:
  Logger* logptr = Logger::instance();

  // Write 'Header' string to file
  std::cout << "Writing header..." << std::endl;
  headerWrite();

  // Take in user input to write to file:
  std::cout << "Writing user input..." << std::endl;
  inputWrite();

  // Write 'Closer' string to file
  std::cout << "Writing closer..." << std::endl;
  Logger::instance() -> report("End of file input\n");

  std::cout << "Tests complete, check 'log.txt' to ensure they passed" << std::endl;

  return 0;
}
