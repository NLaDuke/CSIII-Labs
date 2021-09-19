#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>
#include <string>

using std::string;

//----------------------------
// File: logger.hpp
// By: Nolan LaDuke
// Date: 3/10/2021
//------------------------------------------------------------------------------
// Function: header and implementation file for a logger class
//
// Implemented using the Singleton design pattern
//------------------------------------------------------------------------------

class Logger {
public:
 // Constructor for 'atexit()' call
  Logger();

 // Instance Function:
  static Logger* instance();

 // Public Functions:
  void report(const string&);

private:
  // Deletion Function:
   static void cleanUp();

 // Prohibited Methods:
  Logger(const Logger&){};
  Logger& operator=(const Logger&){};

 // Pointer to instance:
  static Logger* loggerInstance;

 // Private Elements:
  std::ofstream outputFile;
};

//==================== Implementation of Logger Class ==========================

// Initialize pointer
Logger* Logger::loggerInstance = nullptr;

// Constructor for calling 'cleanup' at exit
Logger::Logger(){
  atexit(cleanUp);
}

// Instance function
Logger* Logger::instance(){
 // On first call, set up object + fstream
  if(loggerInstance == nullptr){
    loggerInstance = new Logger;
    loggerInstance->outputFile.open("log.txt", std::fstream::app);
  }
  return loggerInstance;
}

// Report Function:
// Appends the provided string to 'log.txt'
void Logger::report(const string& appendString){
  loggerInstance->outputFile << appendString;
}

// Cleanup Function:
// Closes file and flushes buffer
void Logger::cleanUp(){
  loggerInstance->outputFile.close();
  delete loggerInstance;
}

#endif
