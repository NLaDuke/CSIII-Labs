// sorting apples
// non STL-algorithm code to be replaced by algorthms
// Mikhail Nesterenko
// 10/03/2018


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector; using std::deque;

struct Apples{
   double weight; // oz
   string color;  // red or green
   void print() const { cout << color << ", " <<  weight << endl; }
};



int main(){
   srand(10);
   const double minWeight = 8.;
   const double maxWeight = 3.;

   cout << "Input crate size: ";
   int size;
   cin >> size;

   vector <Apples> crate(size);

   // assign random weight and color to apples in the crate
   // replace with generate()
   for(auto it = crate.begin(); it != crate.end(); ++it){
      it->weight = minWeight +
	           static_cast<double>(rand())/RAND_MAX*(maxWeight - minWeight);
      it->color = rand() % 2 == 1 ? "green" : "red";
   }


   cout << "Enter weight to find: ";
   double toFind;
   cin >> toFind;

   // count_if()
   int cnt = 0;
   for(auto it = crate.cbegin(); it != crate.cend(); ++it)
      if(it->weight > toFind) ++cnt;

   cout << "There are " << cnt << " apples heavier than "
	<< toFind << " oz" <<  endl;

   // find_if()
   cout << "at positions ";
   for(int i=0; i < size; ++i)
      if(crate[i].weight > toFind)
	 cout << i << ", ";
   cout << endl;


   // max_element()
   double heaviest = crate[0].weight;
   for(int i=1; i < size; ++i)
      if(crate[i].weight > heaviest) heaviest = crate[i].weight;
   cout << "Heaviest apple weighs: " << heaviest << " oz" << endl;


   // for_each() or accumulate()
   double sum = 0;
   for(int i=0; i < size; ++i)
      sum += crate[i].weight;
   cout << "Total apple weight is: " << sum << " oz" << endl;


   // transform();
   cout << "How much should they grow: ";
   double toGrow;
   cin >> toGrow;
   for(int i=0; i < crate.size(); ++i)
      crate[i].weight += toGrow;

   // remove_if()
   cout << "Input minimum acceptable weight: ";
   double minAccept;
   cin >> minAccept;

   for(auto it=crate.begin(); it != crate.end(); )
      if(it->weight < minAccept)
	 it = crate.erase(it);
      else
	 ++it;

   cout << "removed " << size - crate.size() << " elements" << endl;


   // bubble sort, replace with sort()
   bool swapped;
   do{
      swapped = false;
      for(int i=0; i < crate.size()-1; ++i)
	 if(crate[i].weight > crate[i+1].weight){
	    std::swap(crate[i], crate[i+1]);
	    swapped = true;
	 }
   }while(swapped);


   // moving all red apples from crate to peck
   // remove_copy_if() with back_inserter()/front_inserter() or equivalents
   deque<Apples> peck;
   for(auto it=crate.begin(); it != crate.end();)
      if(it->color == "red"){
	 peck.push_front(std::move(*it));
	 it=crate.erase(it);
      }else
	 ++it;

   // for_each() possibly
   cout << "apples in the create"<< endl;
   for(const auto &e: crate) {
      e.print();
   }

   cout << endl;

   // for_each() possibly
   cout << "apples in the peck"<< endl;
   for(const auto &e: peck) {
      e.print();
   }


   // prints every "space" apple in the peck
   //
   const int space=3;
   cout << "\nevery " << space << "\'d apple in the peck"<< endl;

   // replace with advance()/next()/distance()
   // no iterator arithmetic
   auto it=peck.cbegin(); int i = 0;
   while(it != peck.cend()){
      if(i == space){
	 it->print();
	 i=0;
      }
      ++i;
      ++it;
   }


   // putting all small green apples in a jam
   // use a binder to create a functor with configurable max weight
   // accumulate() or count_if() then remove_if()
   const double weightToJam = 10.0;
   double jamWeight = 0;
   for(auto it=crate.begin(); it != crate.end();)
      if(it->weight < weightToJam){
	 jamWeight += it->weight;
	 it=crate.erase(it);
      }else
	 ++it;

   cout << "Weight of jam is: " << jamWeight << endl;

}
