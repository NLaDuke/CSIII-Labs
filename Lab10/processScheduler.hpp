#ifndef PROCESS_SCHEDULER_HPP
#define PROCESS_SCHEDULER_HPP

#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <deque>
#include "Process.hpp"

using std::cout; using std::endl;


//----------------------------
// File: processScheduler.hpp
// By: Nolan LaDuke
// Date: 3/25/2021
//------------------------------------------------------------------------------
// Function: Scheduler class that manages a queue of 'Process' objects as they
//            transition between states
//------------------------------------------------------------------------------

class Scheduler{
public:
// Constructor:
  Scheduler(int numProcesses = 0){
   // Seed the random number generator based on the current time
    srand(time(NULL));
   // add the provided number of elements, admitting and reporting each
    for(int i = 0; i < numProcesses; i++){
      Process* newProc = templateProcess->clone();
      cout << newProc->report() << endl;
      newProc->admitted();
      cout << newProc->report() << endl;
      pQueue.push(newProc);
    }
   // If there's an element in the queue, dispatch first element
    if(!pQueue.empty()){
      pQueue.front()->schedulerDispatch();
      cout << pQueue.front()->report() << endl;
    }
  }

// Function to add a new process to the scheduler
  void addProcess(){
   // Create new Procedure, admit and report it
    Process* newProc = templateProcess->clone();
    cout << newProc->report() << endl;
    newProc->admitted();
    cout << newProc->report() << endl;
    pQueue.push(newProc);

   // If new element is inserted to front, dispatch it
    if(pQueue.front()->getState() == "ready"){
      pQueue.front()->schedulerDispatch();
      cout << pQueue.front()->report() << endl;
    }
  }

// Function to Update state of scheduler
  void update(){
    if(empty()) return; // If no elements, do nothing

   // Update waitingDeque if needed:
    if(!waitingDeque.empty()) updateWait();

   // Check if queue still empty, if so return
    if(pQueue.empty()) return;

  // Generate random number for what the running element will do
    int willDo = (rand() % 3);
    switch(willDo){
      case(0):{ // Case 0: Exit
       // Exit, Report, and Erase running elem
        pQueue.front()->exit();
        cout << pQueue.front()->report() << endl;
        pQueue.pop();
       // If not empty, start running next elem and report
        if(!pQueue.empty()){
          pQueue.front()->schedulerDispatch();
          cout << pQueue.front()->report() << endl;
        }
        break;
      }
      case(1):{ // Case 1: Interrupt
      // Interrupt and Report running elem, then move to back of queue
        pQueue.front()->interrupt();
        cout << pQueue.front()->report() << endl;
        pQueue.push(pQueue.front());
        pQueue.pop();
      // Start running next elem
        pQueue.front()->schedulerDispatch();
        cout << pQueue.front()->report() << endl;
        break;
      }
      case(2):{ // Case 2: I/O Wait
      // I/O Wait and Report running elem, then move it to waitingDeque
        pQueue.front()->ioEventWait();
        cout << pQueue.front()->report() << endl;
        waitingDeque.push_back(pQueue.front());
        pQueue.pop();
      // If not empty, start running next elem and report
        if(!pQueue.empty()){
          pQueue.front()->schedulerDispatch();
          cout << pQueue.front()->report() << endl;
        }
        break;
      }


    }
  }

// Function for updating state of waitingDeque
  void updateWait(){
   // For each element, 50/50 whether it stays waiting or receives I/O
   //  and is moved back to the queue as 'ready'
    auto itr = std::remove_if(waitingDeque.begin(), waitingDeque.end(),
    [](Process* pr){
      int willDo = rand() % 2;
      return willDo;
    });

  // Iterate through removed elements, Event-Complete -> Report -> Move -> erase
    while(itr != waitingDeque.end()){
      (*itr)->ioEventCmplt();
      cout << (*itr)->report() << endl;
      pQueue.push(*itr);
      itr = waitingDeque.erase(itr);
    }

  }

// Getters:
  bool empty(){return pQueue.empty() && waitingDeque.empty();}

private:
  std::queue<Process*> pQueue;
  std::deque<Process*> waitingDeque;
  Process* templateProcess = new Process(); // Template for Processes to clone
};



#endif
