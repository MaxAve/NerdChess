#include <iostream>
#include <thread>
#include <vector>
#include "engine.h"

namespace NerdChess
{
namespace engine
{
/**
 * @brief Minimax function
 * 
 * @param maximizing 
 * @param depth 
 * @return struct engine_eval 
 */
struct engine_eval minimax(struct board::position pos, bool maximizing, int alpha, int beta, uint8_t depth)
{
    int evaluation = 0;
    int best_move[2] = {8, 16};

    if(maximizing)
        evaluation = -INT_MAX;
    else
        evaluation = INT_MAX;

    if(depth == 0)
    {
        struct engine_eval ee;
        ee.eval = eval::eval_position(pos);
        ee.best_move[0] = best_move[0];
        ee.best_move[1] = best_move[1];
        return ee;
    }
    else
    {
        for(int i = 0; i < 64; ++i)
        {
            if(board::piece_color_at(pos, i, !maximizing)) // I f*cked up with the values, so !maximizing == WHITE which makes no sense, sorry
            {
                // List of moves for the current piece
                std::vector<int> moves = board::get_moves(pos, i, board::get_piece_type(pos, i), !maximizing, false);

                for(int j = 0; j < moves.size(); ++j)
                {
                    if(maximizing)
                    {
                        // Attempt each move and call minimax on the hypothetical boards
                        struct board::position hypothetical_board = pos;
                        board::move_piece(hypothetical_board, i, moves[j]);

                        struct engine_eval hypothetical_eval = minimax(hypothetical_board, !maximizing, alpha, beta, depth - 1);

                        if(hypothetical_eval.eval > evaluation)
                        {
                            evaluation = hypothetical_eval.eval;
                            best_move[0] = i;
                            best_move[1] = moves[j];
                        }

                        alpha = std::max(alpha, evaluation);

                        if(alpha >= beta)
                            break;
                    }
                    else
                    {
                        // Attempt each move and call minimax on the hypothetical boards
                        struct board::position hypothetical_board = pos;
                        board::move_piece(hypothetical_board, i, moves[j]);

                        struct engine_eval hypothetical_eval = minimax(hypothetical_board, !maximizing, alpha, beta, depth - 1);

                        if(hypothetical_eval.eval < evaluation)
                        {
                            evaluation = hypothetical_eval.eval;
                            best_move[0] = i;
                            best_move[1] = moves[j];
                        }

                        beta = std::min(beta, evaluation);
                        
                        if(alpha >= beta)
                            break;
                    }
                }
            }
        }
        struct engine_eval ee;
        ee.eval = evaluation;
        ee.best_move[0] = best_move[0];
        ee.best_move[1] = best_move[1];
        return ee;
    }
}
} // namespace engine
} // namespace NerdChess
