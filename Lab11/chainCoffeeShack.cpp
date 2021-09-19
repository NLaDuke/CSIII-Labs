#include "drink.hpp"
#include "barista.hpp"
#include "baristaTypes.hpp"
#include <iostream>
#include <string>

using std::cin; using std::cout;


//----------------------------
// File: chainCoffeeShack.cpp
// By: Nolan LaDuke
// Date: 4/01/2020
//------------------------------------------------------------------------------
// Function: Test file for functionality of drink class and extended, Chain
//            of Responsibility Barista class
//  Asks the user repeatedly to add toppings, then displays total price + info
//------------------------------------------------------------------------------

int main(){
 // Set up variables:
  int numOrders;
  Barista* employees = new juniorBarista(new seniorBarista(new manager()));

  cout << "Testing the functionality of Chain Barista implementation" << endl
       << "How many orders would you like to take? ";
  cin >> numOrders;

  for(int i = 0; i < numOrders; i++){
    Drink* order = employees->takeOrder();
    Customer* cust = new Customer(order, employees, order->getName());
    employees->registerOrder(cust, order);
  }

  while(!employees->doneWorking()){
    employees->work();
  }

  delete employees;

}
