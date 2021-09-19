#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector; using std::deque;
using namespace std::placeholders;

//----------------------------
// File: apple_sorting.cpp
// By: Nolan LaDuke
// Date: 2/23/2021
//------------------------------------------------------------------------------
// Function: Performs actions on a container of 'Apples' using STL algorithms
//------------------------------------------------------------------------------
// Based on a file provided by Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------

// Structure for apples object
struct Apples{
   double weight; // oz
   string color;  // red or green
   void print() const { cout << color << ", " <<  weight << endl; };
};

//Function used for binding later
  // Checks if rhs.weight < compare; if it is then adds rhs.weight to lhs.weight
Apples addApplesIf(Apples lhs, const Apples& rhs, double compare){
  if(rhs.weight < compare){ lhs.weight += rhs.weight;}
  return lhs;
}

int main(){
  // Set up crate
    srand(10);
  // srand(time(NULL));
   const double minWeight = 8.;
   const double maxWeight = 3.;

   cout << "Input crate size: ";
   int size;
   cin >> size;

   while(size <= 0){
     cout << "ERROR: Please input a valid crate size (Must be > 0): ";
     cin >> size;
   }

   vector <Apples> crate(size);

//------------------------------------------------------------------------------

// generate()
  // Assign random weight and color to apples in the crate
   std::generate(crate.begin(), crate.end(), [&]() -> Apples {
    // Lambda function to generate a new apple with random color + weight
     Apples newApple;
     newApple.weight = minWeight +
              static_cast<double>(rand())/RAND_MAX*(maxWeight - minWeight);
     newApple.color = rand() % 2 == 1 ? "green" : "red";
     return newApple;
   });

//------------------------------------------------------------------------------

cout << "Enter weight to find: ";
double toFind;
cin >> toFind;

// count_if()
  // Counts the number of apples with minimum weight 'toFind'
   int cnt = std::count_if(crate.begin(), crate.end(),
                           [&toFind](Apples ap) {return ap.weight > toFind;});

   cout << "There are " << cnt << " apples heavier than "
	       << toFind << " oz" <<  endl;

//------------------------------------------------------------------------------

// find_if()
  // Finds the position of all apples with minimum weight 'toFind'
   cout << "at positions ";

  // For loop starts at first instance of a valid 'Apples'
  // Then it repeatedly searches for valid 'Apples' in the range (lastFound, end)
  // Printing the location of each valid 'Apples'
  // Until it does not find a valid 'Apples'
   for(auto itr = std::find_if(crate.begin(), crate.end(), [&toFind](Apples ap){
                                          return ap.weight > toFind; });
       itr != crate.end();
       itr = std::find_if(itr+1, crate.end(), [&toFind](Apples ap){
                                              return ap.weight > toFind; })
      ){
       // Inside of for loop:
        cout << itr - crate.begin() << ", ";
      }
    cout << endl;

//------------------------------------------------------------------------------

// max_element()
  // Finds the weight of the heaviest Apple
   auto itr = std::max_element(crate.begin(), crate.end(),
                                [](Apples ap1, Apples ap2){
                                  return ap1.weight < ap2.weight;
                                });

   cout << "Heaviest apple weighs: " << itr->weight << " oz" << endl;

//------------------------------------------------------------------------------

// for_each() or accumulate()
 // Finds the total weight of all Apples
   Apples sum = accumulate(crate.begin(), crate.end(), Apples(),
                    [](Apples lhs, Apples rhs){
                      lhs.weight += rhs.weight;
                      return lhs;
                    });

   cout << "Total apple weight is: " << sum.weight << " oz" << endl;

//------------------------------------------------------------------------------

// transform();
  // Takes in a user amount and adds that to every Apple's weight
   cout << "How much should they grow: ";
   double toGrow;
   cin >> toGrow;

  // Transform every apple by adding 'toGrow' to its weight
   std::transform(crate.begin(), crate.end(), crate.begin(),
                  [&toGrow](Apples ap) -> Apples{
                    ap.weight += toGrow;
                    return ap;
                  });

//------------------------------------------------------------------------------

// remove_if()
  // Takes in a user input and removes all Apples with weight < that value
    cout << "Input minimum acceptable weight: ";
    double minAccept;
    cin >> minAccept;

   // Remove any apples with weight < 'minAccept'
    auto end = std::remove_if(crate.begin(), crate.end(),
                              [&minAccept](Apples ap){
                                return (ap.weight < minAccept);
                              });

   // Resize vector to remove old / deleted elements
    int newSize = end - crate.begin();
    cout << "removed " << size - newSize << " elements" << endl;
    crate.resize(newSize);

//------------------------------------------------------------------------------

// bubble sort, replace with sort()
  // Sorts the elements in 'crate' based on apple weight, Lowest -> Highest
  std::sort(crate.begin(), crate.end(), [](Apples lhs, Apples rhs){
                                            return (lhs.weight < rhs.weight);
                                          });

//------------------------------------------------------------------------------


// remove_copy_if() with back_inserter()/front_inserter() or equivalents
  // moving all red apples from crate to peck

 // Declare variables:
  deque<Apples> peck;               // Container for 'red' apples
  auto binst = back_inserter(peck); // Inserted for 'remove_copy_if'

 // Copy 'red' apples into 'peck'
  std::remove_copy_if(crate.begin(), crate.end(), binst,
                      [](Apples ap){
                        return ap.color == "green";
                      });

 // Remove 'red' apples from 'crate'
  auto newEnd = std::remove_if(crate.begin(), crate.end(),
                               [](Apples ap){
                                 return ap.color == "red";
                               });

 // Resize 'crate' to remove old / deleted elements
  int newSize2 = newEnd - crate.begin();
  crate.resize(newSize2);

//------------------------------------------------------------------------------

// for_each() possibly
  // Printing all Apples in the crate
   cout << "apples in the crate"<< endl;
   for_each(crate.begin(), crate.end(), [](Apples ap) {ap.print();});

   cout << endl;

//------------------------------------------------------------------------------

// for_each() possibly
  // Printing all Apples in the peck
   cout << "apples in the peck"<< endl;
   for_each(peck.begin(), peck.end(), [](Apples ap) {ap.print();});

//------------------------------------------------------------------------------

// NOTE TO GRADER:
  // This code will NOT output the same as the code provided as that code is
  // implemented inproperly and prints [4, 7, 10, ...] instead of [3, 6, 9, ...]

// replace with advance()/next()/distance()
  // prints every "space" apple in the peck
   const int space=3;
   cout << "\nevery " << space << "\'d apple in the peck"<< endl;

   auto spaceItr = --peck.begin();
   // Decrement here^ to ensure printing [3, 6, 9...] and not [4, 7, 10...]

   while(std::distance(spaceItr, peck.end()) > space){
     std::advance(spaceItr, space);
     spaceItr->print();
   }


//------------------------------------------------------------------------------

// putting all small green apples in a jam
// use a binder to create a functor with configurable max weight
// accumulate() or count_if() then remove_if()

   const double weightToJam = 10.0;


// Function checks if _2.weight < weightToJam; True → add _2.weight to _1.weight
   auto functr = std::bind(addApplesIf, _1, _2, weightToJam);

 // Accumulate using bound function
   Apples jamSum = std::accumulate(crate.begin(), crate.end(), Apples(), functr);
   cout << "Weight of jam is: " << jamSum.weight << endl;

 // Remove "Jammed" apples
   auto crateSize = std::remove_if(crate.begin(), crate.end(),
                                  [&weightToJam](Apples ap) {
                                    return ap.weight < weightToJam;
                                  });

 // Resize to remove old / deleted elements
   crate.resize(crateSize - crate.begin());


}
