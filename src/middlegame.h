/*
The middlegame.* files contain all necessary functions for evaluating
positions in the middlegame.
*/

#include <iostream>
#include "position.h"

namespace NerdChess
{
// Maps to determine which squares are more important to control for each team
extern int board_control_value_map_w[64];
extern int board_control_value_map_b[64];
namespace middlegame
{
void generate_board_control_value_map(int* buf, bool piece_color);
float eval_material(struct board::position pos, bool piece_color);
float eval_king_safety(struct board::position pos, bool piece_color);
float eval_board_control(struct board::position pos, bool piece_color);
} // namespace middlegame
} // namespace NerdChess
