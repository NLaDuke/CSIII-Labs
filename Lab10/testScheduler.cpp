#include <iostream>
#include "processScheduler.hpp"

using std::cout; using std::endl;

//----------------------------
// File: testScheduler
// By: Nolan LaDuke
// Date: 3/25/2021
//------------------------------------------------------------------------------
// Function: File to perform basic tests on classes implemented
//            in 'processScheduler.cpp'
//------------------------------------------------------------------------------

int main(){
  Scheduler newSched(4);
  while(!newSched.empty()){
    cout << endl;
    newSched.update();
  }
  return 0;
}
