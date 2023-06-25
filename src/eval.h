#ifndef EVAL_H
#define EVAL_H

#include <iostream>
#include "position.h"

#define PAWN_VALUE 100
#define KNIGHT_VALUE 300
#define BISHOP_VALUE 350
#define ROOK_VALUE 600
#define QUEEN_VALUE 950

#define WINNER_WHITE 1
#define WINNER_BLACK -1
#define WINNER_NONE 0

namespace NerdChess {
// Maps to determine which squares are more important to control for each team
extern int board_control_value_map_w[64];
extern int board_control_value_map_b[64];

void generate_board_control_value_map(int* buf, bool piece_color);
namespace eval {
int get_winner(struct board::position pos);
int eval_material(struct board::position pos, int piece_map[]);
int eval_structure(struct NerdChess::board::position board, int piece_map[]);
namespace middlegame {
int eval_board_control(struct board::position pos, bool piece_color);
} // namespace middlegame
int eval_position(struct board::position pos);
} // namespace eval
} // namespace NerdChess

#endif
