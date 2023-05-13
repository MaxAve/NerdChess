/*
NerdChess is my first attempt at making a working Chess Engine, I started working on it somewhere in
the spring of 2023 when I was 15.

This is currently a WIP (work in progress), but I am regularly adding new features. For more info,
go to README.md

Please note: because of school I am not able to add features regularly
*/

#include <iostream>
#include "eval.h"

// Main function
int main()
{
	system("cls"); // Clear output

	// Initialization
	NerdChess::middlegame::generate_board_control_value_map(NerdChess::board_control_value_map_w, WHITE);
	NerdChess::middlegame::generate_board_control_value_map(NerdChess::board_control_value_map_b, BLACK);

	//! TESTS
	struct NerdChess::board::position test_pos;
	NerdChess::board::setup_position(test_pos);
	NerdChess::board::move_piece(test_pos, 52, 36);
	NerdChess::board::move_piece(test_pos, 12, 28);

	//NerdChess::board::remove_piece(test_pos, 5);

	NerdChess::board::debug::print_board(test_pos);
	NerdChess::eval::eval_position(test_pos);

	/*std::cout << "\n\n\n";
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			std::cout << std::to_string(NerdChess::board_control_value_map_w[i*8 + j]) << " ";
		}
		std::cout << "\n";
	}

	std::cout << "\n\n\n";
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			std::cout << std::to_string(NerdChess::board_control_value_map_b[i*8 + j]) << " ";
		}
		std::cout << "\n";
	}*/

	return 0;
}
