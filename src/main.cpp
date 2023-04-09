/*
NerdChess is my first attempt at making a working Chess Engine, I started
working on it when I was 15, this is currently a WIP (work in progress), but
I am regularly adding new features. For more info, go to README.md
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
    NerdChess::bitb::set_bit(test_pos.knight_w, 1, 27);
    NerdChess::debug::print_board(test_pos);
    NerdChess::debug::print_vec(NerdChess::board::get_moves(test_pos, 27, KNIGHT_W));

    return 0;
}
