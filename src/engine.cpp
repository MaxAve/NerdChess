#include <iostream>
#include <thread>
#include <vector>
#include "engine.h"

struct NerdChess::engine::engine_eval NerdChess::engine::minimax(struct board::position pos, bool maximizing, int alpha, int beta, uint8_t depth) {
    int evaluation = maximizing ? -INT_MAX : INT_MAX;
    struct NerdChess::engine::engine_eval eval;
    const int winner = NerdChess::eval::get_winner(pos);

    if(winner != WINNER_NONE) {
        eval.eval = winner * INT_MAX;
        return eval;
    } else if(depth == 0) {
        eval.eval = eval::eval_position(pos);
        return eval;
    } else {
        for(int i = 0; i < 64; ++i) {
            if(board::piece_color_at(pos, i, !maximizing)) { // I f*cked up with the values, so (WHITE == false), sorry
                std::vector<int> moves = board::get_moves(pos, i, board::get_piece_type(pos, i), !maximizing, false);

                for(int j = 0; j < moves.size(); ++j) {
                    if(maximizing) {
                        // Attempt each move and call minimax on the hypothetical boards
                        struct board::position hypothetical_board = pos;
                        board::move_piece(hypothetical_board, i, moves[j]);

                        struct NerdChess::engine::engine_eval hypothetical_eval = minimax(hypothetical_board, !maximizing, alpha, beta, depth - 1);

                        if(hypothetical_eval.eval > evaluation) {
                            evaluation = hypothetical_eval.eval;
                            eval.best_move[0] = i;
                            eval.best_move[1] = moves[j];
                        }

                        alpha = std::max(alpha, evaluation);
                        if(alpha >= beta)
                            break;
                    } else {
                        // Attempt each move and call minimax on the hypothetical boards
                        struct board::position hypothetical_board = pos;
                        board::move_piece(hypothetical_board, i, moves[j]);

                        struct NerdChess::engine::engine_eval hypothetical_eval = minimax(hypothetical_board, !maximizing, alpha, beta, depth - 1);

                        if(hypothetical_eval.eval < evaluation) {
                            evaluation = hypothetical_eval.eval;
                            eval.best_move[0] = i;
                            eval.best_move[1] = moves[j];
                        }

                        beta = std::min(beta, evaluation);
                        if(alpha >= beta)
                            break;
                    }
                }
            }
        }

        eval.eval = evaluation;
        return eval;
    }
}
