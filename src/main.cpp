#include <iostream>
#include <Windows.h>
#include "headers/engine.h"
#include "headers/opening.h"

// TODO Move to a different file?
std::string board_to_str(NerdChess::board::position board)
{
    const char piece_chars[] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};
    std::string str;
    for(int i = 0; i < 64; ++i)
	{
		int pt = NerdChess::board::get_full_piece_type(board, i);
		if(pt == EMPTY)
			str += ".";
		else
			str += piece_chars[pt];
	}
    return str;
}

// Main function
int main()
{
	// Initialization
	NerdChess::generate_board_control_value_map(NerdChess::board_control_value_map_w, WHITE);
	NerdChess::generate_board_control_value_map(NerdChess::board_control_value_map_b, BLACK);
	NerdChess::generate_square_safety_map(NerdChess::square_safety_map_w, WHITE); // ?
	NerdChess::generate_square_safety_map(NerdChess::square_safety_map_b, BLACK); // ?
	NerdChess::board_color_map = NerdChess::generate_board_color_map();
	NerdChess::opening::init_opening_book();

	// Initialize board
	struct NerdChess::board::position board = NerdChess::board::get_empty_position();
	NerdChess::board::setup_position(board);

	// Cursor
	int selected_piece = 63;
	int selected_square = 63;

	// Game
	while(1)
	{
		// PLayer turn
		system("cls");
		NerdChess::board::print_board(board, -1, selected_square);

		// Select piece
		while(1)
		{
			if(GetKeyState('W') & 0x8000)
			{
				selected_piece -= 8;
				system("cls");
				NerdChess::board::print_board(board, selected_piece, -1);
			}

			if(GetKeyState('S') & 0x8000)
			{
				selected_piece += 8;
				system("cls");
				NerdChess::board::print_board(board, selected_piece, -1);
			}

			if(GetKeyState('A') & 0x8000)
			{
				selected_piece--;
				system("cls");
				NerdChess::board::print_board(board, selected_piece, -1);
			}

			if(GetKeyState('D') & 0x8000)
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
			if(GetKeyState('W') & 0x8000)
			{
				selected_square -= 8;
				system("cls");
				NerdChess::board::print_board(board, -1, selected_square);
			}

			if(GetKeyState('S') & 0x8000)
			{
				selected_square += 8;
				system("cls");
				NerdChess::board::print_board(board, -1, selected_square);
			}

			if(GetKeyState('A') & 0x8000)
			{
				selected_square--;
				system("cls");
				NerdChess::board::print_board(board, -1, selected_square);
			}

			if(GetKeyState('D') & 0x8000)
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
		try
		{
			// See if a book move is possible
			auto book_move = NerdChess::opening::book.at(board_to_str(board));
			NerdChess::board::move_piece(board, book_move[0], book_move[1]);
		}
		catch(const std::exception& e)
		{
			struct NerdChess::engine::engine_eval eval = NerdChess::engine::minimax(board, false, -INT_MAX, INT_MAX, 4); // Actual thinking
			NerdChess::board::move_piece(board, eval.best_move[0], eval.best_move[1]);

			system("cls");
			NerdChess::board::debug::print_board(board);
		}
	}

	return 0;
}
