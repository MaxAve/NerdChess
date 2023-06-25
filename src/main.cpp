#include <iostream>
#include <Windows.h>
#include "engine.h"
#include "opening.h"

#define SQUARE_SIZE 110

void GetDesktopResolution(int& horizontal, int& vertical) {
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

int main(int argc, char* argv[]) {
	srand(time(NULL));

	// Initialization
	NerdChess::generate_board_control_value_map(NerdChess::board_control_value_map_w, WHITE);
	NerdChess::generate_board_control_value_map(NerdChess::board_control_value_map_b, BLACK);
	NerdChess::opening::init_opening_book();

	// Initialize board
	struct NerdChess::board::position board = NerdChess::board::get_empty_position();
	NerdChess::board::setup_position(board);

	// Cursor
	int selectedPiece = 0;
	int selectedSquare = 0;

	// Game
	while(1) {
		// PLayer turn
		system("cls");
		NerdChess::board::print_board(board, -1, selectedSquare);

		// Select piece
		while(1) {
			if(GetKeyState(VK_UP) & 0x8000) {
				selectedPiece -= 8;
				system("cls");
				NerdChess::board::print_board(board, selectedPiece, -1);
			}

			if(GetKeyState(VK_DOWN) & 0x8000) {
				selectedPiece += 8;
				system("cls");
				NerdChess::board::print_board(board, selectedPiece, -1);
			}

			if(GetKeyState(VK_LEFT) & 0x8000) {
				selectedPiece--;
				system("cls");
				NerdChess::board::print_board(board, selectedPiece, -1);
			}

			if(GetKeyState(VK_RIGHT) & 0x8000) {
				selectedPiece++;
				system("cls");
				NerdChess::board::print_board(board, selectedPiece, -1);
			}

			if(GetKeyState(VK_SPACE) & 0x8000) {
				system("cls");
				NerdChess::board::print_board(board, selectedPiece, -1);
				selectedSquare = selectedPiece;
				break;
			}

			Sleep(80);
		}

		Sleep(200);

		// Select square
		while(1)
		{
			if(GetKeyState(VK_UP) & 0x8000) {
				selectedSquare -= 8;
				system("cls");
				NerdChess::board::print_board(board, -1, selectedSquare);
			}

			if(GetKeyState(VK_DOWN) & 0x8000) {
				selectedSquare += 8;
				system("cls");
				NerdChess::board::print_board(board, -1, selectedSquare);
			}

			if(GetKeyState(VK_LEFT) & 0x8000) {
				selectedSquare--;
				system("cls");
				NerdChess::board::print_board(board, -1, selectedSquare);
			}

			if(GetKeyState(VK_RIGHT) & 0x8000) {
				selectedSquare++;
				system("cls");
				NerdChess::board::print_board(board, -1, selectedSquare);
			}

			if(GetKeyState(VK_SPACE) & 0x8000) {
				system("cls");
				NerdChess::board::print_board(board, -1, selectedSquare);
				break;
			}

			Sleep(80);
		}

		NerdChess::board::move_piece(board, selectedPiece, selectedSquare);
		system("cls");
		NerdChess::board::debug::print_board(board);

		// CPU turn
		try {
			// See if a book move is possible
			auto book_move = NerdChess::opening::book.at(NerdChess::board::board_to_str(board));
			NerdChess::board::move_piece(board, book_move[0], book_move[1]);
		}
		catch(const std::exception& e) {
			struct NerdChess::engine::engine_eval eval = NerdChess::engine::minimax(board, false, -INT_MAX, INT_MAX, 4); // Actual thinking
			NerdChess::board::move_piece(board, eval.best_move[0], eval.best_move[1]);

			system("cls");
			NerdChess::board::debug::print_board(board);
		}
	}

	return EXIT_SUCCESS;
}
