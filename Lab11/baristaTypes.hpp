#ifndef BARISTA_TYPES_HPP
#define BARISTA_TYPES_HPP

#include "barista.hpp"
#include "drink.hpp"

#include <string>

using std::cout; using std::endl;
using std::string;

//----------------------------
// File: baristaTypes.hpp
// By: Nolan LaDuke
// Date: 4/01/2021
//------------------------------------------------------------------------------
// Function: Chain of command design extension of Barista class
//------------------------------------------------------------------------------

// Abstract class for different levels of Baristas
class baristaWorker : public Barista{
public:
 // Constructors:
  baristaWorker(baristaWorker* nextLevel = nullptr) : Barista(), nextLevel_(nextLevel){};
  virtual ~baristaWorker(){delete nextLevel_;};

// makeOrder functions:

 // Generic makeOrder [overrides Barista::makeOrder()]
  bool makeOrder() override{
    // Iterate through until an unprepared order is found
     auto completableOrder = std::find_if(orderQueue_.begin(), orderQueue_.end(),
       [](Drink* order){
         return !order->getStatus();
       });

    // if one is found, call makeOrder(Drink*)
     if(completableOrder != orderQueue_.end()){
       return makeOrder(*completableOrder);
     }
     return false;
  };

 // Class specific makeOrder [called in baristaWorker::makeOrder]
  virtual bool makeOrder(Drink* order){
    if(nextLevel_ != nullptr){
      return nextLevel_->makeOrder(order);
    }
    else{
      cout << "Sorry, none of the employees here can make that!" << endl;
      return false;
    }
  }

private:
  baristaWorker* nextLevel_;
};

//========================= Concrete implementaitons ===========================

// Junior Barista class can make any order with no toppings
class juniorBarista : public baristaWorker{
public:
 // Constructors:
  juniorBarista(baristaWorker* nextlevel = nullptr) : baristaWorker(nextlevel){};
  ~juniorBarista(){};

// Functions:
  bool makeOrder(Drink* order) override{
   // If there are no toppings, make it and return
    if(!order->hasTopping()){
      order->setReady();
      cout << "Junior Barista finished making order number " << order->getID() << endl;
      return true;
    }
   // Otherwise, call parent class' makeOrder(Drink*)
    return baristaWorker::makeOrder(order);
  }

};

// Senior Barista class can make any order not containing milk foam
class seniorBarista : public baristaWorker{
public:
 // Constructors:
  seniorBarista(baristaWorker* nextlevel = nullptr) : baristaWorker(nextlevel){};
  ~seniorBarista(){};

// Functions:
  bool makeOrder(Drink* order) override{
   // If no milkfoam, make it and return
    if(order->getToppings().find("Foam") == std::string::npos){
      order->setReady();
      cout << "Senior Barista finished making order number " << order->getID() << endl;
      return true;
    }
   // Otherwise, call parent class' makeOrder(Drink*)
    return baristaWorker::makeOrder(order);
  }
};

// Manager class can make any order
class manager : public baristaWorker{
public:
 // Constructors:
  manager(baristaWorker* nextlevel = nullptr) : baristaWorker(nextlevel){};
  ~manager(){};

// Functions:
  bool makeOrder(Drink* order) override{
    order->setReady();
    cout << "Manager finished making order number " << order->getID() << endl;
    return true;
  }
};



#endif
