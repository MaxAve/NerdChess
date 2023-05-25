/*
eval.* files contain all the necessary functions to evaluate positions at
different stages of the game.

Things to note:
Evaluation is always x100 larger than it actually should be. This is because
the last 2 digits can be viewed as "fractions". Working with floating-point numbers
proved to be error-prone.
Example: in chess a queen is worth 9 points, and here it is 900
*/

#include <iostream>
#include "position.h"

// Some quick math
#define SQUARE(x)((x)*(x)) // x^2

// Piece values (material points * 100)
#define PAWN_VALUE 100
#define KNIGHT_VALUE 300
#define BISHOP_VALUE 310
#define ROOK_VALUE 500
#define QUEEN_VALUE 900

namespace NerdChess
{
// Maps to determine which squares are more important to control for each team
extern int board_control_value_map_w[64];
extern int board_control_value_map_b[64];

// Maps to determine which squares are safest for the kings
extern int square_safety_map_w[64];
extern int square_safety_map_b[64];

// Map for square colors (used for evaluating bishops)
extern bitboard board_color_map;

extern bool is_endgame;

void generate_board_control_value_map(int* buf, bool piece_color);
void generate_square_safety_map(int* buf, bool piece_color);
bitboard generate_board_color_map();

namespace eval
{
int eval_material(struct board::position pos, bool piece_color);
namespace middlegame
{
int eval_king_safety(struct board::position pos, bool piece_color);
int eval_board_control(struct board::position pos, bool piece_color);
} // namespace middlegame
int eval_position(struct board::position pos);
} // namespace eval
} // namespace NerdChess
