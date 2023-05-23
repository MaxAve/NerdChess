#include <iostream>
#include <algorithm>
#include "eval.h"

namespace NerdChess
{
namespace engine
{
// Used by the minimax function to determine the evaluation and best move
// best_move array contains the numbers of the squares from and to which a piece needs to move to
struct engine_eval
{
    int eval;
    int best_move[2];
};

struct engine_eval minimax(struct board::position pos, bool maximizing, int alpha, int beta, uint8_t depth);
} // namespace engine
} // namespace NerdChess
