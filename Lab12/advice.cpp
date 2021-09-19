
#include <iostream>
#include <string>
#include <vector>
#include "genealogy.hpp"

using std::cout; using std::endl;
using std::string; using std::cin;
using std::vector;

//----------------------------
// File: advice.cpp
// By: Nolan LaDuke
// Date: 4/06/2021
//------------------------------------------------------------------------------
// Function: Test file for functionality of 'Marriage advice' function in
//            genealogy.hpp
//------------------------------------------------------------------------------
// Based on a file provided by Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------

// I'm getting real sick and tired of fixing your memory leaks professor >:(
  // -> I fixed the memory leak, but get some non-fatal errors
  //    when running the code with valgrind, not sure how to fix it so I'll
  //    just finish the lab as is

int main(){
// setting up the genealogical tree
  // the tree is as follows
  //
  //
  //       James Smith  <--spouse-->   Mary
  //	                                  |
  //	                                 children -------------------------
  //	                                  |              |                |
  //	                                  |              |                |
  //	   William Johnson <--spouse-> Patricia      Robert Smith       Linda
  //	                                  |
  //	                                 children------------
  //	                                  |                 |
  //                                     |                 |
  //	   Jennifer  <--spouse-->  Michael Johnson      Barbara
  //	       |
  //	     children
  //	       |
  //          |
  //	     Susan


  // first generation
  Man *js = new Man("Smith", "James", nullptr, nullptr, nullptr);
  Woman *ms = new Woman({}, "Mary", nullptr, nullptr, nullptr);
  ms->setSpouse(js); js->setSpouse(ms);

  // second generation
  Woman *ps = new Woman({}, "Patricia",  nullptr, js, ms);
  Man *wj = new Man("Johnson", "William", nullptr, nullptr, nullptr);
  ps->setSpouse(wj); wj->setSpouse(ps);

  vector<Person *> marysKids  = {ps,
               new Man("Smith", "Robert", nullptr, js, ms),
               new Woman({}, "Linda", nullptr, js, ms)};
  ms->setChildren(marysKids);

  // third generation
  Man *mj = new Man("Johnson", "Michael", nullptr, wj, ps);
  vector<Person *> patsKids   = {mj, new Woman({}, "Barbara", nullptr, wj, ps)};
  ps->setChildren(patsKids);

  Woman *jj = new Woman({}, "Jennifer", nullptr, nullptr, nullptr);
  vector<Person *> jensKids = {new Woman({}, "Susan", nullptr, mj ,jj)};

  jj->setSpouse(mj); mj->setSpouse(jj);
  jj->setChildren(jensKids);


// Start of actual program:
  cout << "It's time to find out if your marriage is legal!" << endl;

// Set up variables:
  string name11; string name12;
  string name21; string name22;

// Prompt user:
  cout << "Enter first candidate [Must be in the family tree]: ";
  cin >> name11; cin >> name12; name11 += " " + name12;
  cout << "Enter second candidate: ";
  cin >> name21; cin >> name22; name21 += " " + name22;

// Setup variables + Call function:
  MarriageChecker* mc = new MarriageChecker(name11, name21);
  js->accept(mc);


// If the first person is not found,
  if(!mc->getFoundPerson()){
    cout << "Sorry, the first person you entered does not exist in this family tree"
         << " and thus their compatability cannot be determined." << endl;
  }

  delete js;
  delete mc;
}
