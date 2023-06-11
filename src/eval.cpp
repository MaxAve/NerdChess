#include <iostream>
#include <math.h>
#include "headers/eval.h"

namespace NerdChess
{
int board_control_value_map_w[64] = {0};
int board_control_value_map_b[64] = {0};

int square_safety_map_w[64] = {0};
int square_safety_map_b[64] = {0};

bitboard board_color_map = 0ULL;

bool is_endgame = false;

void generate_board_control_value_map(int* buf, bool piece_color)
{
    if(!piece_color)
        // For white pieces
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
                buf[i*8+j] = (int)pow(sqrt(8-i)*2 + (5 - sqrt(pow(j - 3.5, 2) + pow(i - 3, 2))), 2);
    else
        // For black pieces
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
                buf[63 - (i*8+j)] = (int)pow(sqrt(8-i)*2 + (5 - sqrt(pow(j - 3.5, 2) + pow(i - 3, 2))), 2);
}

void generate_square_safety_map(int* buf, bool piece_color)
{
    if(piece_color)
    {
        // For white pieces
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
                buf[i*8+j] = (int)pow(sqrt(8-i)*2 + (sqrt(pow(j - 3.5, 2) + pow(i - 3, 2))), 2);
    }
    else
    {
        // For black pieces
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
                buf[63 - (i*8+j)] = (int)pow(sqrt(8-i)*2 + (sqrt(pow(j - 3.5, 2) + pow(i - 3, 2))), 2);
    }
}

bitboard generate_board_color_map()
{
    bitboard map = 0b1111111111111111111111111111111111111111111111111111111111111111ULL; // ._.
    for(int i = 0; i < 8; ++i)
        for(int j = 0; j < 8; ++j)
            if((i+1*8+j) % 2 == 0)
                clear_bit(map, i*8+j);
    return map;
}

namespace eval
{
int eval_material(struct board::position pos)
{
    int evaluation = 0;
    bitboard piece_map = board::map_pieces(pos);
    const int piece_values[] = {PAWN_VALUE, KNIGHT_VALUE, BISHOP_VALUE, ROOK_VALUE, QUEEN_VALUE, 1000000000, -PAWN_VALUE, -KNIGHT_VALUE, -BISHOP_VALUE, -ROOK_VALUE, -QUEEN_VALUE, -1000000000};

    for(int i = 0; i < 64; ++i)
    {
        if(bitb::get_bit(piece_map, i))
        {
            evaluation += piece_values[board::get_full_piece_type(pos, i)];
        }
    }
    return evaluation;
}
/*int eval_material(struct board::position pos, bool piece_color)
{
    int evaluation = 0;

    // Maps pieces on a bitboard
    // Very useful when you only need to know whether there is a piece located on a square, and can ignore its color and type
    bitboard piece_map = board::map_pieces(pos);

    for(int i = 0; i < 64; ++i)
    {
        if(bitb::get_bit(piece_map, i))
        {
            if(board::piece_color_at(pos, i, piece_color))
            {
                switch(board::get_piece_type(pos, i))
                {
                    // Pawn
                    case PAWN:
                        evaluation += PAWN_VALUE;
                        break;

                    // Knight
                    case KNIGHT:
                        evaluation += KNIGHT_VALUE;
                        break;

                    // Bishop
                    case BISHOP:
                        evaluation += BISHOP_VALUE;
                        break;

                    // Rook
                    case ROOK:
                        evaluation += ROOK_VALUE;
                        break;

                    // Queen
                    case QUEEN:
                        evaluation += QUEEN_VALUE;
                        break;

                    // King
                    case KING:
                        evaluation += 1000000000;
                        break;
                }
            }
        }
    }
    return evaluation;
}*/

// Evaluation functions for the middlegame phase
namespace middlegame
{
int eval_board_control(struct board::position pos, bool piece_color)
{
    int eval = 0;
    int* value_map;
    if(piece_color)
        value_map = board_control_value_map_b;
    else
        value_map = board_control_value_map_w;
    bitb::bitboard control_map = board::get_control_map(pos, piece_color);
    for(int i = 0; i < 64; ++i)
        if(get_bit(control_map, i) == 1)
            eval += value_map[i] / 5; // Increases the evaluation based on the value of the squares controlled
    return eval;
}
} // namespace middlegame

int eval_position(struct board::position pos)
{
    int evaluation = 0;

    // Material (includes piece activity and pawn structure)
    evaluation += eval_material(pos);

	// Control
    evaluation += middlegame::eval_board_control(pos, WHITE);
    evaluation -= middlegame::eval_board_control(pos, BLACK);
	
    return evaluation;
}
} // namespace eval
} // namespace NerdChess
