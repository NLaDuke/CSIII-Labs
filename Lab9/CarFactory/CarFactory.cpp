#include <array>
#include <cstdlib>
#include "CarFactory.hpp"

//----------------------------
// File: CarFactory.cpp
// By: Nolan LaDuke
// Date: 3/16/2021
//------------------------------------------------------------------------------
// Function: Implementation file for Factory and Car classes
//
//------------------------------------------------------------------------------
// Based on a file provided by Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------

// product methods

Ford::Ford(){
   static const std::array<std::string, 4>
      models = {"Focus", "Mustang", "Explorer", "F-150"};
   make_ = "Ford";
   model_ = models[rand() % models.size()];
}

Toyota::Toyota(){
   static const std::array<std::string, 5>
      models = {"Corolla", "Camry", "Prius", "4Runner", "Yaris"};
   make_ = "Toyota";
   model_ = models[rand() % models.size()];
}


// ractory methods
Car* CarFactory::requestCar() {
	++numCarsInProduction_;
	return makeCar();
}

Car* FordFactory::makeCar() {
	return new Ford();
}

Car* ToyotaFactory::makeCar() {
	return new Toyota();
}
