#include "misc.h"
#include <iterator>

int getNthBit(U64 num, int n) {
    // get the value of the nth bit (0 or 1)
    return (num >> n) & 1;
}

template<typename T>
std::list<T> change_value(std::list<T> toChange, const T& value, int position) {
    // change the value of an element in the list at the specified position
    auto start = toChange.begin();
    std::advance(start, position);
    *start = value;
    return toChange;
}

U64 setBit(U64 num, int target_bit) {
    // set a specific bit in num to 1
    return num | (static_cast<U64>(1) << target_bit);
}

U64 setBitzero(U64 num, int target_bit) {
    // clear a specific bit in num (set it to 0)
    U64 mask = ~(1ULL << target_bit);  // Shift 1 to the left by 'target_bit', then negate
    return num & mask;  // Use bitwise AND to clear the bit at 'target_bit'
}

template<typename T>
T get_value(std::list<T> given, int position) {
    // get the value of an element in the list at the specified position
    auto start = given.begin();
    std::advance(start, position);
    return *start;
}

// Explicit instantiations for template functions
template std::list<int> change_value(std::list<int> toChange, const int& value, int position);
template std::list<char> change_value(std::list<char> toChange, const char& value, int position);
template std::list<U64> change_value(std::list<U64> toChange, const U64& value, int position);
template int get_value(std::list<int> given, int position);
template char get_value(std::list<char> given, int position);
template U64 get_value(std::list<U64> given, int position);

