/*
NerdChess is my first attempt at making a working Chess Engine, I started working on it somewhere in
the spring of 2023 when I was 15.

This is currently a WIP (work in progress), but I am regularly adding new features. For more info,
go to README.md

Please note: because of school I am not able to add features regularly
*/

#include <iostream>
#include <Windows.h>
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

	// Initialize board
	struct NerdChess::board::position board = NerdChess::board::get_empty_position();
	NerdChess::board::setup_position(board);

	int selected_piece = 63;
	int selected_square = 63;

 	while(1)
	{
		// PLayer turn
		system("cls");
		NerdChess::board::print_board(board, -1, selected_square);

		// Select piece
		while(1)
		{
			if(GetKeyState(VK_UP) & 0x8000)
			{
				selected_piece -= 8;
				system("cls");
				NerdChess::board::print_board(board, selected_piece, -1);
			}

			if(GetKeyState(VK_DOWN) & 0x8000)
			{
				selected_piece += 8;
				system("cls");
				NerdChess::board::print_board(board, selected_piece, -1);
			}

			if(GetKeyState(VK_LEFT) & 0x8000)
			{
				selected_piece--;
				system("cls");
				NerdChess::board::print_board(board, selected_piece, -1);
			}

			if(GetKeyState(VK_RIGHT) & 0x8000)
			{
				selected_piece++;
				system("cls");
				NerdChess::board::print_board(board, selected_piece, -1);
			}

			if(GetKeyState(VK_SPACE) & 0x8000)
			{
				system("cls");
				NerdChess::board::print_board(board, selected_piece, -1);
				selected_square = selected_piece;
				break;
			}

			Sleep(80);
		}

		Sleep(200);

		// Select square
		while(1)
		{
			if(GetKeyState(VK_UP) & 0x8000)
			{
				selected_square -= 8;
				system("cls");
				NerdChess::board::print_board(board, -1, selected_square);
			}

			if(GetKeyState(VK_DOWN) & 0x8000)
			{
				selected_square += 8;
				system("cls");
				NerdChess::board::print_board(board, -1, selected_square);
			}

			if(GetKeyState(VK_LEFT) & 0x8000)
			{
				selected_square--;
				system("cls");
				NerdChess::board::print_board(board, -1, selected_square);
			}

			if(GetKeyState(VK_RIGHT) & 0x8000)
			{
				selected_square++;
				system("cls");
				NerdChess::board::print_board(board, -1, selected_square);
			}

			if(GetKeyState(VK_SPACE) & 0x8000)
			{
				system("cls");
				NerdChess::board::print_board(board, -1, selected_square);
				break;
			}

			Sleep(80);
		}

		NerdChess::board::move_piece(board, selected_piece, selected_square);
		system("cls");
		NerdChess::board::debug::print_board(board);

		// CPU turn
		struct NerdChess::engine::engine_eval eval = NerdChess::engine::minimax(board, false, -INT_MAX, INT_MAX, 4);
		NerdChess::board::move_piece(board, eval.best_move[0], eval.best_move[1]);
		std::cout << eval.best_move[1] << "\n";
		system("cls");
		NerdChess::board::debug::print_board(board);
	}

	return 0;
}
