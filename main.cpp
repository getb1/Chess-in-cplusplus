#include <iostream>
#include <list>
#include <string>
#include <cctype>
#include <cmath>
#include <vector>
#include <array>

typedef unsigned long long U64;

int getNthBit(U64 num, int n) {
	//get the value of the nth bit
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
		//set a bit to 1
		return num | (static_cast<U64>(1) << target_bit);
}

U64 setBitzero(U64 num, int target_bit) {
	U64 mask = ~(1ULL << target_bit);  // Shift 1 to the left by 'pos', then negate

    // Use bitwise AND to clear the bit at 'pos'
    return num & mask;

}

template<typename T>
T get_value(std::list<T> given, int position) {
		//get value of any item in a list
		auto start = given.begin();
		std::advance(start, position);
		return *start;
}

class Game {
	
	
	public:
		U64 white;
		U64 black;
		U64 rooks;
		U64 knights;
		U64 bishops;
		U64 kings;
		U64 queens;
		U64 pawns;
		std::list<U64> precomp_knight_moves;
		
		const int BOARD_SIZE = 8;
		const int BOARD_AREA = BOARD_SIZE*BOARD_SIZE;
		std::list<U64> rookPrecompMoves;
		std::list<U64> bishopPrecompMoves;
		std::list<U64> queenPrecompMoves;
		std::list<U64> kingPrecompMoves;
		Game() {
				init();
		}
		
	void init() {
		// initialise the board to the starting position to start the game
		white = 0x000000000000ffff;
		black = 0xffff000000000000;
		rooks = 0x8100000000000081;
		knights=0x4200000000000042;
		bishops=0x2400000000000024;
		kings = 0x1000000000000010;
		queens= 0x0800000000000008;
		pawns = 0x00ff00000000ff00;
		
		//precompute all the knight moves so that we can use these later
		
		precomp_knight_moves = precompute_knight_moves();
		
		std::vector<int> bishopDx = {1, 1, -1, -1};
		std::vector<int> bishopDy = {1, -1, 1, -1};
		std::vector<int> rookDx = {0, 0, 1, -1};
		std::vector<int> rookDy = {1, -1, 0, 0};
		std::vector<int> queenDx = {0, 0, 1, -1, 1, 1, -1, -1};
		std::vector<int> queenDy = {1, -1, 0, 0, 1, -1, 1, -1};
		
		rookPrecompMoves = precomputePieceMoves(rookDx, rookDy);
        bishopPrecompMoves = precomputePieceMoves(bishopDx, bishopDy);
        queenPrecompMoves = precomputePieceMoves(queenDx, queenDy);
        kingPrecompMoves = precomputeKingMoves(queenDx, queenDy);
		
		}
		
	void display_board() {
			//display the board from all the bitboards we defined earlier
			std::cout << "  1 2 3 4 5 6 7 8\n";
			
			//all the bitboards
			std::list<U64> bit_boards = {rooks,knights,bishops,kings,queens,pawns};
			std::list<char> symbols = {'r','n','b','k','q','p'};
			std::list<char> board(BOARD_AREA, '-');
			U64 all_pieces = black | white;
			
			
			for (auto symbol = symbols.begin(); symbol!=symbols.end(); ++symbol) {
					U64 board_being_used = bit_boards.front();
					bit_boards.pop_front();
					
					for (int j =0; j<BOARD_AREA; ++j) {
							
							if(getNthBit(board_being_used, j)) {
									//we can set the sqaure in the board to the pieces symbol if it is there
									char toUse = *symbol;
									if (getNthBit(white,j)) {
										    //if the piece is white capitalise it for standrd display
											toUse = std::toupper(toUse);
									}
									board = change_value(board,toUse,j);
							}
							
					}
			}
			
			int counter = 0;
			for (auto sqaure = board.begin(); sqaure!=board.end();++sqaure) {
					if(counter %BOARD_SIZE == 0) {
							std::cout << (counter/BOARD_SIZE) + 1 << " ";
					}
					std::cout << *sqaure << " ";
					counter++;
					if(counter % BOARD_SIZE ==0) {
							std::cout << std::endl;
					}
			}
		
	}
	
	void debug_display_bitboard(U64 board, char symbol) {
			
			
			// the same logic as the display function however only displays one board
			for(int i =0;i<BOARD_AREA;++i) {
			
					if(getNthBit(board, i)) {
							std::cout << symbol << " ";
					} else {std::cout << "· ";}
					if((i+1)%BOARD_SIZE==0) {
							std::cout << "\n";
					}
				
			}
			
			
			
			
	}
	
	std::list<U64> precompute_knight_moves() {
			 const int dx[] = {2,1,-2,-1,2,1,-2,-1};
			 const int dy[] = {1,2, 1, 2,-1,-2,-1,-2};
			 //all posible offsets
			 int file,rank,target_rank,target_file, target_sq;
			 U64 move_mask;
			 
			 std::list<U64> legal_moves(BOARD_AREA, 0ULL);
			 //loop through all the sqaures
			 for(int i = 0; i<BOARD_AREA; ++i) {
				move_mask = 0ULL;
				file = i%BOARD_SIZE;
				rank = floor(i/BOARD_SIZE);
				
				for(int j =0;j<8;++j) {
					//loop through all offsets
					target_file = file+dy[j];
					target_rank = rank+dx[j];
					//if the target sqaure is on the board
					if((target_file>=0&&target_file<BOARD_SIZE)&&(target_rank>=0&&target_rank<BOARD_SIZE)) {
						
						target_sq = (target_rank*BOARD_SIZE)+target_file;
						move_mask = setBit(move_mask,target_sq);
						// set the target_sq in the move mask to 1 so we know we can get there
						
					}
				}
						
				legal_moves=change_value(legal_moves, move_mask, i);
				//append the bitboard to the end of the list
					
			 }
			 
			 return legal_moves;
	}
	
	std::list<U64> precomputeKingMoves(std::vector<int>& dx, std::vector<int>& dy) {
		
		int file,rank,target_rank,target_file, target_sq;
			 U64 move_mask;
			 
			 std::list<U64> legal_moves(BOARD_AREA, 0ULL);
			 //loop through all the sqaures
			 for(int i = 0; i<BOARD_AREA; ++i) {
				move_mask = 0ULL;
				file = i%BOARD_SIZE;
				rank = floor(i/BOARD_SIZE);
				
				int length = sizeof(dx)/sizeof(dx[0]);
				
				for(int j =0;j<length;++j) {
					//loop through all offsets
					
					target_file = file+dy[j];
					target_rank = rank+dx[j];
					//if the target sqaure is on the board
					if((target_file>=0&&target_file<BOARD_SIZE)&&(target_rank>=0&&target_rank<BOARD_SIZE)) {
						
						target_sq = (target_rank*BOARD_SIZE)+target_file;
						move_mask = setBit(move_mask,target_sq);
						// set the target_sq in the move mask to 1 so we know we can get there
						
					}
				}
				//move_mask = setBitzero(move_mask,i);
				legal_moves=change_value(legal_moves, move_mask, i);
				//append the bitboard to the end of the list
					
			 }
			 
			 return legal_moves;
	    
	    
	}
	
	std::list<U64> precomputePieceMoves(std::vector<int>& dx, std::vector<int>& dy) {
	    
	    
			 //all posible offsets
			 int file,rank,target_rank,target_file, target_sq;
			 U64 move_mask;
			 
			 std::list<U64> legal_moves(BOARD_AREA, 0ULL);
			 //loop through all the sqaures
			 for(int i = 0; i<BOARD_AREA; ++i) {
				move_mask = 0ULL;
				file = i%BOARD_SIZE;
				rank = floor(i/BOARD_SIZE);
				
				int length = sizeof(dx)/sizeof(dx[0]);
				
				for(int j =0;j<length;++j) {
					//loop through all offsets
					for(int x=1;x<9;++x) {
					target_file = file+dy[j]*x;
					target_rank = rank+dx[j]*x;
					//if the target sqaure is on the board
					if((target_file>=0&&target_file<BOARD_SIZE)&&(target_rank>=0&&target_rank<BOARD_SIZE)) {
						
						target_sq = (target_rank*BOARD_SIZE)+target_file;
						move_mask = setBit(move_mask,target_sq);
						// set the target_sq in the move mask to 1 so we know we can get there
						
					}
				}}
				move_mask = setBitzero(move_mask,i);
				legal_moves=change_value(legal_moves, move_mask, i);
				//append the bitboard to the end of the list
					
			 }
			 
			 return legal_moves;
	    
	    
	}
		
	};


int main() {
		Game chess;
		
		chess.display_board();
		char x = '*';
		chess.debug_display_bitboard(chess.rookPrecompMoves.front(), x);
		
		
		
		return 0;
}
