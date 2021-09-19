#include "drink.hpp"
#include "barista.hpp"
#include <iostream>
#include <string>

using std::cin; using std::cout;
using std::string; using std::endl;

//----------------------------
// File: observerCoffeeShack.cpp
// By: Nolan LaDuke
// Date: 3/30/2021
//------------------------------------------------------------------------------
// Function: Test file for functionality of drink class and extended
//             Barista + Customer classes
//  Asks the user repeatedly to add toppings, then displays total price + info
//------------------------------------------------------------------------------

int main(){
 // Set up variables:
  int numOrders;
  Barista* employee = new Barista();

  cout << "Testing the functionality of Barista + Customer classes" << endl
       << "How many orders would you like to take? ";
  cin >> numOrders;

  for(int i = 0; i < numOrders; i++){
    Drink* order = employee->takeOrder();
    Customer* cust = new Customer(order, employee, order->getName());
    employee->registerOrder(cust, order);
  }

  while(!employee->doneWorking()){
    employee->work();
  }

  delete employee;

}
