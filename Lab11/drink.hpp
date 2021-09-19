#ifndef DRINK_HPP
#define DRINK_HPP

#include <string>


//----------------------------
// File: drink.hpp
// By: Nolan LaDuke
// Date: 3/30/2021
//------------------------------------------------------------------------------
// Function: Implementation of class Drink using Decorator design pattern
//
//------------------------------------------------------------------------------
// Based on a file provided by Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------

enum class DrinkType {small, medium, large};
static const std::string DrinkNames[] = {"small", "medium", "large"};

// Drink template class that provides basis for implementation + stores
class Drink{
public:
 // Constructor delegates work to element constructors
  Drink(DrinkType type=DrinkType::small, std::string name = "", double price=0.0, int id=0):
                 type_(type), custName_(name), price_(price), prepared_(false), ID_(id){}

 // Destructor:
  virtual ~Drink(){};

 // Getters:
  virtual double getPrice() const {return price_;};
  virtual std::string getName() const {return custName_;};
  virtual std::string getToppings() const {return "Love";};
  virtual DrinkType getSize() const {return type_;};
  virtual std::string getSizeName() const {return DrinkNames[int(type_)];};
  virtual int getID() const {return ID_;};
  virtual bool hasTopping() {return false;};

 // Setters:
  virtual void setID(int newID) {ID_ = newID;};

 // Status Functions:
  bool getStatus() const {return prepared_;};
  void setReady(){prepared_ = true;};

protected:
  double price_;

private:
  int ID_;
  bool prepared_ = false;
  std::string custName_;
  DrinkType type_;
};

// Decorator Template Class:
class topping : public Drink{
public:
 // Constructors:
  topping() : drink_(nullptr){};
  virtual ~topping(){delete drink_;};

 // Pure Virtual function for retreiving toppings:
  virtual std::string getToppings() const override = 0;

 // Price Getter: return 'price_' + price of drink_
  double getPrice() const override {return price_ + drink_->getPrice();};

 // Getters call function on base drink_
  std::string getName() const override {return drink_->getName();};
  DrinkType getSize() const override {return drink_->getSize();};
  std::string getSizeName() const override {return drink_->getSizeName();};
  int getID() const override {return drink_->getID();};
  void setID(int newID) override {drink_->setID(newID);};
  bool hasTopping() override {return true;};

protected:
  Drink* drink_;
};

// Class for adding sprinkles topping (0.50)
class sprinkles : public topping{
public:
 // Constructor initializes price and pointer to Drink
  sprinkles(Drink* baseDrink = nullptr){price_ = 0.5; drink_ = baseDrink;};

 // Destructor:
  ~sprinkles(){}

 // Topping Getter: return topping name + drink_->getToppings()
  std::string getToppings() const override {return "Sprinkles, " + drink_->getToppings();};
};

// Class for adding caramel topping (0.20)
class caramel : public topping{
public:
 // Constructor initializes price and pointer to Drink
  caramel(Drink* baseDrink = nullptr){price_ = 0.2; drink_ = baseDrink;};

 // Destructor:
  ~caramel(){}

 // Topping Getter: return topping name + drink_->getToppings()
  std::string getToppings() const override {return "Caramel, " + drink_->getToppings();};
};

// Class for adding milkfoam topping (0.40)
class milkFoam : public topping{
public:
 // Constructor initializes price and pointer to Drink
  milkFoam(Drink* baseDrink = nullptr){price_ = 0.4; drink_ = baseDrink;};

 // Destructor:
  ~milkFoam(){}

 // Topping Getter: return topping name + drink_->getToppings()
  std::string getToppings() const override {return "Foam, " + drink_->getToppings();};
};

// Class for adding ice topping (0.10)
class ice : public topping{
public:
 // Constructor initializes price and pointer to Drink
  ice(Drink* baseDrink = nullptr){price_ = 0.1; drink_ = baseDrink;};

 // Destructor:
  ~ice(){}

 // Topping Getter: return topping name + drink_->getToppings()
  std::string getToppings() const override {return "Ice, " + drink_->getToppings();};
};

#endif
