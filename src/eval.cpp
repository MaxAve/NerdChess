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
#include <math.h>
#include "eval.h"

namespace NerdChess
{
int board_control_value_map_w[64] = {0};
int board_control_value_map_b[64] = {0};

int square_safety_map_w[64] = {0};
int square_safety_map_b[64] = {0};

bitboard board_color_map = 0ULL;

bool is_endgame = false;

/**
 * @brief Generates a map on which each square has a specific value. This can be used to determine whether a piece controls useful squares. "Good squares" include squares on the enemy side and center squares.
 * 
 * @param buf Buffer to save to
 * @param piece_color Which pieces should use the map
 */
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

/**
 * @brief Generates a square safety map to the provided buffer. Safe squares include squares closer to the corners
 * 
 * @param buf Buffer to save to
 * @param piece_color Which pieces should use the map
 */
void generate_square_safety_map(int* buf, bool piece_color)
{
    if(piece_color)
        // For white pieces
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
                buf[i*8+j] = (int)pow(sqrt(8-i)*2 + (sqrt(pow(j - 3.5, 2) + pow(i - 3, 2))), 2);
    else
        // For black pieces
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
                buf[63 - (i*8+j)] = (int)pow(sqrt(8-i)*2 + (sqrt(pow(j - 3.5, 2) + pow(i - 3, 2))), 2);
}

/**
 * @brief Generates a map with each bit corresponding to a square's color on a chessboard. If a bit is set to 0 then it is white, if 1 then black.
 * 
 * @param buf 
 */
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
/**
 * @brief Evaluates material
 * 
 * @param pos 
 * @param piece_color 
 * @return int 
 */
int eval_material(struct board::position pos, bool piece_color)
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
                    {
                        evaluation += PAWN_VALUE;
                        // Distance to promotion square
                        if(!piece_color) // White
                        {
                            bool passed = true;
                            int sq = i - (i % 8);
                            for(int j = i; j >= 0; j-=8)
                                if(bitb::get_bit(piece_map, j))
                                    passed = false;
                            if(passed)
                                evaluation += (8 - (sq / 8)) * 6;
                        }
                        else // Black
                        {
                            bool passed = true;
                            int sq = i - (i % 8);
                            for(int j = i; j < 64; j+=8)
                                if(bitb::get_bit(piece_map, j))
                                    passed = false;
                            if(passed)
                                evaluation += ((sq / 8)) * 6;
                        }
                        break;
                    }

                    // Knight
                    case KNIGHT:
                    {
                        evaluation += KNIGHT_VALUE;
                        break;
                    }

                    // Bishop
                    case BISHOP:
                    {
                        evaluation += BISHOP_VALUE;
                        break;
                    }

                    // Rook
                    case ROOK:
                    {
                        evaluation += ROOK_VALUE;
                        break;
                    }

                    // Queen
                    case QUEEN:
                    {
                        evaluation += QUEEN_VALUE;
                        break;
                    }

                    // King
                    case KING:
                    {
                        evaluation += 1000000000;
                        break;
                    }
                }
            }
        }
    }
    return evaluation;
}

// Evaluation functions for the middlegame phase
namespace middlegame
{
/**
 * @brief Returns the evaluation of the king safety for a team
 * 
 * @return King safety level as float
 */
int eval_king_safety(struct board::position pos, bool piece_color)
{
    int evaluation = 0;

    // Distance to center
    int king_location = board::find_piece(piece_color ? pos.king_b : pos.king_w);

    // Maps pieces on a bitboard
    bitboard piece_map = board::map_pieces(pos);

    // If the king is missing return the lowest possible number
    if(king_location == -1)
        return -(INT_MAX - 100000); // Retuns the lowest int value - 100,000. We substract 100,000 to ensure there are no overflows
        
    evaluation += piece_color ? square_safety_map_b[king_location] : square_safety_map_w[king_location];

    // Pawns in front of king?
    int pawn_protection = 0;
    if(!piece_color)
    {
        // White king
        for(int i = 0; i < 3; ++i)
        {
            if(king_location % 8 == 0 && i == 0)
                continue;
            if(king_location+1 % 8 == 0 && i == 2)
                continue;
            if(bitb::get_bit(pos.pawn_w, king_location - 9 + i) == 1)
                pawn_protection += 100;
        }
        for(int i = 0; i < 3; ++i)
        {
            if(king_location % 8 == 0 && i == 0)
                continue;
            if(king_location+1 % 8 == 0 && i == 2)
                continue;
            if(bitb::get_bit(pos.pawn_w, king_location - 17 + i) == 1)
                pawn_protection += 90; // Pawns 1 square away from king
        }
    }
    else
    {
        // Black king
        for(int i = 0; i < 3; ++i)
        {
            if(king_location % 8 == 0 && i == 0)
                continue;
            if(king_location+1 % 8 == 0 && i == 2)
                continue;
            if(bitb::get_bit(pos.pawn_b, king_location + 7 + i) == 1)
                pawn_protection += 100;
        }
        for(int i = 0; i < 3; ++i)
        {
            if(king_location % 8 == 0 && i == 0)
                continue;
            if(king_location+1 % 8 == 0 && i == 2)
                continue;
            if(bitb::get_bit(pos.pawn_b, king_location + 15 + i) == 1)
                pawn_protection += 90; // Pawns 1 square away from king
        }
    }

    // Enemy pieces aiming at/near king?
    bitb::bitboard control_map = board::get_control_map(pos, !piece_color);
    int attack_magnitude = 0; // Number of squares around the king controlled by enemy pieces. E.g: a surrounded king will get an attack magnitude of 8
    for(int i = -1; i < 2; ++i)
    {
        for(int j = -1; j < 2; ++j)
        {
            int xy = king_location + ((i * 8) + j - 1);
            if(xy >= 0 && xy <= 63)
            {
                if((king_location % 8 == 0 && j == 0) || ((king_location+1)%8 == 0 && j == 2))
                    continue;
                if(get_bit(control_map, xy) == 1)
                    attack_magnitude++;
            }
        }
    }
    evaluation -= (SQUARE(attack_magnitude) * 20 - pawn_protection);
    
    return evaluation;
}

/**
 * @brief Returns an evaluation based on the squares which the pieces of a single color control. Quantity of squares as well as their importance is evaluated. Only works when board_control_value_map_w and board_control_value_map_b have been initialized.
 * 
 * @param pos 
 * @param piece_color 
 * @return float 
 */
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

/**
 * @brief Produces an overall evaluation of a position
 * 
 * @param pos 
 * @return evaluation
 */
int eval_position(struct board::position pos)
{
    int evaluation = 0;

    // King safety
    //evaluation += middlegame::eval_king_safety(pos, WHITE);
    //evaluation -= middlegame::eval_king_safety(pos, BLACK);

    // Material (includes piece activity and pawn structure)
    evaluation += eval_material(pos, WHITE);
    evaluation -= eval_material(pos, BLACK);

	// Control
    evaluation += middlegame::eval_board_control(pos, WHITE);
    evaluation -= middlegame::eval_board_control(pos, BLACK);
	
    return evaluation;
}
} // namespace eval
} // namespace NerdChess