/*
NerdChess is my first attempt at making a working Chess Engine, I started working on it somewhere in
the spring in 2023 when I was 15.

This is currently a WIP (work in progress), but I am regularly adding new features. For more info,
go to README.md

Please note: when school starts again, I will not be able to update this as regularly as I am now.
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
	NerdChess::bitb::set_bit(test_pos.king_b, 1, 37);
	//NerdChess::bitb::print_bitboard(NerdChess::board::get_control_map(test_pos, BLACK));
	NerdChess::debug::print_board(test_pos);
	NerdChess::debug::print_vec(NerdChess::board::get_moves(test_pos, 37, KING, BLACK, false));

	return 0;
}
