/*
Here, position refers to a combination of (12) bitboards which define a specific
chess piece layout. This file contains all necessary function for working with
a position, such as retrieving piece types from squares and working with them.
*/

#include <iostream>
#include <vector>
#include "bitboard.h"

using namespace NerdChess::bitb;

// Piece types
// Used by the get_moves() function
#define EMPTY -1
#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5

// Piece colors
#define WHITE 0
#define BLACK 1

namespace NerdChess
{
namespace board
{
extern int en_pessant_squares_w; // En pessant square for white pawns (white captures)
extern int en_pessant_squares_b; // En pessant square for black pawns (black captures)

// A position is a struct containing 14 bitboards
// 1 bitboard = 1 piece type
// 7 bitboards = 1 team
struct position
{
	// White pieces
	bitboard pawn_w;
	bitboard knight_w;
	bitboard bishop_w;
	bitboard rook_w;
	bitboard queen_w;
	bitboard king_w;

	// Black pieces
	bitboard pawn_b;
	bitboard knight_b;
	bitboard bishop_b;
	bitboard rook_b;
	bitboard queen_b;
	bitboard king_b;
};

bool is_empty(struct position pos, uint8_t square);
bool piece_color_at(struct position pos, uint8_t square_location, uint8_t piece_color);
int get_piece_type(struct position pos, uint8_t square_location);
void remove_piece(struct position& pos, int location);
void move_piece(struct position& pos, int location, int new_location);
bitboard map_bitboard(std::vector<int> vec);
bitboard get_control_map(struct position pos, bool piece_color);
bitboard map_pieces(struct position pos);
int count_bits(bitboard bb);
int count_pieces(struct position pos);
std::vector<int> get_moves(struct position pos, uint8_t piece_location, uint8_t piece_type, bool piece_color, bool control);
void setup_position(struct position& pos);
struct position empty_position();
int find_piece(bitboard bb);

namespace debug
{
void print_vec(std::vector<int> vec);
void print_board(struct position pos);
} // namespace debug
} // namespace board
} // namespace NerdChess
