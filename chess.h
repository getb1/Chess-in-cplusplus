#include <iostream>
#include <list>
#include <string>
#include <cctype>
#include <cmath>
#include <vector>
#include <array>




#ifndef GAME_H
#define GAME_H


class Game {
	
	
	public:
    typedef unsigned long long U64;
    void init();
    void display_board();
    void debug_display_bitboard(U64 board, char symbol);
    std::list<U64> precompute_knight_moves();
    std::list<U64> precomputeKingMoves(std::vector<int>& dx, std::vector<int>& dy);
    std::list<U64> precomputePieceMoves(std::vector<int>& dx, std::vector<int>& dy);
    U64 get_legal_moves_for_piece_in_position(int position, U64 board, std::list<U64> precomputed, U64 colour_board);
    std::list<U64> precompPawnMoves(int colour);
    U64 get_legal_knight_moves_in_positon(int position, U64 board, int colour);
    U64 get_legal_pawn_moves_in_position(int position, U64 board, int colour, int enPasssq);
		U64 white;
		U64 black;
		U64 rooks;
		U64 knights;
		U64 bishops;
		U64 kings;
		U64 queens;
		U64 pawns;
		const int BLACK = 0;
		const int WHITE = 1;
		std::list<U64> precomp_knight_moves;
		
		const int BOARD_SIZE = 8;
		const int BOARD_AREA = BOARD_SIZE*BOARD_SIZE;
    const int WHITE_PAWN_START_RANK = 1;
    const int BLACK_PAWN_START_RANK = 6;
		std::list<U64> rookPrecompMoves;
		std::list<U64> bishopPrecompMoves;
		std::list<U64> queenPrecompMoves;
		std::list<U64> kingPrecompMoves;
    std::list<U64> whitePawnPrecompMoves;
    std::list<U64> blackPawnPrecompMoves;
    

		Game();
};
#endif
