#include "drink.hpp"
#include <iostream>
#include <string>

using std::cin; using std::cout;
using std::string; using std::endl;

//----------------------------
// File: baseCoffeeShack.cpp
// By: Nolan LaDuke
// Date: 3/30/2021
//------------------------------------------------------------------------------
// Function: Test file for functionality of drink class
//  Asks the user repeatedly to add toppings, then displays total price + info
//------------------------------------------------------------------------------

int main(){
 // Set up variables:
  char userInput;
  string custName;

 // Prompt user
  cout << "Welcome to Not_Starbucks, can I get a name for this order? ";
  cin >> custName;

  cout << "Hello " << custName << "!" << endl
       << "Would you like a [l]arge, [m]edium, or [s]mall coffee? ";
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

 // Print ending sequence
  cout << endl
       << custDrink->getName() << ", your " << custDrink->getSizeName()
       << " with " << custDrink->getToppings() << " is ready." << endl
       << "Your total is $" << custDrink->getPrice() << endl;

  delete custDrink; // Delete pointers
  return 0;
}
