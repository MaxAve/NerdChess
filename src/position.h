/*
Here, position refers to a combination of (14) bitboards which define a specific
chess piece layout. This file contains all necessary function for working with
a position.
*/

#include <iostream>
#include <vector>
#include "bitboard.h"

using namespace NerdChess::bitb;

// Piece types
// Used by the get_moves() function
#define PAWN_W 0
#define KNIGHT_W 1
#define BISHOP_W 2
#define ROOK_W 3
#define QUEEN_W 4
#define KING_W 5

#define PAWN_B 6
#define KNIGHT_B 7
#define BISHOP_B 8
#define ROOK_B 9
#define QUEEN_B 10
#define KING_B 11

// Piece colors
#define WHITE 0
#define BLACK 1

namespace NerdChess
{
namespace board
{
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
std::vector<int> get_moves(struct position pos, uint8_t piece_location, uint8_t piece_type);
void setup_position(struct position& pos);
} // namespace board
} // namespace NerdChess
