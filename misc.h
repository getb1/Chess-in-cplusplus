#ifndef MISC_H
#define MISC_H

#include <list>

typedef unsigned long long U64; // Define U64 type if not already defined

// Function to get the value of the nth bit from a number
int getNthBit(U64 num, int n);

// Function to change the value of an element in a list at a specified position
template<typename T>
std::list<T> change_value(std::list<T> toChange, const T& value, int position);

// Function to set a specific bit in a number to 1
U64 setBit(U64 num, int target_bit);

// Function to set a specific bit in a number to 0
U64 setBitzero(U64 num, int target_bit);

// Function to get the value of an element in a list at a specified position
template<typename T>
T get_value(std::list<T> given, int position);

#endif

