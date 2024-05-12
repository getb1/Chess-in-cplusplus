/* Copyright 2024 Guy Bailey */


#include <iostream>
#include <list>
#include <string>
#include <cctype>
#include <cmath>
#include <vector>
#include <array>
#include "./chess.h"
#include "./misc.h"


/* Todo
 * Diagonal move validation test
 * Pawn Move Precomputation cheking
 * Checks
 * Checkmate
 * Stalemate
 * Making/undoing moves
 * UI
 * */




int main() {
    Game chess;
    chess.display_board();
    U64 rook_legal = chess.get_legal_moves_for_piece_in_position(3, chess.queens, chess.queenPrecompMoves, chess.white);
    U64 legal_moves = chess.get_legal_pawn_moves_in_position(8, chess.pawns, 1, -1);
    chess.debug_display_bitboard(legal_moves, '*');
    return 0;
}
