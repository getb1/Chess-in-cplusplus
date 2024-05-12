#include "chess.h"
#include <iostream>
#include <list>
#include <string>
#include <cctype>
#include <cmath>
#include <vector>
#include <array>
#include "misc.h"
Game::Game() {init();} 
typedef unsigned long long U64;		
void Game::init() {
		// initialise the board to the starting position to start the game
		white = 0x000000000000ffff;
		black = 0xffff000000000000;
		rooks = 0x8100000000000081;
		knights=0x4200000000000042;
		bishops=0x2400000000000024;
		kings = 0x1000000000000010;
		queens= 0x0800000000000008;
		pawns = 0x00ff00000000ff00;
		kingsNotMoved = 0b11; // store wether either king has moved using  3 bit number 
                       // 3 - Neither king has moved
                       // 2 - Black king has moved but white hasn't
                       // 
    rooksNotMoved = 0b1111 // same system for the rooks
		// precompute all the knight moves so that we can use these later
		
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
		whitePawnPrecompMoves = precompPawnMoves(1);
    blackPawnPrecompMoves = precompPawnMoves(0);
		}
		
void Game::display_board() {
			// display the board from all the bitboards we defined earlier
			std::cout << "  1 2 3 4 5 6 7 8\n";
			
			// all the bitboards
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

void Game::debug_display_bitboard(U64 board, char symbol) {
			
			std::cout << "  a b c d e f g h\n";
			
			// the same logic as the display function however only displays one board
			for(int i =0;i<BOARD_AREA;++i) {
					if(i %BOARD_SIZE == 0) {
							std::cout << (i/BOARD_SIZE) + 1 << " ";
					}
					if(getNthBit(board, i)) {
							std::cout << symbol << " ";
					} else {std::cout << "· ";}
					if((i+1)%BOARD_SIZE==0) {
							std::cout << "\n";
					}
				
			}
			
			
			
			
	}
	
std::list<U64> Game::precompute_knight_moves() {
			 const int dx[] = {2,1,-2,-1,2,1,-2,-1};
			 const int dy[] = {1,2, 1, 2,-1,-2,-1,-2};
			 // all posible offsets
			 int file,rank,target_rank,target_file, target_sq;
			 U64 move_mask;
			 
			 std::list<U64> legal_moves(BOARD_AREA, 0ULL);
			 // loop through all the sqaures
			 for(int i = 0; i<BOARD_AREA; ++i) {
				move_mask = 0ULL;
				file = i%BOARD_SIZE;
				rank = floor(i/BOARD_SIZE);
				
				for(int j =0;j<8;++j) {
					// loop through all offsets
					target_file = file+dy[j];
					target_rank = rank+dx[j];
					// if the target sqaure is on the board
					if((target_file>=0&&target_file<BOARD_SIZE)&&(target_rank>=0&&target_rank<BOARD_SIZE)) {
						
						target_sq = (target_rank*BOARD_SIZE)+target_file;
						move_mask = setBit(move_mask,target_sq);
						// set the target_sq in the move mask to 1 so we know we can get there
						
					}
				}
						
				legal_moves=change_value(legal_moves, move_mask, i);
				// append the bitboard to the end of the list
					
			 }
			 
			 return legal_moves;
	}
	
std::list<U64> Game::precomputeKingMoves(std::vector<int>& dx, std::vector<int>& dy) {
		
		int file,rank,target_rank,target_file, target_sq;
			 U64 move_mask;
			 
			 std::list<U64> legal_moves(BOARD_AREA, 0ULL);
			 // loop through all the sqaures
			 for(int i = 0; i<BOARD_AREA; ++i) {
				move_mask = 0ULL;
				file = i%BOARD_SIZE;
				rank = floor(i/BOARD_SIZE);
				
				int length = sizeof(dx)/sizeof(dx[0]);
				
				for(int j =0;j<length;++j) {
					// loop through all offsets
					
					target_file = file+dy[j];
					target_rank = rank+dx[j];
					// if the target sqaure is on the board
					if((target_file>=0&&target_file<BOARD_SIZE)&&(target_rank>=0&&target_rank<BOARD_SIZE)) {
						
						target_sq = (target_rank*BOARD_SIZE)+target_file;
						move_mask = setBit(move_mask,target_sq);
						// set the target_sq in the move mask to 1 so we know we can get there
						
					}
				}
				// move_mask = setBitzero(move_mask,i);
				legal_moves=change_value(legal_moves, move_mask, i);
				// append the bitboard to the end of the list
					
			 }
			 
			 return legal_moves;
	    
	    
	}
	
std::list<U64> Game::precomputePieceMoves(std::vector<int>& dx, std::vector<int>& dy) {
	    
	    
			 // all posible offsets
			 int file,rank,target_rank,target_file, target_sq;
			 U64 move_mask;
			 
			 std::list<U64> legal_moves(BOARD_AREA, 0ULL);
			 // loop through all the sqaures
			 for(int i = 0; i<BOARD_AREA; ++i) {
				move_mask = 0ULL;
				file = i%BOARD_SIZE;
				rank = floor(i/BOARD_SIZE);
				
				int length = sizeof(dx)/sizeof(dx[0]);
				
				for(int j =0;j<length;++j) {
					// loop through all offsets
					for(int x=1;x<9;++x) {
					target_file = file+dy[j]*x;
					target_rank = rank+dx[j]*x;
					// if the target sqaure is on the board
					if((target_file>=0&&target_file<BOARD_SIZE)&&(target_rank>=0&&target_rank<BOARD_SIZE)) {
						
						target_sq = (target_rank*BOARD_SIZE)+target_file;
						move_mask = setBit(move_mask,target_sq);
						// set the target_sq in the move mask to 1 so we know we can get there
						
					}
				}}
				move_mask = setBitzero(move_mask,i);
				legal_moves=change_value(legal_moves, move_mask, i);
				// append the bitboard to the end of the list
					
			 }
			 
			 return legal_moves;
	    
	    
	}
	
U64 Game::get_legal_moves_for_piece_in_position(int position, U64 board, std::list<U64> precomputed,U64 colour_board) {
		
	if(getNthBit(board, position)) {
		
		U64 moves = get_value(precomputed, position);
		U64 all_other_pieces = setBitzero(white|black, position);
		int file,rank,square;
		U64 opposite = ((white&colour_board)==white) ? black: white;
		// check upwards moves
		// e.g. the bitboard is like this and one of the pieces is blocking a move
		// 0001000
		// 000*000
		// 111x111
		// the highest one should be blocked
		
		rank = floor(position/8);
		file = position%8;
		int all=0;
		for(int i=-1;i>-BOARD_SIZE;--i) {
				square = 8*(rank+i)+file;
				// get the value of the sqaure
				if((square<0)) {break;} else {
				/* Check the piece is on the baord and if so
				  * then check if there is a collision whit a whitepiece or if we have
				  * already had a collision then blank that sqaure and set the all variable
				  * to one to continue blanking sqaures eve if we have no collison as the 
				  * remainig sqaures should be blocked
				  * The next loops all use the same logic however horizontal checks check to see if
				  * the piece is on the same rank rather than on the board. */
				if(getNthBit(colour_board,square)||all) {
					moves = setBitzero(moves, square);
					all = 1;	
				} else if(getNthBit(opposite, square)) {
						// if we have a collision with the opposite colour then we know we can keep itas a capture and set the all variable to 1
						all=1;
				}
			
				}
		}
		all=0;
		for(int i=1;i<BOARD_SIZE;++i) {
				square = 8*(rank+i)+file;
				
				if((square>63)) {break;} else {
					
				 
				if(getNthBit(colour_board,square)||all) {
					moves = setBitzero(moves, square);
					all = 1;	
				} else if(getNthBit(opposite, square)) {
						all=1;}
				}}
		all=0;
		for(int i=-1;i>-BOARD_SIZE;--i) {
				square = (rank*8)+file+i;
				
				if((square/8)!=rank) {break;} else {
				if(getNthBit(colour_board,square)||all) {
					moves = setBitzero(moves, square);
					all = 1;	
				} else if(getNthBit(opposite, square)) {
						all=1;}
				}}
		all=0;
		for(int i=1;i<BOARD_SIZE;++i) {
				square = (rank*8)+file+i;
				
				if((square/8)!=rank) {break;} else {
				if(getNthBit(colour_board,square)||all) {
					moves = setBitzero(moves, square);
					all = 1;	
				} else if(getNthBit(opposite, square)) {
						all=1;}
				}}
		
		all=0;
		std::vector<int> bishopDx = {1, 1, -1, -1};
		std::vector<int> bishopDy = {1, -1, 1, -1};
		int newRank,newFile,oldrank,oldfile,newSquare;
		for(int i=0;i<4;++i) {
				int offsetX = bishopDx[i];
				int offsetY = bishopDy[i];
				for(int j=1;j<8;++j) {
						oldrank = rank;
						oldfile = file;
						newRank = rank+(offsetY*j);
						newFile = file+(offsetX*j);
						newSquare = (newRank*8)+newFile;
						if(newSquare<64) {
								if((newFile>=0)&&(newFile<8)) {
										if((newRank>=0)&&(newRank<8)) {
											if(getNthBit(colour_board,newSquare)||all) {
												moves = setBitzero(moves, newSquare);
												all = 1;	
											} else if(getNthBit(opposite, newSquare)) {
													all=1;}	
										}
								} else {break;}
						} else {
								break;
						}
						
						
				}
				
				
		}
		
		return moves;
		}
		return 0ULL;
		}
	
	
  

std::list<U64> Game::precompPawnMoves(int colour) {
    std::list<U64> legal_moves(64,0ULL);
    U64 move_mask = 0ULL;
    int startRank = (colour) ? 1:6;
    int finalRank = (colour) ? 0:7;
    int multiplier = (colour) ? 1:-1;
    int rank,file;
    for (int i = 0; i < BOARD_AREA; i++) {
      move_mask=0ULL;
      rank=floor(i/8);
      file = i%8;
      if(((rank<startRank)&&(startRank==2))||((rank>startRank)&&(startRank==6))) {
        continue;}
  
      if(rank==startRank) {
        
        move_mask = setBit(move_mask, i+(16*multiplier));

      }
      if(rank!=finalRank) {

      move_mask = setBit(move_mask,i+(8*multiplier));

      }
    legal_moves = change_value(legal_moves, move_mask,i);
    }
  return legal_moves;
  }
U64 Game::get_legal_knight_moves_in_positon(int position, U64 board, int colour) {

  if(getNthBit(board,position)) {
    U64 pieceColour = (colour) ? white:black;
    U64 opposite = (colour) ? black:white;
    U64 precomputed = get_value(precomp_knight_moves, position);
    U64 conflicts = precomputed&pieceColour;
    U64 legal_moves = conflicts^precomputed;
    
    return legal_moves;
  }


  return 0ull;
}

U64 Game::get_legal_pawn_moves_in_position(int position, U64 board, int colour, int enPasssq) {

  if(getNthBit(board,position)) {

  const U64 pieceColour = (colour) ? white:black;
  const U64 opposite = (colour) ? black:white;
  const U64 precomputed = (colour) ? get_value(whitePawnPrecompMoves,position):get_value(blackPawnPrecompMoves,position);
  const int rank = floor(position/8);
  const int startRank = (colour) ? WHITE_PAWN_START_RANK:BLACK_PAWN_START_RANK;
  const int twoMoves = (rank==startRank) ? 1:0;
  const int multiplier = (colour) ? 1:-1;
  const int collisions = (black|white)&precomputed;

  U64 legal_moves = precomputed;
  const int range = (twoMoves) ? 3:2;
  
    int blockAll = 0;
    for(int i=1;i<range;++i) {
      int newPos = position+((i*BOARD_SIZE)*multiplier);
      if(getNthBit(collisions, position+((i*BOARD_SIZE)*multiplier))||blockAll) {

        legal_moves = setBitzero(legal_moves, newPos);
        
      }

    }
  const int file = position%BOARD_SIZE;
  int dx[] = {1,-1};
  int dy = 1 * multiplier;
  
  for(int i=0; i<2;++i) {

    int new_file = file+dx[i];
    int new_rank = rank+(dy*multiplier);
    int takepos = (new_rank*8)+new_file;
    if(((new_file >=0)&&(new_file<BOARD_SIZE))&&((new_rank >=0)&&(new_rank<BOARD_SIZE))) {
      if(getNthBit(opposite, takepos)) {
        legal_moves=setBit(legal_moves, takepos);

      }

    }
  }
  
  if(enPasssq!=-1) {
    static const int enPassRank = floor(enPasssq/BOARD_SIZE);
    static const int enPassFile = enPasssq%BOARD_SIZE;
    if(enPassRank==rank+(1*multiplier)) {
      if((enPassFile==file-1)||(enPassFile==file+1)) {
        legal_moves = setBit(legal_moves, enPasssq);}

    }
    

  }

  return legal_moves;
}
return 0ull;
}

U64 Game::generate_attack_map(int colour) {

  static const U64 colour_board = (colour) ? white:black;
  static const U64 rooks_board  = colour_board&rooks;
  static const U64 knight_board = colour_board&knights;
  static const U64 bishops_board= colour_board&bishops;
  static const U64 queen_board  = colour_board&queens;
  static const U64 king_board   = colour_board&kings; 
  U64 attackMap = 0ULL
  for(int i=0;i<BOARD_AREA) {

    if(getNthBit(rooks_board,i)) {
      
    attackMap |= get_legal_moves_for_piece_in_position(i, rooks_board, rookPrecompMoves, colour_board);
    } else if(getNthBit(knight_board,i)) {
      attackMap |= get_legal_knight_moves_in_positon(i, knight_board, colour);
    } else if(getNthBit(bishops_board,i)) {
      attackMap |= get_legal_moves_for_piece_in_position(i, bishops_board,bishopPrecompMoves, colour_board);
    }

  }

}
