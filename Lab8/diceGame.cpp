#include "diceGame.hpp"

//----------------------------
// File: diceGame.hpp
// By: Nolan LaDuke
// Date: 3/10/2021
//------------------------------------------------------------------------------
// Function: Test file for 'Game' and child classes
//------------------------------------------------------------------------------
// Based on a file provided by Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------

int main() {
   srand(time(nullptr));

   Game* gp = nullptr;

   // play chess 10 times
   for (int i = 0; i < 10; ++i){
      gp = new Chess;
      gp->playGame();
      delete gp;
   }


   // play monopoly 5 times
   for (int i = 0; i < 5; ++i){
      gp = new Monopoly;
      gp->playGame();
      delete gp;
   }

  // Play Dice Game once
   gp = new Dice;
   gp->playGame();
   delete gp;
}
