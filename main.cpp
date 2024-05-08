/* Copyright 2024 Guy Bailey */


#include <iostream>
#include <list>
#include <string>
#include <cctype>
#include <cmath>
#include <vector>
#include <array>
#include "chess.h"

typedef long long U64;

/* Todo
 * Diagonal move validation test
 * Pawn Move Precomputation cheking
 * Checks
 * Checkmate
 * Stalemate
 * Making/undoing moves
 * UI
 * */

int getNthBit(U64 num, int n) {
	// get the value of the nth bit
		return 1==((num>>n)&1);
}

template<typename T>
std::list<T> change_value(std::list<T> toChange, const T& value, int position) {
		// change the value of any list to a certain value
		auto start = toChange.begin();
		std::advance(start,position);
		*start = value;
		return toChange;
}

U64 setBit(U64 num, int target_bit) {
		// set a bit to 1
		return num | (static_cast<U64>(1) << target_bit);
}

U64 setBitzero(U64 num, int target_bit) {
	U64 mask = ~(1ULL << target_bit);  // Shift 1 to the left by 'pos', then negate

    // Use bitwise AND to clear the bit at 'pos'
    return num & mask;

}

template<typename T>
T get_value(std::list<T> given, int position) {
		// get value of any item in a list
		auto start = given.begin();
		std::advance(start, position);
		return *start;
}


int main() {
    Game chess;
    chess.display_board();
    U64 rook_legal = chess.get_legal_moves_for_piece_in_position(3, chess.queens, chess.queenPrecompMoves, chess.white); 
    chess.debug_display_bitboard(get_value(chess.whitePawnPrecompMoves, 8), '*');
    return 0;
}
