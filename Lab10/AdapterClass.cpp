#include <iostream>

using std::cout; using std::endl;

//----------------------------
// File: AdapterClass.cpp
// By: Nolan LaDuke
// Date: 3/23/2021
//------------------------------------------------------------------------------
// Function: Implementation of a Class Adapter for 'Figures' and 'Rectangles'
//
//------------------------------------------------------------------------------
// Based on a file provided by Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------

// base interface
class Figure {
public:
   virtual void draw()=0;
   virtual void resize(int)=0;
   virtual ~Figure(){}
};

// adaptee/implementer
class LegacyRectangle{
public:
  // Constructor
   LegacyRectangle(int topLeftX,
		   int topLeftY,
		   int bottomRightX,
		   int bottomRightY):
      topLeftX_(topLeftX),
      topLeftY_(topLeftY),
      bottomRightX_(bottomRightX),
      bottomRightY_(bottomRightY){}

  // Drawing Function
   void oldDraw(){
      for(int i=0; i < bottomRightY_; ++i){
	 for(int j=0; j < bottomRightX_; ++j)
	    if(i >= topLeftY_  && j >= topLeftX_ )
	       cout << '*';
	    else
	       cout << ' ';
	 cout << endl;
      }
   }

   // Move function
    void move(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY){
      topLeftX_ = topLeftX;
      topLeftY_ = topLeftY;
      bottomRightX_ = bottomRightX;
      bottomRightY_ = bottomRightY;
    }

   // Getters:
    int getTopLeftX(){return topLeftX_;};
    int getTopLeftY(){return topLeftY_;};
    int getBottomRightX(){return bottomRightX_;};
    int getBottomRightY(){return bottomRightY_;};

// defining top/left and bottom/right coordinates
private:
   int topLeftX_;
   int topLeftY_;
   int bottomRightX_;
   int bottomRightY_;
};

// adapter uses multiple inheritance to inherit
// interface and implementation
class SquareAdapter: public Figure,
		     private LegacyRectangle {
public:
  // Constructor:
   SquareAdapter(int size):
      LegacyRectangle(0,0,size,size){};

  // Draw Function:
   void draw() override {
      oldDraw();
   };

  // Size Getter:
   int size(){
     return (getBottomRightX() - getTopLeftX());
   }

  // Resize Function:
    void resize(int newSize){
      move(0, 0, newSize, newSize);
    }
};


int main(){
 // Set up variable
  Figure *square;

// Print setup
  cout << "We're making Squares!" << endl;
  cout << "What size would you like the square to be?: ";

// Prompt user until they input a valid size
  int size;
  std::cin >> size;
  while(size < 0){
    cout << "Sorry, an invalid size was input, please try again!: ";
    std::cin >> size;
  }

// Create and draw sequare
  square = new SquareAdapter(size);
  square->draw();

// Print setup
  cout << "We're changing the size.." << endl;
  cout << "What new size would you like the square to be?: ";

// Prompt user until they input a valid size
  std::cin >> size;
  while(size < 0){
    cout << "Sorry, an invalid size was input, please try again!: ";
    std::cin >> size;
  }

// Resize and draw square:
  square->resize(size);
  square->draw();

}
