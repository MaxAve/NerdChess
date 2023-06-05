#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "eval.h"

namespace NerdChess
{
namespace engine
{
struct engine_eval
{
    int eval; // Position evaluation
    int best_move[2]; // Contains the numbers of the squares from and to which a piece needs to move to
};

struct engine_eval minimax(struct board::position pos, bool maximizing, int alpha, int beta, uint8_t depth);
} // namespace engine
} // namespace NerdChess
