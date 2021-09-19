#include <iostream>
#include <ctime>
#include <cstdlib>
#include <map>
#include <vector>
#include <string>

using std::cin; using std::cout; using std::endl;
using std::string; using std::multimap; using std::vector;

//----------------------------
// File: orange_sorting.cpp
// By: Nolan LaDuke
// Date: 2/23/2021
//------------------------------------------------------------------------------
// Function: Generates random 'fruits', then prints out the color of each
//      orange separated by a ' '. Implemented using a multimap
//------------------------------------------------------------------------------
// Based on a file provided by Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------

// Setup to convert random-integers into usable data
enum class Variety {orange, pear, apple};
vector<string> colors = {"red", "green", "yellow"};

int main(){
 // Set seed
  srand(time(nullptr));

 // Set up variables:
  multimap<Variety, string> tree;   // Structure to contain generated fruit
  int numOfFruits = (rand()%100+1); // Number of fruits to generate

   for(int i = 0; i < numOfFruits; i++){
     // Generate fruit pair and add it to tree
      tree.emplace(std::make_pair(static_cast<Variety>(rand() % 3),
                                  colors[rand() % 3]));
   }

   cout << "Colors of the oranges: ";
  // Iterate through oranges and print out the color of each
   for(auto f=tree.lower_bound(Variety(0)); f!=tree.upper_bound(Variety(0));++f)
      cout << f->second << " ";
   cout << endl;

}
