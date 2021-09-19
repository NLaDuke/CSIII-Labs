#include <set>
#include <map>
#include <iostream>
#include <string>
#include <deque>
#include <algorithm>

using std::cout; using std::endl;
using std::map; using std::set;
using std::string; using std::deque;

//----------------------------
// File: observer.cpp
// By: Nolan LaDuke
// Date: 4/21/2021
//------------------------------------------------------------------------------
// Function: Observer with Name Based Lookup
//------------------------------------------------------------------------------
// Based on a file provided by Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------

class Observer;
class Subject;

class EventRegistry{
public:
   static void registerSubject(Subject*);
   static void registerObserver(Subject*, Observer*);
   static void deregisterObserver(Subject*, Observer*);
   static void registerObserver(string, Observer*);
   static void deregisterObserver(string, Observer*);
   static void handleMessage(Subject*);
private:
   static map<Subject*, set<Observer*>> observerMap_;
   static deque<Subject*> subjectList_;
};


// initialize the static map
map<Subject*, set<Observer*>> EventRegistry::observerMap_;
deque<Subject*> EventRegistry::subjectList_;


class Observer{
public:
   Observer(const string &name): name_(name) {}
   string getName() const {return name_;}
   void subscribe(Subject *s) {EventRegistry::registerObserver(s, this);}
   void unsubscribe(Subject *s){EventRegistry::deregisterObserver(s, this);}
   void subscribe(string s) {EventRegistry::registerObserver(s, this);}
   void unsubscribe(string s){EventRegistry::deregisterObserver(s, this);}
   void handleMessage(Subject*);
private:
   string name_;
};

class Subject{
public:
   Subject(const string &name): name_(name) {}
   string getName() const {return name_;}
   void generateMessage(){
      cout << name_ << " sent a message" << endl;
      EventRegistry::handleMessage(this);}
private:
   string name_;
};

  // Manually add a subject to the list
void EventRegistry::registerSubject(Subject* s){
  subjectList_.push_back(s);
}

void EventRegistry::registerObserver(Subject *s, Observer* o){
   observerMap_[s].insert(o);
   cout << o->getName() << " subscribed to " << s->getName() << endl;
}

void EventRegistry::deregisterObserver(Subject *s, Observer* o){
   observerMap_[s].erase(o);
   cout << o->getName() << " unsubscribed from " << s->getName() << endl;
}


  // Register based on name
void EventRegistry::registerObserver(string subName, Observer* o){
  bool found = false;

 // Iterate through the map, if we find a name matching the given, register 'o' to it
  std::for_each(observerMap_.begin(), observerMap_.end(),
  [&subName, &o, &found](std::pair<Subject*, set<Observer*>> elem){
    if(elem.first->getName() == subName){
      observerMap_[elem.first].insert(o);
      cout << o->getName() << " subscribed to " << elem.first->getName() <<" via name" << endl;
      found = true;
    }
  });

  if(found) return;
  // If subject was not found, check list of registered senders

  std::for_each(subjectList_.begin(), subjectList_.end(),
  [&subName, &o](Subject* sub){
   // If we find a registered subject with the same name, add them to the map with observer 'o'
    if(sub->getName() == subName){
      observerMap_[sub].insert(o);
      cout << o->getName() << " subscribed to " << sub->getName() <<" via name" << endl;
    }
  });
}

  // Deregister based on name
void EventRegistry::deregisterObserver(string subName, Observer* o){
 // Iterate through the map until we find the subject, then erase 'o' from their list
  std::for_each(observerMap_.begin(), observerMap_.end(),
  [&subName, &o](std::pair<Subject*, set<Observer*>> elem){
    if(elem.first->getName() == subName){
      observerMap_[elem.first].erase(o);
      cout << o->getName() << " unsubscribed from " << elem.first->getName() << " via name" << endl;
    }
  });
}


void EventRegistry::handleMessage(Subject *s){
   for (auto e: observerMap_[s])
      e->handleMessage(s);
}

void Observer::handleMessage(Subject *s) {
   cout << name_ << " received message from " << s->getName() << endl;
}


int main() {
   Observer olaf("Observer Olaf"), olga("Observer Olga");
   Subject  sue("Subject Sue"),  steve("Subject Steve");

   // Register subjects to make code function properly
   EventRegistry::registerSubject(&sue);
   EventRegistry::registerSubject(&steve);

   // Subscribe normally
   olga.subscribe(&sue);
   // Subscribe based on name
   olaf.subscribe(string("Subject Sue")); olaf.subscribe(string("Subject Steve"));

   cout << endl;

   sue.generateMessage();
   steve.generateMessage();

   cout << endl;
   // Unsubscribe based on name
   olga.unsubscribe(string("Subject Sue"));
   olaf.unsubscribe(string("Subject Sue"));
   // Unsubscribe Normally
   olaf.unsubscribe(&steve);
   sue.generateMessage();
}
