#ifndef DICE_GAME_HPP
#define DICE_GAME_HPP

#include <ctime>
#include <cstdlib>
#include <iostream>

using std::cout; using std::endl;

//----------------------------
// File: diceGame.hpp
// By: Nolan LaDuke
// Date: 3/10/2021
//------------------------------------------------------------------------------
// Function: Header and implementation file for Dice Game classes
//------------------------------------------------------------------------------
// Based on a file provided by Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------


// Provided Class Template:
class Game{
public:
   Game():playersCount_(0), movesCount_(0), playerWon_(noWinner){}

   // template method
   void playGame(const int playersCount = 0) {
      playersCount_ = playersCount;
      movesCount_=0;

      initializeGame();

      for(int i=0; !endOfGame(); i = (i+1) % playersCount_){
	       makeMove(i);
	       if (i==playersCount_-1)
	         ++movesCount_;
      }

      printWinner();
   }

   virtual ~Game(){}

protected:
   // primitive operations
   virtual void initializeGame() = 0;
   virtual void makeMove(int player) = 0;
   virtual void printWinner() = 0;
   virtual bool endOfGame() {return playerWon_ != noWinner;} // this is a hook
                   // returns true if winner is decided
   static const int noWinner=-1;

   int playersCount_;
   int movesCount_;
   int playerWon_;
};

// Provided Classes: -----------------------------------------------------------

// Monopoly - a concrete game implementing primitive
// operations for the template method
class Monopoly: public Game {
public:
   // implementing concrete methods
   void initializeGame(){
      playersCount_ = rand() % numPlayers_ + 1 ; // initialize players
   }

   void makeMove(int player) {
      if (movesCount_ > minMoves_){
	 const int chance = minMoves_ + rand() % (maxMoves_ - minMoves_);
	 if (chance < movesCount_) playerWon_= player;
      }
   }

   void printWinner(){
      cout << "Monopoly, player "<< playerWon_<< " won in "
	   << movesCount_<< " moves." << endl;
   }

private:
   static const int numPlayers_ = 8; // max number of players
   static const int minMoves_ = 20; // nobody wins before minMoves_
   static const int maxMoves_ = 200; // somebody wins before maxMoves_
};

// Chess - another game implementing
// primitive operations
class Chess: public Game {
public:
   void initializeGame(){
      playersCount_ = numPlayers_; // initalize players
      for(int i=0; i < numPlayers_; ++i)
	     experience_[i] = rand() % maxExperience_ + 1 ;
   }

   void makeMove(int player){
      if (movesCount_ > minMoves_){
	 const int chance = (rand() % maxMoves_) / experience_[player];
	 if (chance < movesCount_) playerWon_= player;
      }
   }

   void printWinner(){
      cout << "Chess, player " << playerWon_
	   << " with experience " << experience_[playerWon_]
	   << " won in "<< movesCount_ << " moves over"
	   << " player with experience " << experience_[playerWon_== 0 ? 1:0]
           << endl;
   }

private:
   static const int numPlayers_ = 2;
   static const int minMoves_ = 2; // nobody wins before minMoves_
   static const int maxMoves_ = 100; // somebody wins before maxMoves_
   static const int maxExperience_ = 3; // player's experience
                              // the higher, the greater probability of winning
   int experience_[numPlayers_];
};

//-------------------------- End of Provided Code ------------------------------

class Dice : public Game{
public:
  void initializeGame(){
   // Print opening to game
    cout << "Welcome to the dice game!" << endl
    << "In this game, you will role dice against a computer opponent" << endl
    << "There are three rounds, and every round you will have the choice to pass or roll again"
    << endl << "The Computer goes first, and wins all ties." << endl << endl;
   // Set up variable
    playersCount_ = numPlayers_;
  }

  void makeMove(int player){
   // Check for end condition (3 moves or both done)
    if(movesCount_ == 3 || (computerDone && playerDone)){
      playerWon_ = 1;
      return;
    }

   // Based on number passed, execute that player's turn
    switch(player){
      case 0:{ // Case for Computer turn
        if(!computerDone){
         // Print to terminal
          cout << "It's the Computer's turn..." << endl
          << "Rolling: ";

         // Declare variable to track running total for dice
          int diceSum = 0;

         // Roll 5 dice and add them up
          for(int i = 0; i < 5; ++i){
            int diceRoll = (rand() % diceSize) + 1;
            cout << diceRoll << " ";
            diceSum += diceRoll;
          }
          cout << endl;

         // Update Maximum
          computerHighest = (diceSum > computerHighest) ? diceSum : computerHighest;

         // Determine if rolling again
          if(movesCount_ < 2){
            computerDone = rand() % 2;
            if(computerDone)
              cout << "Computer is done rolling" << endl;
            else
              cout << "Computer will roll again" << endl;
          }
          cout << "Computer's highest score: " << computerHighest << endl
               << endl;
        }
        break;
      }

      case 1:{ // Case for player turn
        if(!playerDone){
          // Print to terminal
           cout << "It's Your turn..." << endl
           << "Rolling: ";

          // Declare variable to track running total for dice
           int diceSum = 0;

          // Roll 5 dice and add them up
           for(int i = 0; i < 5; ++i){
             int diceRoll = (rand() % diceSize) + 1;
             cout << diceRoll << " ";
             diceSum += diceRoll;
           }

          // Update Maximum
           playerHighest = (diceSum > playerHighest) ? diceSum : playerHighest;

          // Determine if rolling again
           if(movesCount_ < 2){
             char rollAgain;
             cout << "Roll again? [y, n] ";
             std::cin >> rollAgain;

            // Loop in case of invalid inputs
             while (rollAgain != 'y' && rollAgain != 'n'){
               cout << "ERROR: Invalid input, please try again" << endl;
               cout << "Roll again? [y, n] ";
               std::cin >> rollAgain;
             }

             playerDone = !(rollAgain == 'y');
           }

          // If statement for last-instance formatting
           if(movesCount_ >= 2){
             cout << endl;
           }

        }
        cout << "Player's highest score: " << playerHighest << endl
             << endl;
        break;
      }

      default:{ // Case for invalid argument passed
        std::cerr << "ERROR: INVALID PLAYER TURN OCCURED IN 'Dice' CLASS\n";
      }
    }


  };

  void printWinner(){
   // Determine higher score
    int higherScore = (playerHighest > computerHighest) ? playerHighest : computerHighest;
   // Determine winner and print
    cout << "The Winner is..." << endl;
    if(playerHighest > computerHighest)
      cout << "You!!!" << endl;
    else
      cout << "The Computer..." << endl;
    cout << "With a score of: "
      << higherScore
      << endl;
  };


private:
  static const int numPlayers_ = 2; // Two players [human | cpu]
  static const int minMoves_ = 1;   // Nobody wins before 1 round
  static const int maxMoves_ = 3;   // Both players can go 1-3 times
  static const int diceSize = 6;    // Maximum value rollable by dice

  int playerHighest = 0;    // Contains player's highest score
  int computerHighest = 0;  // Contains cpu's highest score

  bool playerDone = false;   // Contains if human player has passed
  bool computerDone = false; // Contains if cpu player has passed
};




#endif
