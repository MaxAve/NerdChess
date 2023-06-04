/*
NerdChess is my first attempt at making a working Chess Engine, I started working on it somewhere in
the spring of 2023 when I was 15.

This is currently a WIP (work in progress), but I am regularly adding new features. For more info,
go to README.md

Please note: because of school I am not able to add features regularly
*/

#include <iostream>
#include <chrono>
#include "engine.h"

// Main function
int main()
{
	// Initialization
	NerdChess::generate_board_control_value_map(NerdChess::board_control_value_map_w, WHITE);
	NerdChess::generate_board_control_value_map(NerdChess::board_control_value_map_b, BLACK);
	NerdChess::generate_square_safety_map(NerdChess::square_safety_map_w, WHITE);
	NerdChess::generate_square_safety_map(NerdChess::square_safety_map_b, BLACK);
	NerdChess::board_color_map = NerdChess::generate_board_color_map();

	//! TESTS
	struct NerdChess::board::position test_pos = NerdChess::board::get_empty_position();

	NerdChess::board::setup_position(test_pos);

	while(1)
	{
		struct NerdChess::engine::engine_eval eval1 = NerdChess::engine::minimax(test_pos, true, -INT_MAX, INT_MAX, 4);
		NerdChess::board::move_piece(test_pos, eval1.best_move[0], eval1.best_move[1]);

		system("cls");
		NerdChess::board::debug::print_board(test_pos);

		/*int from;
		int to;
		std::cout << "\nFrom: ";
		std::cin >> from;
		std::cout << "To: ";
		std::cin >> to;

		NerdChess::board::move_piece(test_pos, from, to);*/

		system("cls");
		NerdChess::board::debug::print_board(test_pos);

		struct NerdChess::engine::engine_eval eval2 = NerdChess::engine::minimax(test_pos, false, -INT_MAX, INT_MAX, 4);
		NerdChess::board::move_piece(test_pos, eval2.best_move[0], eval2.best_move[1]);

		system("cls");
		NerdChess::board::debug::print_board(test_pos);
	}

	return 0;
}
