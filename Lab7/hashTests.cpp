#include "hashmap.hpp"
#include <iostream>
#include <string>
#include <cassert>

using std::cin; using std::cout; using std::endl;
using std::string;

//----------------------------
// File: hashTests.cpp
// By: Nolan LaDuke
// Date: 3/5/2021
//------------------------------------------------------------------------------
// Function: Test file for classes in 'hashmap.hpp' file
//------------------------------------------------------------------------------

// I would have used individual functions for each thing I needed to test, but
// I didn't want to mess around with passing the hashmap around 500 times and
// risk thinking something was broken in the class when it was just a problem here

int main() {
  // Setup int | string hashmap
  hashmap<int, string> testHash;

// Testing 'insert' ============================================================

cout << "Testing functionality of 'hashmap.hpp' file..." << endl
<< "NOTE: Tests assume all un-modified, provided functions [ex: find()] work properly"
<< endl << endl
<< "TESTING INSERTION FUNCTION ---------------------------------------------------"
<< endl;

  // Test insertions:
  cout << "Inserting 1 | Inst1 using 'insert' " << endl;
  auto inst1 = testHash.insert(std::make_pair(1, "Inst1"));
  cout << "Inserting 2 | Inst2 using 'insert' " << endl;
  auto inst2 = testHash.insert(std::make_pair(2, "Inst2"));

  cout << endl << "Checking insertions..." << endl;

// Checks if the inserts' returned boolean as 'true' for a successful insert
  assert(inst1.second);
  assert(inst2.second);

  cout << "System believes it has inserted elements (based on returned pair)"
  << endl << endl << "Checking if true using find()..." << endl;

// Checks if address + contents of insert's elements is the same as the address returned
//   by attempting to find the element
  assert(inst1.first == &testHash.find(1)->second);
  assert(*inst1.first == testHash.find(1)->second);
  assert(inst2.first == &testHash.find(2)->second);
  assert(*inst2.first == testHash.find(2)->second);

  cout << "Insertions were a success!" << endl << endl
  << "Attempting to insert elements with a duplicate key..." << endl << endl;

  auto inst1Again = testHash.insert(std::make_pair(1, "Should Fail"));
  auto inst2Again = testHash.insert(std::make_pair(2, "Should Fail"));

  cout << "Checking insertions..." << endl;

  assert(!inst1Again.second);
  assert(!inst2Again.second);

  cout << "System believes it has not inserted elements (based on returned pair)"
  << endl << endl << "Checking if true using find()..." << endl;

// Cheking if returned values are the old values
  assert(inst1Again.first == inst1.first);
  assert(inst2Again.first == inst2.first);

  cout << "Insertions failed successfully!" << endl << endl;



// Testing 'opreator[]' ========================================================
cout << "TESTING OPERATOR[] -----------------------------------------------------------"
<< endl << endl
  << "Trust me it doesn't even use find(), you can check"
  << endl << endl;

  cout << "Inserting large number of arbitrary elements using insert()..." << endl;
  for(int i = 3; i < 500; ++i){
    inst1 = testHash.insert(std::make_pair(i, string(i%10 + 1, 'a')));
    //Check inserts as we go
    assert(inst1.second);
    assert(*inst1.first == testHash.find(i)->second);
  }

  cout << "All insertions successful!" << endl << endl

  << "Checking operator[] for all inserted elements..." << endl;

  string output;
  for(int i = 3; i < 500; ++i){
    output = testHash[i];
    assert(output == string(i%10 + 1, 'a'));
  }

  cout << "operator[] successfully returns valid elements!"
  << endl << endl
  << "Testing operator[] with unassigned keys..." << endl;

  assert(testHash[1000] == string());
  assert(testHash[1010] == string());
  assert(testHash[5] == "aaaaaa");

  cout << "Tests passed!" << endl << endl;



// Testing erase() function ====================================================
cout << "TESTING ERASE FUNCTION ------------------------------------------------------"
<< endl << endl;

cout << "Testing if erase actually removes elements..." << endl;

//Check if element has a value
assert(testHash[5] != string());
//Erase the element
testHash.erase(5);
//Check to make sure it doesn't have a value anymore
assert(testHash.find(5) == nullptr);

//Repeat of last test with different key
assert(testHash[7] != string());
testHash.erase(7);
assert(testHash.find(7) == nullptr);

cout << "Erases successful!" << endl << endl
<< "Now testing return values for different erases..." << endl;

cout << "Testing Valid Erase at Front / Middle ... ";

// Check that the values used to contain something
assert(testHash[1] != string());
assert(testHash[105] != string());
assert(testHash[200] != string());
assert(testHash[499] != string());

// Erase values
auto er1 = testHash.erase(1);
auto er105 = testHash.erase(105);
auto er200 = testHash.erase(200);
auto er499 = testHash.erase(499);

// Check that the system believes the elements were erased
assert(er1.second);
assert(er105.second);
assert(er200.second);
assert(er499.second);

// Check that the elements were actually erased
assert(testHash.find(1) == nullptr);
assert(testHash.find(105) == nullptr);
assert(testHash.find(200) == nullptr);
assert(testHash.find(499) == nullptr);

// Check that the returned pointer is correct [should point to next element]
assert(er1.first->first == 102);
assert(er105.first->first == 206);
assert(er200.first->first == 354);
assert(er499.first->first == 43);

cout << "Successfuly returns the next element!" << endl

<< "Testing Invalid Erase ... ";

// Attempt to erase an unassigned key and check to make sure it returns
// false to indicate it did not erase
auto erInvalid = testHash.erase(3000);
assert(!erInvalid.second);

auto erInvalid2 = testHash.erase(5000);
assert(!erInvalid2.second);

cout << "Sucessfully returns false!" << endl
<< "Testing Valid Erase at End ... ";

// Erase the current last element in the hash
// Check to make sure it believes it erased the element
// Check to make sure the element was erased
// Check to make sure it returns a pointer to 'nullptr' since its the last element
auto eraseEnd = testHash.erase(456);
assert(eraseEnd.second);
assert(testHash.find(456) == nullptr);
assert(eraseEnd.first == nullptr);

auto eraseNextEnd = testHash.erase(355);
assert(eraseNextEnd.second);
assert(testHash.find(355) == nullptr);
assert(eraseNextEnd.first == nullptr);

cout << "Successfuly returns 'nullptr'!" << endl
<< "Testing modification idiom to remove all elements ... " << endl;

// Using a variant of the loop-modification idiom for iterators, erase all
// elements in the hashmap
std::pair<const int, string>* currentElem = testHash.find(101);
while(currentElem != nullptr){
  currentElem = testHash.erase(currentElem->first).first;
}

//Check if it worked
for(int i = 0; i < 500; i++){
  assert(testHash.find(i) == nullptr);
}

cout << "Sucessfully removes all elements!" << endl << endl;

cout << "TESTING REHASH FUNCTION -----------------------------------------------------"
<< endl << endl
<< "Creating a small hash and filling with elements...";

// Create a small hash function that can be easily printed / scaled up
hashmap<int, string> smallHash(std::equal_to<int>(), DefaultHash<int>(10));

for(int i = 0; i < 25; i++){
  smallHash.insert(std::make_pair(i, string(1, char(65 + i))));
}

cout << "Resulting Data:" << endl;
smallHash.debugPrint();

cout << endl << "Attempting to rehash to a smaller size..." << endl;
smallHash.rehash(5);
smallHash.rehash(2);

cout << "Resulting Data: [should be no change]" << endl;
smallHash.debugPrint();

cout << endl << "Attempting to rehash to a larger size..." << endl;

smallHash.rehash(12);

cout << "Resulting Data: [should be 12 buckets]" << endl;
smallHash.debugPrint();

cout << endl << "Attempting to rehash to a larger size..." << endl;

smallHash.rehash(25);

cout << "Resulting Data: [should have 1 element per bucket]" << endl;
smallHash.debugPrint();


cout << endl << "----- ALL TESTS COMPLETE -----" << endl;
  return 0;
}
