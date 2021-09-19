#include <iostream>

using std::cout; using std::endl; using std::cin;

//----------------------------
// File: BridgeFigures.cpp
// By: Nolan LaDuke
// Date: 3/17/2021
//------------------------------------------------------------------------------
// Function: Header and Implementation file for 'Fill' and derived classes
//
//------------------------------------------------------------------------------
// Based on a file provided by Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------


// abstract body
class Fill{
public:
   Fill(char fillChar, char borderChar) : fillChar_(fillChar), borderChar_(borderChar){};
   virtual char getBorder()=0;
   virtual char getInternal()=0;
   virtual ~Fill() {}
protected:
   char fillChar_;
   char borderChar_;
};

// concrete body
class Hollow: public Fill{
public:
   Hollow(char borderChar):Fill(' ', borderChar){}
   char getBorder() override {return borderChar_;}
   char getInternal() override {return ' ';}
   ~Hollow(){}
};

// another concrete body
class Filled: public Fill {
public:
   Filled(char fillChar):Fill(fillChar, fillChar){}
   char getBorder() override {return borderChar_;}
   char getInternal() override {return fillChar_;}
   ~Filled(){}
};

//======================EnhancedFilled Class + Implementation===================

class EnhancedFilled: public Filled {
public:
   EnhancedFilled(char fillChar, char borderChar): Filled(borderChar){fillChar_ = fillChar;};
   ~EnhancedFilled(){};
};

//==============================================================================


// abstract handle
class Figure {
public:
   Figure(int size, Fill* fill): size_(size), fill_(fill){}
   void changeFill(Fill* fill) {fill_ = fill;};
   virtual void draw() =0;
   virtual ~Figure(){}
protected:
   int size_;
   Fill *fill_;
};

// concrete handle
class Square: public Figure{
public:
   Square(int size, Fill* fill): Figure(size, fill){}
   void draw() override;

};


void Square::draw(){
   for(int i=0; i < size_; ++i){
      for(int j=0; j < size_; ++j)
	 if(i==0 || j==0 || i==size_-1 || j==size_-1 )
	    cout << fill_ -> getBorder();
	 else
	    cout << fill_ -> getInternal();
      cout << endl;
   }
}


int main(){
  cout << "Testing a shape drawing class!" << endl;

   // ask user for figure parameters
   cout << "Enter border character: ";
            char bchar; cin >> bchar;
   cout << "Enter fill character: ";
            char fchar; cin >> fchar;
   cout << "Enter size: "; int size; cin >> size;

 // Draw as filled:
   cout << "Drawing Shape of type Filled: " << endl;

   Figure *userBox = new Square(size, static_cast<Fill *>(new Filled(fchar)));
   userBox -> draw();
   cout << endl;
   delete userBox;

// Draw as hollow:
  cout << "Drawing Shape of type Hollow: " << endl;

  userBox = new Square(size, static_cast<Fill *>(new Hollow(bchar)));
  userBox -> draw();
  cout << endl;
  delete userBox;

// Draw as EnhancedFilled:
  cout << "Drawing Shape of type EnhancedFilled: " << endl;

  userBox = new Square(size, static_cast<Fill *>(new EnhancedFilled(fchar, bchar)));
  userBox -> draw();
  cout << endl;
  delete userBox;

  cout << endl;

}
