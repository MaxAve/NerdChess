/*
Contains functions responsible for generating output for the user,
such as UI, debug, etc.
*/

#include <iostream>
#include "position.h"

using namespace NerdChess::board;

namespace NerdChess
{
namespace debug
{
// Prints out a vector with a JavaScript-like format.
// Example output:
// [1, 143, 92]
void print_vec(std::vector<int> vec)
{
    std::cout << "[";
    for(int i = 0; i < vec.size()-1; ++i)
        std::cout << vec[i] << ", ";
    std::cout << vec[vec.size()-1] << "]";
}

// Prints out the entire collection of bitboards as one board
void print_board(struct position pos)
{
    std::cout << "\033[35m+---+---+---+---+---+---+---+---+\n";
    for(int i = 0; i < 8; ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
            std::cout << "\033[35m| ";
            if(bitb::get_bit(pos.pawn_w, j + i*8) == 1)
                std::cout << "\033[97mp ";
            else if(bitb::get_bit(pos.pawn_b, j + i*8) == 1)
                std::cout << "\033[90mp ";
            else if(bitb::get_bit(pos.knight_w, j + i*8) == 1)
                std::cout << "\033[97mN ";
            else if(bitb::get_bit(pos.knight_b, j + i*8) == 1)
                std::cout << "\033[90mN ";
            else if(bitb::get_bit(pos.bishop_w, j + i*8) == 1)
                std::cout << "\033[97mB ";
            else if(bitb::get_bit(pos.bishop_b, j + i*8) == 1)
                std::cout << "\033[90mB ";
            else if(bitb::get_bit(pos.rook_w, j + i*8) == 1)
                std::cout << "\033[97mR ";
            else if(bitb::get_bit(pos.rook_b, j + i*8) == 1)
                std::cout << "\033[90mR ";
            else if(bitb::get_bit(pos.queen_w, j + i*8) == 1)
                std::cout << "\033[97mQ ";
            else if(bitb::get_bit(pos.queen_b, j + i*8) == 1)
                std::cout << "\033[90mQ ";
            else if(bitb::get_bit(pos.king_w, j + i*8) == 1)
                std::cout << "\033[97mK ";
            else if(bitb::get_bit(pos.king_b, j + i*8) == 1)
                std::cout << "\033[90mK ";
            else
                std::cout << "  ";
        }
        std::cout << "\033[35m|\n\033[35m+---+---+---+---+---+---+---+---+\n";
    }
    std::cout << "\033[97m\n";
}
} // namespace debug
} // namespace NerdChess
