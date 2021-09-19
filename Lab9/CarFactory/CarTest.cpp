
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "CarFactory.hpp"

//----------------------------
// File: CarTest.cpp
// By: Nolan LaDuke
// Date: 3/16/2021
//------------------------------------------------------------------------------
// Function: Test file for Car and Factory classes
//
//------------------------------------------------------------------------------
// Based on a file provided by Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------

using std::vector;
using std::cout; using std::endl;

/*
CarFactory *getLeastBusyFactory(const vector<CarFactory*> &inFactories) {

   if (inFactories.size() == 0) return nullptr;

   auto bestSoFar = inFactories[0];

   for (size_t i = 1; i != inFactories.size(); ++i) {
      if (inFactories[i]->getNumCarsInProduction() < bestSoFar->getNumCarsInProduction())
	 bestSoFar = inFactories[i];
   }
   return bestSoFar;
}
*/

// same function with an STL algorithm
CarFactory *getLeastBusyFactory(const vector<CarFactory*> &inFactories) {

   if (inFactories.size() == 0) return nullptr;

   return accumulate(inFactories.begin()+1, inFactories.end(), *inFactories.begin(),
	   [](CarFactory *a, CarFactory *b)
	     { return a->getNumCarsInProduction() < b->getNumCarsInProduction() ? a: b; }
	   );
}


int main() {
   vector<CarFactory*> factories;

   srand(time(nullptr));

   // Create 3 Ford factories and
   // 1 Toyota factory with random load.
   factories.push_back(new FordFactory("Chicago, IL", rand()%4));
   factories.push_back(new FordFactory("Dearborn, MI", rand()%4));
   factories.push_back(new FordFactory("Louisville, KY", rand()%4));
   factories.push_back(new ToyotaFactory("Lafayette,IN", rand()%4));


   for (size_t i=0; i < 10; ++i) {
      CarFactory *currentFactory = getLeastBusyFactory(factories);
      Car *newCar = currentFactory->requestCar();
      cout << "Ordering " << newCar->getMake()  << " " << newCar->getModel()
	   << " from " << currentFactory->getLocation()
	   << endl;
   }
}
