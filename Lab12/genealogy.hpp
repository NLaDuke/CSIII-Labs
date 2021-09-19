#ifndef GENEALOGY_HPP
#define GENEALOGY_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using std::cout; using std::endl;
using std::string;
using std::vector;

//----------------------------
// File: genealogy.hpp
// By: Nolan LaDuke
// Date: 4/06/2021
//------------------------------------------------------------------------------
// Function: Visitor + Composite DP implementation of family tree
//
//------------------------------------------------------------------------------
// Based on a file provided by Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------

//======================== Composite DP Classes ================================

class Person{ // component
public:
   Person(string firstName, Person *spouse, Person *father, Person *mother):
      firstName_(firstName), spouse_(spouse), father_(father), mother_(mother){}
   const string & getFirstName(){return firstName_;}
   Person *getSpouse(){return spouse_;}
   void setSpouse(Person *spouse){spouse_=spouse;}
   Person *getFather(){return father_;}
   Person* getMother(){return mother_;}

   virtual const string& getLastName()=0;

   virtual void accept(class PersonVisitor *)=0;
   virtual void accept(class MarriageChecker*)=0;
   virtual ~Person(){}
   virtual void deleteSelf()=0;
private:
   const string firstName_;
   Person *spouse_;
   Person *father_;
   Person *mother_;
};


// leaf
// man has a last name
class Man: public Person{
public:
   Man(string lastName, string firstName, Person *spouse,
       Person *father, Person *mother):
      lastName_(lastName),
      Person(firstName, spouse, father, mother){}

   ~Man(){
     if(getSpouse() != nullptr){
       getSpouse()->deleteSelf();
     }
   }

 // Special function to ensure deletion of every element without seg-faults
   void deleteSelf() override{
     if(!deleted){
       deleted = true;
       delete this;
     }
   }

   const string & getLastName(){return lastName_;}
   void accept(class PersonVisitor *visitor) override;
   void accept(class MaidenPrinter *visitor);
   void accept(class MarriageChecker* visitor);
private:
   const string lastName_;
   bool deleted = false;
};

// composite
// woman has a list of children
class Woman: public Person{
public:
   Woman(vector<Person *> children,
       string firstName,
       Person *spouse,
       Person *father, Person *mother):
       children_(children),
       Person(firstName, spouse, father, mother){}

   ~Woman(){
     if(getSpouse() != nullptr){
         getSpouse()->deleteSelf();
     }
     for(auto child : children_){
         child->deleteSelf();
     }
   }

   const string& getLastName() override{
     if (getSpouse() != nullptr)
        return static_cast<Man *>(getSpouse())->getLastName();
     else if (getFather() != nullptr)
        return static_cast<Man *> (getFather())->getLastName();
     else
        return defaultLName;
   }

 // Special function to ensure deletion of every element without seg-faults
   void deleteSelf() override{
     if(!deleted){
       deleted = true;
       delete this;
     }
   }

   const vector<Person *> & getChildren() {return children_;}
   void setChildren(const vector<Person *> &children){ children_ = children;}
   void accept(class PersonVisitor *visitor) override;
   void accept(class MarriageChecker* visitor);

private:
   vector<Person *> children_;
   string defaultLName = "[Not-Available]";
   bool deleted = false;
};

//=========================== Visitor DP Classes ===============================

// abstract visitor
class PersonVisitor{
public:
   virtual void visit(Man*)=0;
   virtual void visit(Woman*)=0;
   virtual ~PersonVisitor(){}
};

// composite methods that define traversal
void Man::accept(PersonVisitor *visitor) {
    visitor->visit(this);
}

void Woman::accept(PersonVisitor *visitor){
   // children traversal through mother only
   // father's children are not traversed so as not
   // to traverse them twice: for mother and father

   visitor->visit(this);
   // traversing descendants
   for(auto child : children_)
      child->accept(visitor);


}

// concrete visitors
// the last name for a man is stored in object
// the last name a woman is determined by her
// spouse if she is married
// or by her father if she is not
class NamePrinter: public PersonVisitor{
public:
   void visit(Man *m) override {
      cout << m->getFirstName() << " " << m->getLastName() << endl;
   }
   void visit(Woman *w) override {
      cout << w->getFirstName() << " ";
      if (w->getSpouse() != nullptr)
	 cout << static_cast<Man *> (w->getSpouse())->getLastName();
      else if (w->getFather() != nullptr)
	 cout << static_cast<Man *> (w->getFather())->getLastName();
      else
	 cout << "Doe";
      cout << endl;
   }
};


class ChildrenPrinter: public PersonVisitor{
public:
   void visit(Man *m) override {
      cout << m->getFirstName() << ": ";
      Woman *spouse = static_cast<Woman *>(m->getSpouse());
      if(spouse != nullptr)
	 printNames(spouse->getChildren());
      cout << endl;
   }
   void visit(Woman *w) override {
      cout << w->getFirstName() << ": ";
      printNames(w->getChildren());
      cout << endl;
   }
private:
   void printNames(const vector<Person *> &children){
      for(const auto c: children)
	 cout << c->getFirstName() << ", ";
   }
};


// Prints all women in the form FirstName MaidenName
// If there is no recorded father, her maiden name is printed as 'Not-Available'
class MaidenPrinter: public PersonVisitor{
public:
// Visitor Functions:

 // Visitor Case: Man
  void visit(Man* guy) override {
    // Call accept to get special traversal implementation
    guy->accept(this);
  }

 // Visitor Case: Woman
  void visit(Woman* girl) override {
    cout << girl->getFirstName() << " ";
    if (girl->getFather() != nullptr)
      cout << static_cast<Man *> (girl->getFather())->getLastName();
    else
      cout << "[Not-Available]";
    cout << endl;
  }

private:

};


// Special traversal for Maiden printer: if the guy has a spouse, accept it
void Man::accept(MaidenPrinter *visitor) {
    if(getSpouse() != nullptr){
      static_cast<Woman*>(getSpouse())->accept(visitor);
    }
}


// Marriage Checker : Given two names, will find if they are compatible / existent
//  Note: If it finds the first person in the tree, but does not find 'marrying' it will print as a valid marriage
class MarriageChecker : PersonVisitor{
public:
 // Constructor to store names of candidates
  MarriageChecker(string name = "", string name2 = "") : nameToFind(name), marrying(name2){};

// Visitors:
  void visit(Man* guy) override{
    if(checkName(guy) && !foundPerson){
      foundPerson = true;
     // It's them, check qualifications and print accordingly
      if(checkAll(guy)){
        cout << "The marriage is compatible" << endl;
        return;
      }
      cout << "The marriage is not compatible" << endl;
      return;
    }
  }

  void visit(Woman* girl) override{
    if(checkName(girl) && !foundPerson){
      foundPerson = true;
     // It's them, check qualifications and print accordingly
      if(checkAll(girl)){
        cout << "The marriage is compatible" << endl;
        return;
      }
      cout << "The marriage is not compatible" << endl;
      return;
    }
  }

 // Returns whether or not they found the first person
  bool getFoundPerson(){return foundPerson;}

private:

// Checker used in initial traversal
bool checkName(Person* person){
  return person->getFirstName() + " " + person->getLastName() == nameToFind;
}

// Checker to run all checkers: [Need to be split due to typing of some functions]
bool checkAll(Man* person){
  return  !checkSelf() && !checkSibling(person, true)
          && !checkParent(person) && !checkChildren(person) && !checkAntUnc(person)
          && checkSingle(person);
}

bool checkAll(Woman* person){
  return  !checkSelf() && !checkSibling(person, true)
          && !checkParent(person) && !checkChildren(person) && !checkAntUnc(person)
          && checkSingle(person);
}

// Checkers for Marriage: ------------------------------------------------------

 // Checks to make sure the two individuals are not the same person
  bool checkSelf(){
    if(nameToFind == marrying){
      cout << "Issue: Same Person" << endl;
      return true;
    }
    return false;
  }

 // Checks if the two individuals are siblings
  bool checkSibling(Person* self, bool print = false){
    if(self->getMother() != nullptr){
      auto children = static_cast<Woman*>(self->getMother())->getChildren();
      auto person = std::find_if(children.begin(), children.end(),
        [this](Person* sibling){
          return sibling->getFirstName() + " " + sibling->getLastName() == marrying;
        });

      if(person == children.end()){
        return false;
      }
      if(print) cout << "Issue: Sibling" << endl;
      return true;
    }
  }

 // Checks if the two individuals have a Child->Parent relationship
  bool checkParent(Person* self){
    bool pCheck = true;

    if(self->getMother() != nullptr)
      pCheck = pCheck && (self->getMother()->getFirstName() + " "
                          + self->getMother()->getLastName() != marrying);

    if(self->getFather() != nullptr)
      pCheck = pCheck && (self->getFather()->getFirstName() + " "
                          + self->getFather()->getLastName() != marrying);

    if(!pCheck) cout << "Issue: Parent" << endl;
    return !pCheck;
  }


// Pretty sure checkChildren is redundant since you can't have children if you
// aren't married, but I wrote it anyway

 // Checks if the two individuals have a parent->child relationship -> Male
  bool checkChildren(Man* self){
    if(self->getSpouse() != nullptr){
      // Delegate work to Spouse
      checkChildren(static_cast<Woman*>(self->getSpouse()));
    }
  }

 // Checks if the two individuals have a parent->child relationship -> Female
  bool checkChildren(Woman* self){
    auto children = self->getChildren();
    auto person = std::find_if(children.begin(), children.end(),
      [this](Person* child){
        return child->getFirstName() + " " + child->getLastName() == marrying;
      });

    if(person == children.end()){
      return false;
    }
    cout << "Issue: Child" << endl;
    return true;
  }

 // Checks if the two individuals are aunt and uncle
  bool checkAntUnc(Person* self){
    bool auCheck = false;

    if(self->getMother() != nullptr)
      auCheck = auCheck || checkSibling(self->getMother());

    if(self->getFather() != nullptr)
      auCheck = auCheck || checkSibling(self->getFather());

    if(auCheck) cout << "Issue: Aunt / Uncle" << endl;
    return auCheck;
  }

 // Checks if the individual is single
  bool checkSingle(Person* self){
    if(self->getSpouse() == nullptr) return true;
    cout << "Issue: Married" << endl;
    return false;
  }

  bool foundPerson = false;
  string nameToFind;
  string marrying;
};


// ================ Traversal Methods for Marriage Checker =====================


// Accept for marriage checker - Man
// Visit this and run accept on spouse [if exists]
void Man::accept(class MarriageChecker* visitor){
  visitor->visit(this);
  if(getSpouse() != nullptr)
    static_cast<Woman*>(getSpouse())->accept(visitor);
}

// Accept for marriage checker - Woman
// Visit spouse, then this and accept any existing children
void Woman::accept(class MarriageChecker* visitor){
  if(getSpouse() != nullptr){
    visitor->visit(static_cast<Man*>(getSpouse()));
  }
  visitor->visit(this);
  for(auto child : children_)
     child->accept(visitor);
}

#endif
