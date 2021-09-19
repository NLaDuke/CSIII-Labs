#include <iostream>
#include <vector>
#include <cstdlib>
#include <array>
#include <string>
#include <algorithm>
#include "CarFactory.cpp"

using std::vector; using std::string;
using std::cout; using std::endl;

//----------------------------
// File: CarBuyer.cpp
// By: Nolan LaDuke
// Date: 3/16/2021
//------------------------------------------------------------------------------
// Function: header and implementation file for CarLot class
//    Also contains main function for car buyers
//
//------------------------------------------------------------------------------
// Based on a file provided by Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------

class CarLot{
public:
  // Constructors:
   CarLot();

  // Functions:

  // Returns first car in the lot
   Car *testDriveCar(){return *car4sale_.begin();}

  // Function for buying a car and requesting a new one
   Car *buyCar(Car* want2Buy){
      Car* bought = want2Buy;

    // Find element being bought in car4sale_ and replace it with new car
      *std::find(car4sale_.begin(), car4sale_.end(), want2Buy)
          = factories_[rand()%factories_.size()]->requestCar();

      return bought;
   }

// Getters:

  // Get the car one position after the car provided
   Car* nextCar(Car* currentCar){
     auto itr = ++std::find(car4sale_.begin(), car4sale_.end(), currentCar);
     if (itr != car4sale_.end()){
        return *itr;
     }
     std::cerr << "ERROR: Attempted to get 'nextCar', but there are no more cars\n";
     return currentCar;
   }

  // Returns the size of the lot [number of cars it can hold]
   int lotSize(){
     return lotSize_;
   }

private:
   const int lotSize_; // Number of cars the lot can hold
   vector<Car *> car4sale_; // Container for up to lotSize_ Cars in lot
   vector<CarFactory *> factories_;
};


CarLot::CarLot() : lotSize_(10), car4sale_(lotSize_){
   // creates 2 Ford factories and 2 Toyota factories
   factories_.push_back(new FordFactory());
   factories_.push_back(new ToyotaFactory());
   factories_.push_back(new FordFactory());
   factories_.push_back(new ToyotaFactory());

   // Fills lot with cars:
   std::generate(car4sale_.begin(), car4sale_.end(),
   [&]()-> Car*{
     return factories_[rand() % factories_.size()] -> requestCar();
   });

}



CarLot *carLotPtr = nullptr; // global pointer instantiation

// test-drives lot
// buys car if Toyota + Model they like
void toyotaLover(int id){
  if (carLotPtr == nullptr)
     carLotPtr = new CarLot();


 // Determine preferred model
  static const std::array<std::string, 5>
    models = {"Corolla", "Camry", "Prius", "4Runner", "Yaris"};

  string preferredModel = models[rand() % models.size()];

 // Set up variables for loop
  Car *toBuy = carLotPtr -> testDriveCar();
  int carsTested = 0;
  bool foundCar = false;

 // Loop until a good car has been found, or all cars have been tried
  while(carsTested < carLotPtr -> lotSize() && !foundCar){
      cout << "Buyer " << id << endl;
      cout << "test driving "
   	       << toBuy->getMake() << " "
           << toBuy->getModel();

    if (toBuy->getMake() == "Toyota" && toBuy->getModel() == preferredModel){
       cout << " love it! buying it!" << endl;
       carLotPtr -> buyCar(toBuy); // Buy car
       foundCar = true;       // Set exit condition
    } else {
       cout << " did not like it!" << endl;
       ++carsTested; // Increment cars tested
       if(carsTested < carLotPtr -> lotSize())
         toBuy = carLotPtr->nextCar(toBuy); // Get next car from the lot (if not all cars tested)
   }
 }
 if(!foundCar){
   cout << "Buyer " << id << " didn't find any cars they liked." << endl;
 }
}

// test-drives lot
// buys it if Ford + model they like
void fordLover(int id){
   if (carLotPtr == nullptr)
      carLotPtr = new CarLot();


  // Determine preferred model
   static const std::array<std::string, 4>
     models = {"Focus", "Mustang", "Explorer", "F-150"};

   string preferredModel = models[rand() % models.size()];

  // Set up variables for loop
   Car *toBuy = carLotPtr -> testDriveCar();
   int carsTested = 0;
   bool foundCar = false;

  // Loop until a good car has been found, or all cars have been tried
   while(carsTested < carLotPtr -> lotSize() && !foundCar){
       cout << "Buyer " << id << endl;
       cout << "test driving "
    	      << toBuy->getMake() << " "
            << toBuy->getModel();

     if (toBuy->getMake() == "Ford" && toBuy->getModel() == preferredModel){
        cout << " love it! buying it!" << endl;
        carLotPtr -> buyCar(toBuy); // Buy car
        foundCar = true;       // Set exit condition
     } else {
        cout << " did not like it!" << endl;
        ++carsTested; // Increment cars tested
        if(carsTested < carLotPtr -> lotSize())
          toBuy = carLotPtr->nextCar(toBuy); // Get next car from the lot (if not all cars tested)
    }
  }
  if(!foundCar){
    cout << "Buyer " << id << " didn't find any cars they liked." << endl;
  }
}



int main() {
   srand(time(nullptr));

   const int numBuyers=10;
   for(int i=0; i < numBuyers; ++i){
      if(rand()% 2 == 0)
	       toyotaLover(i);
      else
	       fordLover(i);
      cout << endl;
    }

}
