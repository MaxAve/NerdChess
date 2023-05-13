#include <iostream>
#include "eval.h"

namespace NerdChess
{
namespace eval
{
/**
 * @brief Produces an overall evaluation of a position
 * 
 * @param pos 
 * @return evaluation
 */
float eval_position(struct board::position pos)
{
    float evaluation = 
      middlegame::eval_board_control(pos, WHITE) 
    - middlegame::eval_board_control(pos, BLACK);
    + middlegame::eval_king_safety(pos, WHITE);
    - middlegame::eval_king_safety(pos, BLACK);
    std::cout << "Eval: " << evaluation << "\n";
    return evaluation;
}
} // namespace eval
} // namespace NerdChess
