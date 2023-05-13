/*
The middlegame.* files contain all necessary functions for evaluating
positions in the middlegame.
*/

#include <iostream>
#include <math.h>
#include <limits>
#include "middlegame.h"

namespace NerdChess
{
int board_control_value_map_w[64] = {0};
int board_control_value_map_b[64] = {0};
namespace middlegame
{
/**
 * @brief Generates a map on which each square has a specific value. This can be used to determine whether a piece controls useful squares. "Good squares" include squares on the enemy side and center squares.
 * 
 * @param piece_color 
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
 * @brief Checks the entire board and assings a value for each piece based on their usefulness and overall strength
 * 
 * @param pos 
 * @param piece_color 
 * @return float 
 */
float eval_material(struct board::position pos, bool piece_color)
{
    return 0;
}

/**
 * @brief Returns the evaluation of the king safety for a team
 * 
 * @return King safety level as float
 */
float eval_king_safety(struct board::position pos, bool piece_color)
{
    float evaluation = 0;

    // Distance to center
    int king_location = board::find_piece(piece_color ? pos.king_b : pos.king_w);

    // If the king is missing return the lowest possible number
    if(king_location == -1)
        return -std::numeric_limits<float>::infinity();

    int xpos = king_location % 8;
    int ypos = (king_location - xpos + 1) / 8;
    float distance_to_center_x = xpos - 3.5;
    float distance_to_center_y = ypos - 3.5;
    float distance_to_center = sqrt(pow(distance_to_center_x, 2) + pow(distance_to_center_y, 2));
    evaluation += (((distance_to_center - 4) * abs(distance_to_center - 4))/2);

    // Pawns in front of king?
    float pawn_protection = 0;
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
                pawn_protection++;
        }
        for(int i = 0; i < 3; ++i)
        {
            if(king_location % 8 == 0 && i == 0)
                continue;
            if(king_location+1 % 8 == 0 && i == 2)
                continue;
            if(bitb::get_bit(pos.pawn_w, king_location - 17 + i) == 1)
                pawn_protection += 0.7; // Panws which are 1 square away from the king provide reduced protection
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
                pawn_protection++;
        }
        for(int i = 0; i < 3; ++i)
        {
            if(king_location % 8 == 0 && i == 0)
                continue;
            if(king_location+1 % 8 == 0 && i == 2)
                continue;
            if(bitb::get_bit(pos.pawn_b, king_location + 15 + i) == 1)
                pawn_protection += 0.7; // Panws which are 1 square away from the king provide reduced protection
        }
    }
    // This weird formula makes it so that there is not too big of a difference between having 2 or 3 pawns in front of the king,
    // but having 1 pawn vs 3 will make a huge impact on the eval
    if(pawn_protection >= 1)
        evaluation += sqrt(sqrt(sqrt(sqrt(pawn_protection-1)))) * 5;
    else
        evaluation -= 5;

    // Enemy pieces aiming at/near king?
    bitb::bitboard control_map = board::get_control_map(pos, !piece_color);
    int attack_magnitude = 0; // Number of squares around the king controlled by enemy pieces. E.g: a surrounded king will return an attack magnitude of 8
    for(int i = -9; i < -6; ++i)
    {
        if(king_location % 8 == 0 && i == -9)
            continue;
        if(king_location+1 % 8 == 0 && i == -7)
            continue;
        if(king_location + i < 64 && king_location + i > -1)
            if(bitb::get_bit(control_map, king_location + i) == 1)
                attack_magnitude += 1;
    }
    for(int i = -1; i < 2; ++i)
    {
        if(king_location % 8 == 0 && i == -1)
            continue;
        if(king_location+1 % 8 == 0 && i == 1)
            continue;
        if(king_location + i < 64 && king_location + i > -1)
            if(bitb::get_bit(control_map, king_location + i) == 1)
                attack_magnitude += 1;
    }
    for(int i = 7; i < 10; ++i)
    {
        if(king_location % 8 == 0 && i == 7)
            continue;
        if(king_location+1 % 8 == 0 && i == 9)
            continue;
        if(king_location + i < 64 && king_location + i > -1)
            if(bitb::get_bit(control_map, king_location + i) == 1)
                attack_magnitude += 1;
    }
    evaluation -= (attack_magnitude * 0.8);
    
    return evaluation;
}

/**
 * @brief Returns an evaluation based on the squares which the pieces of a single color control. Quantity of squares as well as their importance is evaluated. Only works when board_control_value_map_w and board_control_value_map_b have been initialized.
 * 
 * @param pos 
 * @param piece_color 
 * @return float 
 */
float eval_board_control(struct board::position pos, bool piece_color)
{
    float eval = 0;
    int* value_map;
    if(piece_color)
        value_map = board_control_value_map_b;
    else
        value_map = board_control_value_map_w;
    bitb::bitboard control_map = board::get_control_map(pos, piece_color);
    for(int i = 0; i < 64; ++i)
        if(get_bit(control_map, i) == 1)
            eval += (float)(value_map[i]) / 100;
    std::cout << eval << "\n";
    return eval;
}
} // namespace middlegame
} // namespace NerdChess
