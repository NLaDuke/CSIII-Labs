#ifndef BARISTA_HPP
#define BARISTA_HPP

#include "drink.hpp"

#include <string>
#include <iostream>
#include <unordered_set>
#include <deque>
#include <ctime>
#include <algorithm>

using std::cin; using std::cout;
using std::string; using std::endl;

//----------------------------
// File: barista.hpp
// By: Nolan LaDuke
// Date: 3/30/2021
//------------------------------------------------------------------------------
// Function: Observer Design Paradigm used for Coffee serving
//------------------------------------------------------------------------------

// Forward class declaration of Customer
class Customer;

// Barista Class
class Barista{
public:
 // Constructors:
  Barista(){srand(time(NULL));}

  virtual ~Barista();

// Function that takes in a new Customer-Drink pair and registers them
  void registerOrder(Customer*, Drink*);

// Function that takes the first non-ready order and makes it ready
  virtual bool makeOrder(){
   // Iterate through until an unprepared order is found
    auto completableOrder = std::find_if(orderQueue_.begin(), orderQueue_.end(),
      [](Drink* order){
        return !order->getStatus();
      });

   // if one is found mark it as ready
    if(completableOrder != orderQueue_.end()){
      (*completableOrder)->setReady();
      cout << "Barista finished making order number " << (*completableOrder)->getID() << endl;
      return true;
    }
    return false;
  }

// Function that takes the first ready order and announces to all registered Customers its ID
  bool serveOrder();

// Function that Customer initiates, deregisters + deletes Customer and matching Drink
  void pickupOrder(Customer*);

 // 50/50 Prepare or Serve order function
  void work(){
    if(rand()%2){
      if(makeOrder()) cout << endl;
    }
    else{
      if(serveOrder()) cout << endl;
    }
  }

// 50/50 Prepare + Serve or Take order function
  void workAlternate();

// Returns whether or not there are any more orders
  bool doneWorking(){
    return (customers_.empty() && orderQueue_.empty());
  }

// Takes in a new order and returns it as a Drink*
  Drink* takeOrder(){
    // Set up variables:
     char userInput;
     string custName;

    // Prompt user
     cout << "Welcome to Not_Starbucks, can I get a name for this order? ";
     cin >> custName;

     cout << "Hello " << custName << "!" << endl
          << "would you like a [l]arge, [m]edium, or [s]mall coffee? ";
     cin >> userInput;

    // Case for invalid inputs:
     while(userInput != 'l' && userInput != 'm' && userInput != 's'){
       cout << "Sorry, the value you input is not valid, please try again:" << endl
            << "Can I get you a [l]arge, [m]edium, or [s]mall coffee? ";
       cin >> userInput;
     }

    // Set Drink type + cost based on input
     DrinkType size = userInput == 'l' ? DrinkType::large : (userInput == 'm' ? DrinkType::medium : DrinkType::small);
     int cost = userInput == 'l' ? 3 : (userInput == 'm' ? 2 : 1);
    // Set up base drink
     Drink* custDrink = new Drink(size, custName, cost);

    // Repeatedly prompt user for toppings until they give the 'n' input:
     cout << "Would you like to add [s]prinkles, [c]aramel, milk [f]oam, [i]ce or [n]othing? ";
     cin >> userInput;

     while(userInput != 'n'){
       switch(userInput){
         case('s'):
            custDrink = new sprinkles(custDrink);
         break;

         case('c'):
           custDrink = new caramel(custDrink);
         break;

         case('f'):
           custDrink = new milkFoam(custDrink);
         break;

         case('i'):
           custDrink = new ice(custDrink);
         break;

         default: // Default for invalid input:
           cout << "Sorry, the input you gave was not valid - please try again:" << endl;
       }
       cout << "Would you like to add [s]prinkles, [c]aramel, milk [f]oam, [i]ce or [n]othing? ";
       cin >> userInput;
     }
     cout << "Thank you, your order will be ready momentarily!" << endl << endl;
     return custDrink;
  }

protected:
  std::deque<Customer*> customers_;
  std::deque<Drink*> orderQueue_;
};

// Customer Class:

class Customer{
public:
 // Constructors:
  Customer(Drink* order = nullptr, Barista* barista = nullptr, string name = "")
      : order_(order), barista_(barista), name_(name), orderNum_(-1){};
  ~Customer(){} // order_ erased manually elsewhere

 // Setters:
  void setNumber(int orderNum){orderNum_ = orderNum;};

 // Getters:
  string getName(){return name_;};
  int getOrderNum(){return orderNum_;};

 // Functions:
  void notifyOrder(Drink*);

private:
  string name_;
  Drink* order_;
  Barista* barista_;
  int orderNum_;
};

//=================== Implementation for Barista Class =========================

// Destructor implementation
Barista::~Barista(){
  std::for_each(customers_.begin(), customers_.end(), [](Customer* cust){delete cust;});
}

// Register Order Implementation
void Barista::registerOrder(Customer* cust, Drink* drnk){
  static int orderNumber = 0;
  cust->setNumber(orderNumber);
  drnk->setID(orderNumber++);
  customers_.push_back(cust);
  orderQueue_.push_back(drnk);
}

// Serve Order implementaiton
bool Barista::serveOrder(){
 // Iterate through until a prepared order is found
  auto completedOrder = std::find_if(orderQueue_.begin(), orderQueue_.end(),
    [](Drink* order){
      return order->getStatus();
    });

 // If an order was not found, return; otherwise, alert everyone
  if(completedOrder == orderQueue_.end()) return false;

 // Announce that the order is ready
  cout << "Order number " << (*completedOrder)->getID()
            << " is now ready for pickup!" << endl;

  for(auto cust : customers_){
    cust->notifyOrder(*completedOrder);
  }

 // Erase and delete picked up order:
  Drink* delPtr = *completedOrder;
  orderQueue_.erase(completedOrder);
  delete delPtr;
  return true;
}

// Pickup Order implementation
void Barista::pickupOrder(Customer* cust){
 // Find Customer's Order
  auto custOrder = std::find_if(orderQueue_.begin(), orderQueue_.end(),
    [&cust](Drink* currentDrink){
      return (cust->getOrderNum() == currentDrink->getID()) && currentDrink->getStatus();
    });

 // Error Case
  if(custOrder == orderQueue_.end()){
    std::cerr << "ERROR: Customer attempted to pick up an invalid order [ID did not exist / order not ready]\n";
    return;
  }

 // Print statement:
  cout << "Here you go " << cust->getName() << "!" << endl
            << "Your " << (*custOrder)->getSizeName()
            << " with " << (*custOrder)->getToppings() << " is ready." << endl
            << "Your total is $" << (*custOrder)->getPrice() << endl;

 // Erase and delete customer

  auto custRemoval = std::find_if(customers_.begin(), customers_.end(),
  [&cust](Customer* currentCust){
    return (currentCust->getOrderNum() == cust->getOrderNum());
  });

 // Error Case
  if(custRemoval == customers_.end()){
    std::cerr << "ERROR: Customer not found yet had drink in queue\n";
    return;
  }

  Customer* forDeletion = *custRemoval;
  customers_.erase(custRemoval);
  delete forDeletion;
}

// Alternative Work implementation
void Barista::workAlternate(){
  if(rand()%2){
    if(makeOrder() && serveOrder()) cout << endl;
  }
  else{
    Drink* order = takeOrder();
    Customer* cust = new Customer(order, this, order->getName());
    registerOrder(cust, order);
  }
}

//==================== Implementation for Customer Class =======================

void Customer::notifyOrder(Drink* finishedOrder){
  if(finishedOrder->getID() == orderNum_){
    barista_->pickupOrder(this);
  }
}


#endif
