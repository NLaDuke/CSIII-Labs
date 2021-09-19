
#include <iostream>
#include <string>
#include <vector>
#include "genealogy.hpp"

using std::cout; using std::endl;
using std::string;
using std::vector;

//----------------------------
// File: genealogyUse.cpp
// By: Nolan LaDuke
// Date: 4/06/2021
//------------------------------------------------------------------------------
// Function: Test file of visitors for genealogy.hpp
//
//------------------------------------------------------------------------------
// Based on a file provided by Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------

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


   // defining two visitors
   ChildrenPrinter *cp = new ChildrenPrinter;
   NamePrinter *np = new NamePrinter;
   MaidenPrinter* mp = new MaidenPrinter;

   // executing the traversal with the composite
   // and the specific visitor

   cout << "\nNAME LIST\n";
   ms->accept(np);

   cout << endl << endl;

   cout << "CHILDREN LIST\n";
   ms->accept(cp);

   cout << "\nJAMES' CHILDREN\n";
   js->accept(cp);

   cout << "\nALL MAIDEN NAMES\n";
   ms->accept(mp);

   delete cp;
   delete np;
   delete mp;
   delete js;
}
