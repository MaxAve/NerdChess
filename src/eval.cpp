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
                buf[i*8+j] = (int)pow(sqrt(8-i)*2 + (sqrt(pow(j - 3.5, 2) + pow(i - 3, 2))), 2) - 300;
    else
        // For black pieces
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
                buf[63 - (i*8+j)] = (int)pow(sqrt(8-i)*2 + (sqrt(pow(j - 3.5, 2) + pow(i - 3, 2))), 2) - 300;
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
 * @brief Checks the entire board and assings a value for each piece found based on their usefulness and overall strength. This function works equally well for both middle- and endgame phases. The final result is the sum of each piece's value.
 * 
 * @param pos position to analyse
 * @param piece_color color for which to check
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
        if(get_bit(piece_map, i))
        {
            if(board::piece_color_at(pos, i, piece_color))
            {
                switch(board::get_piece_type(pos, i))
                {
                    // Pawn
                    // Pawns are only worth 100 points (1), they gain value when they are passed pawns and lose
                    // value if they are doubled or tripled.
                    case PAWN:
                    {
                        evaluation += PAWN_VALUE; // Pawns are worth 100 points

                        // Black and white pawns behave differently, which is why we need seperate functions for them
                        if(!piece_color)
                        {
                            // White
                            if(bitb::get_bit(pos.pawn_w, i-8)) // Doubled pawns
                            {
                                evaluation -= 35; // Doubled pawns are bad...
                                if(i >= 16 && bitb::get_bit(pos.pawn_w, i-16))
                                    evaluation -= 100; // ...but tripled pawns are even worse.
                            }
                            else
                            {
                                // Check if there aren't any pieces in front of this one
                                // If there are none, then increase this pawn's value based on the number of squares
                                // left to promotion
                                bool passed = true;
                                uint8_t squares_to_promotion = 0;
                                for(int j = i - 8; j > -1; j -= 8)
                                {
                                    squares_to_promotion++;
                                    if(get_bit(piece_map, j))
                                    {
                                        passed = false;
                                        break;
                                    }
                                }
                                if(passed)
                                    evaluation += (6 - squares_to_promotion)*15; // Increase the pawn's value if it is an unobstructed passed pawn
                            }
                            // Pawn chains are pretty cool I guess
                            if(i % 8 != 0)
                                if(get_bit(pos.pawn_w, i + 7))
                                    evaluation += 20;
                            if((i+1) % 8 != 0)
                                if(get_bit(pos.pawn_w, i + 9))
                                    evaluation += 20;
                        }
                        else
                        {
                            // Black
                            if(bitb::get_bit(pos.pawn_b, i+8)) // Doubled pawns
                            {
                                evaluation -= 35; // Doubled pawns are bad...
                                if(i <= 47 && bitb::get_bit(pos.pawn_b, i+16))
                                    evaluation -= 100; // ...but tripled pawns are even worse.
                            }
                            else
                            {
                                // Check if there aren't any pieces in front of this one
                                // If there are none, then increase this pawn's value based on the number of squares
                                // left to promotion
                                int s = i+8;
                                bool passed = true;
                                int squares_to_promotion = 0;
                                while(s > 7)
                                {
                                    if(get_bit(piece_map, s))
                                    {
                                        passed = false;
                                        break;
                                    }
                                    s += 8;
                                    squares_to_promotion++;
                                }
                                if(passed)
                                    evaluation += (7 - squares_to_promotion)*15; // Increase the pawn's value if it is an unobstructed passed pawn
                            }
                            // Yeah, pawn chains are definately cool
                            if((i+1) % 8 != 0)
                                if(get_bit(pos.pawn_b, i - 7))
                                    evaluation += 20;
                            if(i % 8 != 0)
                                if(get_bit(pos.pawn_b, i - 9))
                                    evaluation += 20;
                        }
                        break;
                    }

                    // Knight
                    // Knights are better in closed positions.
                    case KNIGHT:
                    {
                        // Knights are better in closed positions, where more pieces are still present
                        // Knights will not be worth as much in open positions
                        evaluation += KNIGHT_VALUE + cbrt(board::count_pieces(pos)-20)*12;
                        break;
                    }

                    // Bishop
                    // Bishops are pieces which, similar to rooks, are more valuable in the endgame since they can control more
                    // squares. The engine should be more willing to trade a bishop for a knight in closed positions.
                    case BISHOP:
                    {
                        // Generates a bitboard of all pawns present on squares of this bishops's color
                        // This is needed because, for example, a light-squared bishop is not of much use in a position
                        // where most of your, and/or the opponent's pawns are put on light squares
                        bitboard barricade_map = (pos.pawn_w | pos.pawn_b) & (get_bit(board_color_map, i) ? ~board_color_map : board_color_map);
                        int blocking_pawns = board::count_bits(barricade_map);

                        // Calculates evaluation based on number of pieces present, and pawns which have potential to block the bishop
                        evaluation += ((BISHOP_VALUE - cbrt(board::count_pieces(pos)-30)*16) - (pow(blocking_pawns, 1.2)));

                        break;
                    }

                    // Rook
                    // Rooks are endgame pieces, they are much more useful and therefore valuable the less pieces
                    // are present. Another important factor is file control.
                    case ROOK:
                    {
                        // (Semi-)file control
                        // Controlling files is one of the most important things for a rook
                        // A rook gains a minimal score boost if it controls a file
                        std::vector<int> rook_moves = board::get_moves(pos, i, ROOK, piece_color, true); // Get all moves
                        uint8_t squares_controlled = 0;
                        for(int j = 0; j < rook_moves.size(); ++j)
                            if((i - rook_moves[j]) % 8 == 0)
                                squares_controlled++;

                        // Calculate evaluation based on number of pieces present, and file control of this rook
                        evaluation += ((ROOK_VALUE - cbrt(board::count_pieces(pos)-10)*20) + (SQUARE(squares_controlled)));
                        break;
                    }

                    // Queen
                    // Queens are important attacking pieces and can completely annihilate an entire position
                    // when given the opportunity. Queens are more valuable the less pieces are present.
                    case QUEEN:
                    {
                        evaluation += QUEEN_VALUE - cbrt(board::count_pieces(pos)-30)*25;
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
                pawn_protection += 65; // Panws which are 1 square away from the king provide reduced protection
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
                pawn_protection += 65; // Panws which are 1 square away from the king provide reduced protection
        }
    }
    // This weird formula makes it so that there is not too big of a difference between having 2 or 3 pawns in front of the king,
    // but having 1 pawn vs 3 will make a huge impact on the eval
    evaluation += (int)(sqrt(pawn_protection)*5);

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
    evaluation -= (SQUARE(attack_magnitude) * 30 - pawn_protection);
    
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
            eval += value_map[i] / 8; // Increases the evaluation based on the value of the squares controlled
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
    evaluation += middlegame::eval_king_safety(pos, WHITE);
    evaluation -= middlegame::eval_king_safety(pos, BLACK);

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
