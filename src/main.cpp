/*
NerdChess is my first attempt at making a working Chess Engine, I started
working on it when I was 15.
*/

#include <iostream>
#include "bitboard.h"
#include "output.h"

// Main function
int main()
{
    system("cls"); // Clear output

    struct NerdChess::board::position test_pos;
    NerdChess::board::setup_position(test_pos);
    NerdChess::debug::print_board(test_pos);
    //NerdChess::bitb::set_bit(test_pos.pawn_w, 1, 17);
    //NerdChess::debug::print_vec(NerdChess::board::get_moves(test_pos, 10, PAWN_B));

    return 0;
}
