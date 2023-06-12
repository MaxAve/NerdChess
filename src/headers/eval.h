#ifndef EVAL_H
#define EVAL_H

#include <iostream>
#include "position.h"

#define SQUARE(x)((x)*(x))
#define IS_NEAR_CENTER(x)(x > 15 && x < 48 && (x % 8) > 1 && (x % 8) < 6)
#define IS_IN_CENTER(x)(x > 23 && x < 40 && (x % 8) > 2 && (x % 8) < 5)

#define PAWN_VALUE 100
#define KNIGHT_VALUE 300
#define BISHOP_VALUE 300
#define ROOK_VALUE 500
#define QUEEN_VALUE 900

namespace NerdChess {
// Maps to determine which squares are more important to control for each team
extern int board_control_value_map_w[64];
extern int board_control_value_map_b[64];
// Maps to determine which squares are safest for the kings
extern int square_safety_map_w[64];
extern int square_safety_map_b[64];

// Map for square colors on a board
extern bitb::bitboard board_color_map;

void generate_board_control_value_map(int* buf, bool piece_color);
void generate_square_safety_map(int* buf, bool piece_color);
bitb::bitboard generate_board_color_map();
namespace eval {
int eval_material(struct board::position pos, int piece_map[]);
int eval_structure(struct NerdChess::board::position board, int piece_map[]);
namespace middlegame {
int eval_board_control(struct board::position pos, bool piece_color);
} // namespace middlegame
int eval_position(struct board::position pos);
} // namespace eval
} // namespace NerdChess

#endif
