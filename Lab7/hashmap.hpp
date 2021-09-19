#ifndef PERSONALHASH_HPP
#define PERSONALHASH_HPP

#include <cstddef>
#include <utility>
#include <functional>
#include <vector>
#include <algorithm>
#include <list>
#include <iostream>

using std::vector;
using std::list;
using std::pair;
using std::make_pair;

//----------------------------
// File: hashmap.hpp
// By: Nolan LaDuke
// Date: 3/5/2021
//------------------------------------------------------------------------------
// Function: header file for a Hash-Map class
//------------------------------------------------------------------------------
// Based on a file provided by Mikhal Nesterenko, KSU
//------------------------------------------------------------------------------


// Hash Class ------------------------------------------------------------------

template <typename T>
class DefaultHash {
public:
   DefaultHash(size_t numBuckets = defaultNumBuckets);
   size_t hash(const T& key) const;
   size_t numBuckets() const { return numBuckets_; }

private:
   // default number of buckets in the hash
   static const size_t defaultNumBuckets = 101;
   size_t numBuckets_;
};

template <typename T>
DefaultHash<T>::DefaultHash(size_t numBuckets): numBuckets_(numBuckets) {}


// uses the division method for hashing.
// treats the key as a sequence of bytes, sums the ASCII
// values of the bytes, and mods the total by the number
// of buckets.
// note, this function does not work for C++ strings
template <typename T>
size_t DefaultHash<T>::hash(const T& key) const {
   size_t res = 0;
   for (size_t i = 0; i < sizeof(key); ++i) {
      const unsigned char b =
      *(reinterpret_cast<const unsigned char *>(&key) + i);
      res += b;
   }
   return res % numBuckets_;
}



// Container Class -------------------------------------------------------------

template <typename Key, typename Value,
          typename Compare = std::equal_to<Key>,
          typename Hash = DefaultHash<Key>>
class hashmap{

public:
   typedef pair<const Key, Value> Element;

   // constructor
   // invokes constructors for comparison and hash objects
   hashmap(const Compare& comp = Compare(),
	   const Hash& hash = Hash());

   Element* find(const Key& x);      // returns pointer to element with key x,
                                     // nullptr if not found

   std::pair<Value*, bool> insert(const Element& x);   // Safe insert

   std::pair<Element*, bool> erase(const Key& x);  // Modified erase

   Value& operator[] (const Key& x); // returns reference on value of
                                     // element with key, inserts if does not exist

   void rehash(size_t n); // Resizes the hashmap if the provided size
                          // is greater than the current number of buckets

   void debugPrint(); // Prints contents of 'elems_' for debugging purposes

private:

   // helper function for various searches
   typename list<Element>::iterator findElement(const Key& x, const size_t bucket);

   size_t size_;   // number of elements in the container
   Compare comp_;  // comparison functor, equal_to by default
   Hash hash_;     // hash functor

   // hash contents: vector of buckets
   // each bucket is a list containing key->value pairs
   vector<list<Element>> elems_;
};


// Implementation --------------------------------------------------------------

// Construct elems_ with the number of buckets.
template <typename Key, typename Value, typename Compare, typename Hash>
   hashmap<Key, Value, Compare, Hash>::hashmap(
   const Compare& comp, const Hash& hash):
   size_(0), comp_(comp), hash_(hash) {
      elems_ = vector<list<Element>>(hash_.numBuckets());
}


// helper function
template <typename Key, typename Value,
          typename Compare, typename Hash>
   typename list<pair<const Key, Value>>::iterator // return type
   hashmap<Key, Value, Compare, Hash>::findElement(const Key& x, size_t bucket){

   // look for the key in the bucket
   for (auto it =  elems_[bucket].begin(); it != elems_[bucket].end(); ++it)
      if (comp_(it->first, x))
	       return it;

   return elems_[bucket].end(); // element not found
}


// returns a pointer to the element with key x
// returns nullptr if no element with this key
template <typename Key, typename Value, typename Compare, typename Hash>
   typename hashmap<Key, Value, Compare, Hash>::Element* // return value type
   hashmap<Key, Value, Compare, Hash>::find(const Key& x) {

   size_t bucket = hash_.hash(x);
   auto it=findElement(x, bucket);    // use the findElement() helper

   if (it != elems_[bucket].end())
      // found the element. Return a pointer to it.
      return &(*it); // dereference the iterator to list
                     // then take the address of list element

   else // didn't find the element -- return nullptr
      return nullptr;
}


// finds the element with key x, inserts an
// element with that key if none exists yet.
// Performs safe insert and returns std::pair<Value*, bool>
template <typename Key, typename Value, typename Compare, typename Hash>
std::pair<Value*, bool> hashmap<Key, Value, Compare, Hash>::insert(const Element& x) {
   bool didInsert = false;                    // Boolean for if it was inserted
   size_t bucket = hash_.hash(x.first);
   auto it = findElement(x.first, bucket);    // try to find the element

   // if not found, insert a new one.
   if (it == elems_[bucket].end()) {
      ++size_;
      elems_[bucket].push_back(x);
      didInsert = true;                  // Elem inserted, so update bool
      it = findElement(x.first, bucket); // Set iterator to newly inserted element
   }

   Value* ptr = &(it->second); // Create pointer to 'Value' of element inserted / found
   return std::make_pair(ptr, didInsert); //Return pair of <pointer, result>

}


// removes the Element with key x, if it exists
template <typename Key, typename Value, typename Compare, typename Hash>
std::pair<typename hashmap<Key, Value, Compare, Hash>::Element*, bool> //Return type
hashmap<Key, Value, Compare, Hash>::erase(const Key& x) {

   size_t bucket = hash_.hash(x);
   auto it = findElement(x, bucket);      // try to find the element
   bool didErase = false;                 // Bool to track if an elem was erased

   typename list<Element>::iterator nextElem; // Iterator to track element returned
                                              // as 'next' in pair

   if (it != elems_[bucket].end()) {      // the element exists, erase it
      nextElem = elems_[bucket].erase(it);
      --size_;
      didErase = true; // Update bool since we erased
   }

  // While nextElem is at the back of its bucket and is not in the last bucket...
    // Then move it to the beginning of the next bucket
  size_t traverseBuckets = bucket;
  while(nextElem == elems_[traverseBuckets].end() && traverseBuckets < (hash_.numBuckets())-1 ){
    ++traverseBuckets; // Increment bucket number;
    nextElem = elems_[traverseBuckets].begin(); // Set nextElem to beginning of next bucket
  }

   if(didErase){
     if(nextElem == elems_[hash_.numBuckets()-1].end()){
       // If erased the very last element, return nullptr
       return std::make_pair(nullptr, didErase);
     }
     // If we erased an element but it wasn't the last, return pointer to next element
     return std::make_pair(&*nextElem, didErase);
   }
   // If element was not find, return pointer to elems_[bucket].end();
   return std::make_pair(&*it, didErase);

}


// returns reference to value of element with key x,
// inserts if does not exist
template <typename Key, typename Value, typename Compare, typename Hash>
Value& hashmap<Key, Value, Compare, Hash>::operator[] (const Key& x) {

  // Use insert to attempt to find an element with key 'x', inserting default value if not found
   auto found = insert(make_pair(x, Value()));
  // Return pointer to either found element or inserted default element
   return *found.first;

}


// Resizes the hashmap if the provided number is greater than the current number
// of buckets
template <typename Key, typename Value, typename Compare, typename Hash>
void hashmap<Key, Value, Compare, Hash>::rehash(size_t n){
 // Check that 'n' > current number of buckets
  if(n <= hash_.numBuckets()){
    return;
  }

 // Set up variables
  vector<list<Element>> newHashMap(n); // Container to store rehashed elements
  auto newHash = DefaultHash<Key>(n); // setup new hash

 // For each List:
  // Iterate through it, using 'newHash' to calculate it's new position
  // and then inserting that element into 'newHashMap' based on that position
  std::for_each(elems_.begin(), elems_.end(),
    [&newHashMap, &newHash](list<Element>& currentList) -> void {
      std::for_each(currentList.begin(), currentList.end(),
        [&newHashMap, &newHash](Element& currentElem) -> void {
          size_t bucket = newHash.hash(currentElem.first);
          newHashMap[bucket].push_back(currentElem);
        });
    });

 // Copy new hash and hashmap into local elements using move
  hash_ = std::move(newHash);
  elems_ = std::move(newHashMap);


}

// Function that prints all elements in the Hashmap in the form:
// 1 : Key | Value, Key | Value, ...
// 2 : Key | Value, Key | Value, ...
// N : Key | Value, ...
// Used for debugging purposes
template <typename Key, typename Value, typename Compare, typename Hash>
void hashmap<Key, Value, Compare, Hash>::debugPrint(){
  int bcktCount = 1;
  std::for_each(elems_.begin(), elems_.end(),
    [&bcktCount](list<Element>& currentList) -> void {
      std::cout << bcktCount << " : ";
      std::for_each(currentList.begin(), currentList.end(),
        [](Element& currentElem) -> void {
          std::cout << currentElem.first << " | " << currentElem.second << ", ";
        });
        std::cout << std::endl;
        ++bcktCount;
    });
}

#endif
