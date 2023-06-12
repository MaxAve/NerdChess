#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <vector>
#include "bitboard.h"

using namespace NerdChess::bitb;

#define EMPTY -2147483648
#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5
#define _BLACK 6 // In the position struct black pieces are represented by the last 6 bitboards, adding 6 to a piece index gives you the black version of the piece

#define WHITE 0
#define BLACK 1

namespace NerdChess
{
namespace board
{
struct position
{
	bitb::bitboard pieces[12];	
	bool castling_rights[2];
	int en_pessant_squares[2];
};

bool is_empty(struct position board, uint8_t square_location);
bool piece_color_at(struct position board, uint8_t square_location, uint8_t piece_color);
int get_piece_type(struct position board, uint8_t square_location);
int get_full_piece_type(struct position board, uint8_t square_location);
void remove_piece(struct position& board, int square_location);
void move_piece(struct position& board, int from, int to);
bitb::bitboard map_bitboard(std::vector<int> vec);
bitb::bitboard get_control_map(struct position board, bool piece_color);
bitb::bitboard map_pieces(struct position board);
int count_bits(bitb::bitboard bb);
int count_pieces(struct position board);
std::vector<int> get_moves(struct position pos, uint8_t piece_location, uint8_t piece_type, bool piece_color, bool control);
void setup_position(struct position& board);
int find_piece(bitb::bitboard bb);
inline struct position get_empty_position() { return (struct position){{0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL, 0ULL}, {true, true}, {INT_MIN, INT_MIN}}; }
void print_board(struct position board, int sp, int ss);
std::string board_to_str(NerdChess::board::position board);

namespace debug
{
void print_vec(std::vector<int> vec);
void print_board(struct position board);
} // namespace debug
} // namespace board
} // namespace NerdChess

#endif
