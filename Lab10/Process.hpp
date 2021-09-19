#ifndef PROCESS_HPP
#define PROCESS_HPP

#include<string>

using std::string;

//----------------------------
// File: Process.hpp
// By: Nolan LaDuke
// Date: 3/25/2021
//------------------------------------------------------------------------------
// Function: Implementation of five state model through use of State Design
//            Pattern as a Process interface
//------------------------------------------------------------------------------

// AUTHOR NOTE:
// There is a small memory leak detected through valgrind,
// I can only assume this is has something to do
// with the Meyer's singleton implementation with pointers instead of references
// used by the State classes. I tried using virtual destructors to delete the pointers
// but got some strange linker errors.

// Forward declaration of State class for use in Process
class State;

// Process class acts as interface through which to manage state
class Process{
public:
  Process();

// Clone Function for Prototyping
  Process* clone();

// State Changes:
  void admitted();
  void schedulerDispatch();
  void interrupt();
  void ioEventWait();
  void ioEventCmplt();
  void exit();

  void changeState(State* newState){state_ = newState;}

// Report State:
  string report(); // Print out the current state of the process

// Getters:
  int getID(){return ID_;};
  string getState();

private:
  State* state_; // Current State of process
  int ID_; // ID to identify Processes
};

// State
class State{
public:
  virtual ~State(){};

// Virtual State Changes w/ default implementation of doing nothing
  virtual void admitted           (Process*){};
  virtual void schedulerDispatch  (Process*){};
  virtual void interrupt          (Process*){};
  virtual void ioEventWait        (Process*){};
  virtual void ioEventCmplt       (Process*){};
  virtual void exit               (Process*){};

// Pure Virtual Report Function
  virtual string report()=0;

// Change State function
  void changeState(Process* pr, State* st){
    pr->changeState(st);
  }
};

//======================== Classes for Each State ==============================
//   Classes implemented using Meyer's Singleton to minimize data redundancy

class newPrc : public State{
public:

// Instance Getter Function:
  static State* instance(){
      static State* onlyInstance = new newPrc;
      return onlyInstance;
  }

// State Transition Functions:
  void admitted(Process*) override;

// Report Function:
  string report() override {return "new";};

private:
// Privated Constructors:
  newPrc(){};
  newPrc(const newPrc&){};
  newPrc& operator=(const newPrc&){};
};

class readyPrc : public State{
public:
// Instance Getter Function:
  static State* instance(){
      static State* onlyInstance = new readyPrc;
      return onlyInstance;
  }

// State Transition Functions:
  void schedulerDispatch(Process*) override;

// Report Function:
  string report() override {return "ready";};

private:
// Privated Constructors:
  readyPrc(){};
  readyPrc(const readyPrc&){};
  readyPrc& operator=(const readyPrc&){};
};

class runningPrc : public State{
public:
// Instance Getter Function:
  static State* instance(){
      static State* onlyInstance = new runningPrc;
      return onlyInstance;
  }

// State Transition Functions:
  void interrupt(Process*) override;
  void ioEventWait(Process*) override;
  void exit(Process*) override;

// Report Function:
  string report() override {return "running";};

private:
// Privated Constructors:
  runningPrc(){};
  runningPrc(const runningPrc&){};
  runningPrc& operator=(const runningPrc&){};
};

class waitingPrc : public State{
public:
// Instance Getter Function:
  static State* instance(){
      static State* onlyInstance = new waitingPrc;
      return onlyInstance;
  }

// State Transition Functions:
  void ioEventCmplt(Process*) override;

// Report Function:
  string report() override {return "waiting";};

private:
// Privated Constructors:
  waitingPrc(){};
  waitingPrc(const waitingPrc&){};
  waitingPrc& operator=(const waitingPrc&){};
};

class terminatedPrc : public State{
public:
// Instance Getter Function:
  static State* instance(){
      static State* onlyInstance = new terminatedPrc;
      return onlyInstance;
  }

// State Transition Functions:

// Report Function:
  string report() override {return "terminated";};

private:
// Privated Constructors:
  terminatedPrc(){};
  terminatedPrc(const terminatedPrc&){};
  terminatedPrc& operator=(const terminatedPrc&){};
};

//===================== State Class Implementations ============================
// newPrc Implementations
void newPrc::admitted(Process* pr){
  changeState(pr, readyPrc::instance());
}

// readyPrc Implementations
void readyPrc::schedulerDispatch(Process* pr){
  changeState(pr, runningPrc::instance());
}

// runningPrc Implementations
void runningPrc::interrupt(Process* pr){
  changeState(pr, readyPrc::instance());
}

void runningPrc::ioEventWait(Process* pr){
  changeState(pr, waitingPrc::instance());
}

void runningPrc::exit(Process* pr){
  changeState(pr, terminatedPrc::instance());
}

// waitingPrc Implementations
void waitingPrc::ioEventCmplt(Process* pr){
  changeState(pr, readyPrc::instance());
}

//===================== Process Class Implementations ==========================

// Constructor:
Process::Process(){
  static int nextID_ = 0;
  ID_ = nextID_++;
  state_ = newPrc::instance();
}

// Clone Function:
Process* Process::clone(){
  Process* tempPtr = new Process();
  tempPtr->changeState(state_);
  return tempPtr;
}

// State Transition Implementations
void Process::admitted()          {state_->admitted(this);};
void Process::schedulerDispatch() {state_->schedulerDispatch(this);};
void Process::interrupt()         {state_->interrupt(this);};
void Process::ioEventWait()       {state_->ioEventWait(this);};
void Process::ioEventCmplt()      {state_->ioEventCmplt(this);};
void Process::exit()              {state_->exit(this);};

// Report Function
string Process::report(){
  return "Process " + std::to_string(ID_) + " is " + state_->report();
}

// State Getter
string Process::getState(){
  return state_->report();
}

#endif
