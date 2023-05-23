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
	struct NerdChess::board::position test_pos = NerdChess::board::empty_position();

	/*set_bit(test_pos.king_w, 57);
	set_bit(test_pos.king_b, 1);

	set_bit(test_pos.pawn_b, 15);
	set_bit(test_pos.pawn_b, 14);
	set_bit(test_pos.pawn_b, 13);
	set_bit(test_pos.pawn_b, 12);

	set_bit(test_pos.pawn_w, 55);
	set_bit(test_pos.pawn_w, 46);
	set_bit(test_pos.pawn_w, 37);
	set_bit(test_pos.pawn_w, 28);*/

	NerdChess::board::setup_position(test_pos);

	while(1)
	{
		auto start = std::chrono::high_resolution_clock::now();

		struct NerdChess::engine::engine_eval eval = NerdChess::engine::minimax(test_pos, true, -INT_MAX, INT_MAX, 4);
		NerdChess::board::move_piece(test_pos, eval.best_move[0], eval.best_move[1]);

		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		std::cout << "Position evaluation took up " << (double)(duration.count()) / 1000000 << " seconds\n";

		system("cls");
		NerdChess::board::debug::print_board(test_pos);

		int from;
		int to;
		std::cout << "\nFrom: ";
		std::cin >> from;
		std::cout << "\nTo: ";
		std::cin >> to;

		NerdChess::board::move_piece(test_pos, from, to);

		system("cls");
		NerdChess::board::debug::print_board(test_pos);
	}

	// Get starting timepoint
	/*auto start = std::chrono::high_resolution_clock::now();

	std::cout << "Eval: " << (double)NerdChess::eval::eval_position(test_pos)/100 << "\n"; //! EVALUATION TEST

	// Get ending timepoint
    auto stop = std::chrono::high_resolution_clock::now();
 
    // Get duration. Substart timepoints to
    // get duration. To cast it to proper unit
    // use duration cast method
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
 
    std::cout << "Time taken by function: "
         << (double)(duration.count()) / 1000000 << " seconds\n";*/



	/*std::cout << "\n\n\n";
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			std::cout << std::to_string(NerdChess::square_safety_map_w[i*8 + j]) << " ";
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
