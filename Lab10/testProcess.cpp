#include "Process.hpp"
#include <iostream>

using std::cout; using std::endl;

//----------------------------
// File: testProcess.cpp
// By: Nolan LaDuke
// Date: 3/25/2021
//------------------------------------------------------------------------------
// Function: File to perform basic tests on classes implemented in 'Process.hpp'
//
//------------------------------------------------------------------------------

int main(){
// Move a procedure through every valid state transition + 1 invalid
  Process testPrc;
  cout << testPrc.report() << endl;
  testPrc.exit();
  cout << testPrc.report() << endl;
  testPrc.admitted();
  cout << testPrc.report() << endl;
  testPrc.schedulerDispatch();
  cout << testPrc.report() << endl;
  testPrc.interrupt();
  cout << testPrc.report() << endl;
  testPrc.schedulerDispatch();
  cout << testPrc.report() << endl;
  testPrc.ioEventWait();
  cout << testPrc.report() << endl;
  testPrc.ioEventCmplt();
  cout << testPrc.report() << endl;
  testPrc.schedulerDispatch();
  cout << testPrc.report() << endl;
  testPrc.exit();
  cout << testPrc.report() << endl;

// Test ID updating properly on other Processes
  Process testPrc2;
  cout << testPrc2.report() << endl;

  Process testPrc3;
  cout << testPrc3.report() << endl;

  testPrc2.admitted();
  cout << testPrc2.report() << endl;

  return 0;
}
