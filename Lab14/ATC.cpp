// mediator-based air traffic control
// Mikhail Nesterenko
// 4/23/2021

#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <cstdio>
#include <ctime>
#include <map>

// for timed sleep
#include <thread>
#include <chrono>


using std::cout; using std::cin; using std::endl;
using std::string;

// Abstract flyweight:
class Logo{
public:
  virtual string name()=0;
};

//======================== Concrete Flyweights: ================================

class AirFrance : public Logo{
  string name() override{return "AIRFRANCE";}
};

class KLM : public Logo{
  string name() override{return "KLM";}
};

class Aeroflot : public Logo{
  string name() override{return "AEROFLOT";}
};

class Lufthansa : public Logo{
  string name() override{return "LUFTHANSA";}
};

class BritishAirways : public Logo{
  string name() override{return "BRITISHAIRWAYS";}
};

class Alitalia : public Logo{
  string name() override{return "ALITALIA";}
};

class Iberia : public Logo{
  string name() override{return "IBERIA";}
};

class Avianca : public Logo{
  string name() override{return "AVIANCA";}
};

class Delta : public Logo{
  string name() override{return "DELTA";}
};

class United : public Logo{
  string name() override{ return "UNITED";}
};

class JetBlue : public Logo{
  string name() override{return "JETBLUE";}
};

class SouthWest : public Logo{
  string name() override{return "SOUTHWEST";}
};

//==============================================================================

// Flyweight Factory
class LogoDist{
public:
  static Logo* makeLogo(string flightName){
    if(logos_.find(flightName) == logos_.end()){

      // Note: since you can't use strings in a switch statement,
      // this was the easiest alternative I could write in a short
      // period of time, yes I know it's lazy

      // Determine flight and add it to map
        if(flightName == "AirFrance")
          logos_[flightName] = new AirFrance;
        else if(flightName == "KLM")
          logos_[flightName] = new KLM;
        else if(flightName == "Aeroflot")
          logos_[flightName] = new Aeroflot;
        else if(flightName == "Lufthansa")
          logos_[flightName] = new Lufthansa;
        else if(flightName == "BritishAirways")
          logos_[flightName] = new BritishAirways;
        else if(flightName == "Alitalia")
          logos_[flightName] = new Alitalia;
        else if(flightName == "Iberia")
          logos_[flightName] = new Iberia;
        else if(flightName == "Avianca")
          logos_[flightName] = new Avianca;
        else if(flightName == "Delta")
          logos_[flightName] = new Delta;
        else if(flightName == "United")
          logos_[flightName] = new United;
        else if(flightName == "JetBlue")
          logos_[flightName] = new JetBlue;
        else if(flightName == "SouthWest")
          logos_[flightName] = new SouthWest;
        else{
          cout << "ERROR: Invalid company name" << endl;
          return nullptr;
        }
    }
    // Return built / found logo
    return logos_[flightName];
  }
private:
  static std::map<string, Logo*> logos_; // Map for string -> logo
};

std::map<string, Logo*> LogoDist::logos_; // Static instantiation



// abstract mediator
class Controller{
public:
   virtual void join(class Flight*)=0;
   virtual void leave(class Flight*)=0;
   virtual void broadcast() = 0;
   virtual void observe() = 0;
};


// concrete mediator
// tower, runway clearance
class Tower: public Controller{
public:
   void join(class Flight*) override;
   void leave(class Flight*) override;
   void broadcast() override;
   void observe() override;
private:
   std::set<class Flight*> waiting_;
};


// abstract colleague
class Flight {
public:
   Flight(Controller *controller):controller_(controller), status_(Status::waiting){
      task_ = rand() % 2 ? Task::taxiing : Task::approaching;
      controller_->join(this);
   }

   string getLogo(){
     return logo_->name() + std::to_string(flightNo_);
   }

   void receive(const string &msg){
      if (msg.find(flightName_) != string::npos || msg.find("all") != string::npos){
	 if(msg.find("clear") != string::npos){
	    cout << flightName_ << " roger that, ";
	    cout << (task_ == Task::taxiing ? "taking off" : "landing") << endl;
    // Added print of logo + action
      cout << "..." << getLogo() << (task_ == Task::taxiing ? " takes off..." : " lands...") << endl;
	    status_ = Status::cleared;
	 } else if(msg.find("status") != string::npos) {
	    cout << flightName_ << " waiting to "
	         << (task_ == Task::taxiing ? "take off" : "land") << endl;
	 } else
	    cout << "Tower, this is " << flightName_ << " please repeat." << endl;
      }
   }
   bool isCleared(){return status_ == Status::cleared;}
   void proceed(){
      std::this_thread::sleep_for(std::chrono::seconds(rand()%3+1)); // wait a while
      cout << flightName_ <<
	   (task_ == Task::taxiing ? " taken off" : " landed")
	   << ". Have a good day!" << endl;
      controller_->leave(this);
   }



protected:
   int flightNo_;
   Logo* logo_;
   string flightName_;
   Controller *controller_;
   enum class Task {taxiing, approaching};  Task task_;
   enum class Status {waiting, cleared};    Status status_;
};


// concrete colleagues
class Airbus: public Flight{
public:
   Airbus(Tower *tower): Flight(tower){
      static const std::vector<string> companies =
	 {"AirFrance", "KLM", "Aeroflot", "Lufthansa",
	  "BritishAirways", "Alitalia", "Iberia", "Avianca"};
    // Generate information for name + logo
    flightNo_ = rand() % 1000;
    string name = companies[rand() % companies.size()];
    logo_ = LogoDist::makeLogo(name);
    flightName_ = name + std::to_string(flightNo_);
      cout << flightName_ << " requesting "
	   << (task_ == Task::taxiing ? "takeoff" : "landing") << endl;
   }
};


class Boeing: public Flight{
public:
   Boeing(Tower *tower): Flight(tower){
      static const std::vector<string> companies =
	         {"Delta", "United", "JetBlue", "SouthWest"};
    // Generate information for name + logo
     flightNo_ = rand() % 1000;
     string name = companies[rand() % companies.size()];
     logo_ = LogoDist::makeLogo(name);
     flightName_ = name + std::to_string(flightNo_);
      cout << flightName_ << " requesting "
	   << (task_ == Task::taxiing ? "takeoff" : "landing") << endl;
   }
};


// member functions for Tower
void Tower::broadcast() {
   cout << "Tower: ";
   string msg;
   getline(cin,msg);
   if(!msg.empty())
      for(auto f: waiting_) f->receive(msg);
}


void Tower::observe() {
   Flight *toProceed = nullptr;
   for(auto f: waiting_)
      if(f->isCleared()){
	 if(toProceed == nullptr)
	    toProceed = f;
	 else // more than a single plane cleared
	    cout << "MAYDAY! MAYDAY! MAYDAY! " << endl;
      }
   if(toProceed != nullptr) toProceed ->proceed();
}

void Tower::leave(Flight *f) {
   waiting_.erase(f);
   delete f;
}

void Tower::join(Flight *f) {
   waiting_.insert(f);
}


int main(){
   srand(time(nullptr));
   Tower jfk;

   new Boeing(&jfk);
   new Airbus(&jfk);
   new Boeing(&jfk);
   new Airbus(&jfk);

   while(true){
      jfk.broadcast();
      jfk.observe();
      if(rand() % 2){
	 if (rand() % 2)
	    new Boeing(&jfk);
	 else
	    new Airbus(&jfk);
      }
   }
}
