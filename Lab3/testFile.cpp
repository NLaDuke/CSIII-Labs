#include "Collection.hpp"
#include <cassert>
#include <iostream>

//--------------------
// File: testFile.cpp
// By: Nolan LaDuke
// File for testing functionality of Collection class
//--------------------------------------------------------

int main(){
  Collection<int> testInts;
  for(int i = 0; i < 50; i++) testInts.add(i);
  testInts.print();

  Collection<Collection<char>> wow;
  Collection<char> a, b, c;

  for(int i = 0; i < 20; i++){
    a.add('a'); a.add('b'); a.add('c');
    b.add('x');
    c.add(char(i+50));
}

  a.print();
  b.print();
  c.print();

  wow.add(c);
  wow.add(b);
  wow.add(a);

  a.remove('b');
  a.print();
  a.remove('a');
  a.print();

  wow.last().print();

  c = b;
  a.swap(b);
  a.print();

  assert(equal(c, a));

  c = a;
  assert(equal(c, a));

  return 0;
}
