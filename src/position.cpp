#include <iostream>
#include <vector>
#include "position.h"

using namespace NerdChess::bitb;

bool NerdChess::board::is_empty(struct NerdChess::board::position board, uint8_t square_location) {
	for(int i = 0; i < 12; ++i)
		if(NerdChess::bitb::get_bit(board.pieces[i], square_location))
			return false;
	return true;
}

bool NerdChess::board::piece_color_at(struct position board, uint8_t square_location, uint8_t piece_color) {
	if(!piece_color) {
		for(int i = 0; i < 6; ++i)
			if(NerdChess::bitb::get_bit(board.pieces[i], square_location))
				return true;
	} else {
		for(int i = 6; i < 12; ++i)
			if(NerdChess::bitb::get_bit(board.pieces[i], square_location))
				return true;
	}
	return false;
}

int NerdChess::board::get_piece_type(struct position board, uint8_t square_location) {
	for(int i = 0; i < 6; ++i)
		if(NerdChess::bitb::get_bit(board.pieces[i], square_location))
			return i;
	for(int i = 6; i < 12; ++i)
		if(NerdChess::bitb::get_bit(board.pieces[i], square_location))
			return i-6;
	return EMPTY;
}

int NerdChess::board::get_full_piece_type(struct position board, uint8_t square_location) {
	for(int i = 0; i < 12; ++i)
		if(NerdChess::bitb::get_bit(board.pieces[i], square_location))
			return i;
	return EMPTY;
}

void NerdChess::board::remove_piece(struct position& board, int square_location) {
	for(int i = 0; i < 12; ++i)
		clear_bit(board.pieces[i], square_location);
}

void NerdChess::board::move_piece(struct position& board, int from, int to) {
	int piece = get_full_piece_type(board, from);
	remove_piece(board, to);
	NerdChess::bitb::move_bit(board.pieces[piece], from, to);
	// Special pawn moves
	if(piece == PAWN) {
		// White pawn

		if((from - to) == 16)
			board.en_pessant_squares[BLACK] = to + 8;
		// En pessant capture
		if(!piece_color_at(board, to, BLACK))
			bitb::clear_bit(board.pieces[6], to + 8);
		// Promote to queen
		if(to < 8)
		{
			remove_piece(board, to);
			NerdChess::bitb::set_bit(board.pieces[QUEEN], to);
		}
	} else if(piece == PAWN+6) {
		// Black pawn

		if((to - from) == 16)
			board.en_pessant_squares[WHITE] = to - 8;
		// En croissant capture
		if(!piece_color_at(board, to, WHITE))
			NerdChess::bitb::clear_bit(board.pieces[PAWN], to - 8);
		// Promote to queen
		if(to > 55) {
			remove_piece(board, to);
			NerdChess::bitb::set_bit(board.pieces[QUEEN+6], to);
		}
	}
}

NerdChess::bitb::bitboard NerdChess::board::map_bitboard(std::vector<int> vec) {
	NerdChess::bitb::bitboard bb = 0ULL;
	for(int i = 0; i < vec.size(); ++i)
		NerdChess::bitb::set_bit(bb, vec[i]);
	return bb;
}

NerdChess::bitb::bitboard NerdChess::board::get_control_map(struct position board, bool piece_color) {
	NerdChess::bitb::bitboard map = 0ULL;
	for(int i = 0; i < 64; i++)
	{
		int piece = get_piece_type(board, i);
		if(piece != EMPTY)
			map |= map_bitboard(get_moves(board, i, piece, piece_color, true));
	}
	return map;
}

NerdChess::bitb::bitboard NerdChess::board::map_pieces(struct NerdChess::board::position board) {
	NerdChess::bitb::bitboard map = 0ULL;
	for(int i = 0; i < 12; ++i)
		map |= board.pieces[i];
	return map;
}

int NerdChess::board::count_bits(NerdChess::bitb::bitboard bb) {
	uint8_t num_bits = 0;
	for(int i = 0; i < 64; ++i)
		if(get_bit(bb, i))
			num_bits++;
	return num_bits;
}

int NerdChess::board::count_pieces(struct NerdChess::board::position board) {
	NerdChess::bitb::bitboard map = 0ULL;
	for(int i = 0; i < 12; ++i)
		map |= board.pieces[i];
	return count_bits(map);
}

std::vector<int> NerdChess::board::get_moves(struct position pos, uint8_t piece_location, uint8_t piece_type, bool piece_color, bool control) {
	std::vector<int> legal_moves; // Vector of legal moves
	const uint8_t opposite_piece_color = !piece_color; // This is used to determine what types of pieces the selected piece is allowed to capture
	bitboard piece_map = map_pieces(pos); // Squares on which there are pieces
	switch(piece_type) {
		case PAWN:
			if(!control) {
				if(!piece_color) {
					// White pawns
					if(pos.en_pessant_squares[WHITE] >= 0 && ((piece_location - 9) == pos.en_pessant_squares[WHITE] || (piece_location - 7) == pos.en_pessant_squares[WHITE]))
						legal_moves.push_back(pos.en_pessant_squares[WHITE]); // En pessant
					if(!NerdChess::bitb::get_bit(piece_map, piece_location - 8) && piece_location > 7)
						legal_moves.push_back(piece_location - 8); // 1 square forward
					if(!NerdChess::bitb::get_bit(piece_map, piece_location - 16) && !get_bit(piece_map, piece_location - 8) && piece_location <= 55 && piece_location >= 48)
						legal_moves.push_back(piece_location - 16); // 2 squares forward (if the pawn is still on it's starting square)
					if(piece_color_at(pos, piece_location - 9, opposite_piece_color) && piece_location % 8 != 0)
						legal_moves.push_back(piece_location - 9); // Piece capture (left)
					if(piece_color_at(pos, piece_location - 7, opposite_piece_color) && (piece_location+1) % 8 != 0)
						legal_moves.push_back(piece_location - 7); // Piece capture (right)
				} else {
					// Black pawns
					if(pos.en_pessant_squares[BLACK] >= 0 && ((piece_location + 9) == pos.en_pessant_squares[BLACK] || (piece_location + 7) == pos.en_pessant_squares[BLACK]))
						legal_moves.push_back(pos.en_pessant_squares[BLACK]); // En pessant
					if(NerdChess::bitb::get_bit(piece_map, piece_location + 8) == 0 && piece_location < 56)
						legal_moves.push_back(piece_location + 8); // 1 square forward
					if(NerdChess::bitb::get_bit(piece_map, piece_location + 16) == 0 && get_bit(piece_map, piece_location + 8) == 0 && piece_location <= 15 && piece_location >= 8)
						legal_moves.push_back(piece_location + 16); // 2 squares forward (if the pawn is still on it's starting square)
					if(piece_color_at(pos, piece_location + 7, opposite_piece_color) && piece_location % 8 != 0)
						legal_moves.push_back(piece_location + 7); // Piece capture (left)
					if(piece_color_at(pos, piece_location + 9, opposite_piece_color) && (piece_location+1) % 8 != 0)
						legal_moves.push_back(piece_location + 9); // Piece capture (right)
				}
			} else {
				if(!piece_color) {
					// White pawns
					if(piece_location % 8 != 0)
						legal_moves.push_back(piece_location - 9); // Piece capture (left)
					if((piece_location+1) % 8 != 0)
						legal_moves.push_back(piece_location - 7); // Piece capture (right)
				} else {
					// Black pawns
					if(piece_location % 8 != 0)
						legal_moves.push_back(piece_location + 7); // Piece capture (left)
					if((piece_location+1) % 8 != 0)
						legal_moves.push_back(piece_location + 9); // Piece capture (right)
				}
			}
		break;

		case KNIGHT:
		if(!control) {
			if((!NerdChess::bitb::get_bit(piece_map, piece_location - 15) || piece_color_at(pos, piece_location - 15, opposite_piece_color)) && (piece_location+1) % 8 != 0 && piece_location > 15)
				legal_moves.push_back(piece_location - 15); // 2 up, 1 right
			if((!NerdChess::bitb::get_bit(piece_map, piece_location - 17) || piece_color_at(pos, piece_location - 17, opposite_piece_color)) && piece_location % 8 != 0 && piece_location > 15)
				legal_moves.push_back(piece_location - 17); // 2 up, 1 left
			if((!NerdChess::bitb::get_bit(piece_map, piece_location - 6) || piece_color_at(pos, piece_location - 6, opposite_piece_color)) && (piece_location+2) % 8 != 0 && (piece_location+1) % 8 != 0 && piece_location > 7)
				legal_moves.push_back(piece_location - 6); // 1 up, 2 right
			if((!NerdChess::bitb::get_bit(piece_map, piece_location - 10) || piece_color_at(pos, piece_location - 10, opposite_piece_color)) && piece_location % 8 != 0 && (piece_location-1) % 8 != 0 && piece_location > 7)
				legal_moves.push_back(piece_location - 10); // 1 up, 2 left
			if((!NerdChess::bitb::get_bit(piece_map, piece_location + 10) || piece_color_at(pos, piece_location + 10, opposite_piece_color)) && (piece_location+2) % 8 != 0 && (piece_location+1) % 8 != 0 && piece_location < 56)
				legal_moves.push_back(piece_location + 10); // 1 down, 2 right
			if((!NerdChess::bitb::get_bit(piece_map, piece_location + 6) || piece_color_at(pos, piece_location + 6, opposite_piece_color)) && (piece_location % 8) != 0 && (piece_location-1) % 8 != 0 && piece_location < 56)
				legal_moves.push_back(piece_location + 6); // 1 down, 2 left
			if((!NerdChess::bitb::get_bit(piece_map, piece_location + 17) || piece_color_at(pos, piece_location + 17, opposite_piece_color)) && (piece_location+1) % 8 != 0 && piece_location < 48)
				legal_moves.push_back(piece_location + 17); // 2 down, 1 right
			if((!NerdChess::bitb::get_bit(piece_map, piece_location + 15) || piece_color_at(pos, piece_location + 15, opposite_piece_color)) && (piece_location % 8) != 0 && piece_location < 48)
				legal_moves.push_back(piece_location + 15); // 2 down, 1 left
		} else {
			if((piece_location+1) % 8 != 0 && piece_location > 15)
				legal_moves.push_back(piece_location - 15); // 2 up, 1 right
			if(piece_location % 8 != 0 && piece_location > 15)
				legal_moves.push_back(piece_location - 17); // 2 up, 1 left
			if((piece_location+2) % 8 != 0 && (piece_location+1) % 8 != 0 && piece_location > 7)
				legal_moves.push_back(piece_location - 6); // 1 up, 2 right
			if((piece_location % 8) != 0 && (piece_location-1) % 8 != 0 && piece_location > 7)
				legal_moves.push_back(piece_location - 10); // 1 up, 2 left
			if((piece_location+2) % 8 != 0 && (piece_location+1) % 8 != 0 && piece_location < 56)
				legal_moves.push_back(piece_location + 10); // 1 down, 2 right
			if((piece_location % 8) != 0 && (piece_location-1) % 8 != 0 && piece_location < 56)
				legal_moves.push_back(piece_location + 6); // 1 down, 2 left
			if((piece_location+1) % 8 != 0 && piece_location < 48)
				legal_moves.push_back(piece_location + 17); // 2 down, 1 right
			if((piece_location % 8) != 0 && piece_location < 48)
				legal_moves.push_back(piece_location + 15); // 2 down, 1 left
		}
		break;

		case BISHOP:
		if(!control) {
			// North-west
			if(piece_location > 7 && (piece_location % 8) != 0) {
				int i = piece_location - 9;
				while(1) {
					if(!NerdChess::bitb::get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i < 8 || i % 8 == 0)
						break;
					i -= 9;
				}
			}
			// South-east
			if(piece_location < 56 && ((piece_location+1) % 8) != 0) {
				int i = piece_location + 9;
				while(1) {
					if(!NerdChess::bitb::get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i >= 56 || (i+1) % 8 == 0)
						break;
					i += 9;
				}
			}
			// North-east
			if(piece_location > 7 && ((piece_location+1) % 8) != 0) {
				int i = piece_location - 7;
				while(1) {
					if(!NerdChess::bitb::get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i < 8 || (i+1) % 8 == 0)
						break;
					i -= 7;
				}
			}

			// South-west
			if(piece_location < 56 && (piece_location % 8) != 0) {
				int i = piece_location + 7;
				while(1) {
					if(!NerdChess::bitb::get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i % 8 == 0 || i >= 56)
						break;
					i += 7;
				}
			}
		} else {
			// North-west
			if(piece_location > 7 && (piece_location % 8) != 0) {
				int i = piece_location - 9;
				while(1) {
					legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i < 8 || i % 8 == 0)
						break;
					i -= 9;
				}
			}
			// South-east
			if(piece_location < 56 && ((piece_location+1) % 8) != 0) {
				int i = piece_location + 9;
				while(1) {
					legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i >= 56 || (i+1) % 8 == 0)
						break;
					i += 9;
				}
			}
			// North-east
			if(piece_location > 7 && ((piece_location+1) % 8) != 0) {
				int i = piece_location - 7;
				while(1) {
					legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i < 8 || (i+1) % 8 == 0)
						break;
					i -= 7;
				}
			}

			// South-west
			if(piece_location < 56 && (piece_location % 8) != 0) {
				int i = piece_location + 7;
				while(1) {
					legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i < 8 || i % 8 == 0)
						break;
					i += 7;
				}
			}
		}
		break;

		case ROOK:
		if(!control)
		{
			// Up
			if(piece_location > 7) {
				for(int i = piece_location - 8; i > -1; i -= 8) {
					if(!NerdChess::bitb::get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i))
						break;
				}
			}
			// Down
			if(piece_location < 56) {
				for(int i = piece_location + 8; i < 56; i += 8) {
					if(!NerdChess::bitb::get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i))
						break;
				}
			}
			// Left
			if(piece_location % 8 != 0) {
				for(int i = piece_location - 1; i > -1; --i) {
					if(!NerdChess::bitb::get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i % 8 == 0)
						break;
				}
			}
			// Right
			if((piece_location+1) % 8 != 0) {
				for(int i = piece_location + 1; i < 63; ++i) {
					if(!NerdChess::bitb::get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || (i+1) % 8 == 0)
						break;
				}
			}
		}
		else
		{
			// Up
			if(piece_location > 7) {
				for(int i = piece_location - 8; i > -1; i -= 8) {
					legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i))
						break;
				}
			}
			// Down
			if(piece_location < 56) {
				for(int i = piece_location + 8; i < 56; i += 8) {
					legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i))
						break;
				}
			}
			// Left
			if(piece_location % 8 != 0) {
				for(int i = piece_location - 1; i > -1; --i) {
					legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i % 8 == 0)
						break;
				}
			}
			// Right
			if((piece_location+1) % 8 != 0) {
				for(int i = piece_location + 1; i < 63; ++i) {
					legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || (i+1) % 8 == 0)
						break;
				}
			}
		}
		break;

		case QUEEN:
		if(!control) {
			// ROOK MOVEMENT
			// Up
			if(piece_location > 7) {
				for(int i = piece_location - 8; i > -1; i -= 8) {
					if(!NerdChess::bitb::get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i))
						break;
				}
			}
			// Down
			if(piece_location < 56) {
				for(int i = piece_location + 8; i < 56; i += 8) {
					if(!NerdChess::bitb::get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i))
						break;
				}
			}
			// Left
			if(piece_location % 8 != 0) {
				for(int i = piece_location - 1; i > -1; --i) {
					if(!NerdChess::bitb::get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i % 8 == 0)
						break;
				}
			}
			// Right
			if((piece_location+1) % 8 != 0) {
				for(int i = piece_location + 1; i < 63; ++i) {
					if(!NerdChess::bitb::get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || (i+1) % 8 == 0)
						break;
				}
			}

			// BISHOP MOVEMENT
			// North-west
			if(piece_location > 7 && (piece_location % 8) != 0) {
				int i = piece_location - 9;
				while(1) {
					if(!NerdChess::bitb::get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i < 8 || i % 8 == 0)
						break;
					i -= 9;
				}
			}
			// South-east
			if(piece_location < 56 && ((piece_location+1) % 8) != 0) {
				int i = piece_location + 9;
				while(1) {
					if(!NerdChess::bitb::get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i >= 56 || (i+1) % 8 == 0)
						break;
					i += 9;
				}
			}
			// North-east
			if(piece_location > 7 && ((piece_location+1) % 8) != 0) {
				int i = piece_location - 7;
				while(1) {
					if(!NerdChess::bitb::get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i < 8 || (i+1) % 8 == 0)
						break;
					i -= 7;
				}
			}

			// South-west
			if(piece_location < 56 && (piece_location % 8) != 0) {
				int i = piece_location + 7;
				while(1) {
					if(!NerdChess::bitb::get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i % 8 != 0 || i >= 56)
						break;
					i += 7;
				}
			}
		} else {
			// ROOK MOVEMENT
			// Up
			if(piece_location > 7) {
				for(int i = piece_location - 8; i > -1; i -= 8) {
					legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i))
						break;
				}
			}
			// Down
			if(piece_location < 56) {
				for(int i = piece_location + 8; i < 56; i += 8) {
					legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i))
						break;
				}
			}
			// Left
			if(piece_location % 8 != 0) {
				for(int i = piece_location - 1; i > -1; --i) {
					legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i % 8 == 0)
						break;
				}
			}
			// Right
			if((piece_location+1) % 8 != 0) {
				for(int i = piece_location + 1; i < 63; ++i) {
					legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || (i+1) % 8 == 0)
						break;
				}
			}

			// BISHOP MOVEMENT
			// North-west
			if(piece_location > 7 && (piece_location % 8) != 0) {
				int i = piece_location - 9;
				while(1) {
					legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i < 8 || i % 8 == 0)
						break;
					i -= 9;
				}
			}
			// South-east
			if(piece_location < 56 && ((piece_location+1) % 8) != 0) {
				int i = piece_location + 9;
				while(1) {
					legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i >= 56 || (i+1) % 8 == 0)
						break;
					i += 9;
				}
			}
			// North-east
			if(piece_location > 7 && ((piece_location+1) % 8) != 0) {
				int i = piece_location - 7;
				while(1) {
					legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i < 8 || (i+1) % 8 == 0)
						break;
					i -= 7;
				}
			}

			// South-west
			if(piece_location < 56 && (piece_location % 8) != 0) {
				int i = piece_location + 7;
				while(1) {
					legal_moves.push_back(i);
					if(NerdChess::bitb::get_bit(piece_map, i) || i < 8 || i % 8 != 0)
						break;
					i += 7;
				}
			}
		}
		break;

		case KING:
		if(!control) {
			// Control map for enemy pieces
			// Used to map squares the king CANNOT move to
			const NerdChess::bitb::bitboard control_map = get_control_map(pos, opposite_piece_color);

			// Loops in a way that creates a 3x3 area around the piece_location (ignores piece_locations itself since the king can't control it's own square)
			for(int i = piece_location - 9; i < piece_location + 9; i += 8)
				for(int j = 0; j < 3; ++j)
					if(i+j != piece_location && i+j >= 0 && i+j < 64)
					//if(NerdChess::bitb::get_bit(control_map, i+j) == 0 && i+j != piece_location && i+j >= 0 && i+j < 64)
						legal_moves.push_back(i+j);
		}
		else
			// Loops in a way that creates a 3x3 area around the piece_location (ignores piece_locations itself since the king can't control it's own square)
			for(int i = piece_location - 9; i < piece_location + 9; i += 8)
				for(int j = 0; j < 3; ++j)
					if(i+j != piece_location && i+j >= 0 && i+j < 64)
						legal_moves.push_back(i+j);
		break;

		// Default (invalid argument)
		default:
		std::cerr << "Invalid piece type \"" << piece_type << "\"";
		break;
	}
	return legal_moves;
}

void NerdChess::board::setup_position(struct position& board) {
	board.castling_rights[WHITE] = true;
	board.castling_rights[BLACK] = true;
	board.en_pessant_squares[WHITE] = INT_MIN;
	board.en_pessant_squares[BLACK] = INT_MIN;

	for(int i = 0; i < 12; ++i)
		board.pieces[i] = 0ULL;

	// Pawns
	for(int i = 0; i < 8; ++i) {
		NerdChess::bitb::set_bit(board.pieces[0], i + 48); // Black pawns
		NerdChess::bitb::set_bit(board.pieces[6], i + 8); // Black pawns
	}

	// Knights
	// White knights
	NerdChess::bitb::set_bit(board.pieces[1], 62);
	NerdChess::bitb::set_bit(board.pieces[1], 57);
	// Black knights
	NerdChess::bitb::set_bit(board.pieces[7], 6);
	NerdChess::bitb::set_bit(board.pieces[7], 1);

	// Bishops
	// White bishops
	NerdChess::bitb::set_bit(board.pieces[2], 61);
	NerdChess::bitb::set_bit(board.pieces[2], 58);
	// Black bishops
	NerdChess::bitb::set_bit(board.pieces[8], 5);
	NerdChess::bitb::set_bit(board.pieces[8], 2);

	// THE ROOK
	// White rook
	NerdChess::bitb::set_bit(board.pieces[3], 63);
	NerdChess::bitb::set_bit(board.pieces[3], 56);
	// Black rook
	NerdChess::bitb::set_bit(board.pieces[9], 7);
	NerdChess::bitb::set_bit(board.pieces[9], 0);

	// Queen
	NerdChess::bitb::set_bit(board.pieces[4], 59); // White queen
	NerdChess::bitb::set_bit(board.pieces[10], 3); // Black queen

	// King
	NerdChess::bitb::set_bit(board.pieces[5], 60); // White king
	NerdChess::bitb::set_bit(board.pieces[11], 4); // Black king
}

// Finds the piece (works best with only 1 present piece)
int NerdChess::board::find_piece(bitboard bb) {
	NerdChess::bitb::bitboard b = bb;
	for(int i = 0; i < 64; ++i) {
		if(b == 1ULL) return i+1;
		b = b >> 1;
	}
	return -1;
}

void NerdChess::board::print_board(struct position board, int sp, int ss) {
	const std::string piece_symbols[] = {"\033[97mp", "\033[97mN", "\033[97mB", "\033[97mR", "\033[97mQ", "\033[97mK", "\033[90mp", "\033[90mN", "\033[90mB", "\033[90mR", "\033[90mQ", "\033[90mK"};
	for(int i = 0; i < 8; ++i) {
		for(int j = 0; j < 8; ++j) {
			if(sp == j + i*8) {
				std::cout << "\x1b[43m  \x1b[0m";
			} else if(ss == j + i*8) {
				std::cout << "\x1b[45m  \x1b[0m";
			} else {
				const int piece_type = get_full_piece_type(board, i*8+j);
				if(piece_type == EMPTY)
					std::cout << "\033[90m. ";
				else
					std::cout << piece_symbols[piece_type] << " ";
			}
		}
		std::cout << "\n";
	}
	std::cout << "\033[97m\n";
}

std::string NerdChess::board::board_to_str(NerdChess::board::position board) {
    const char piece_chars[] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};
    std::string str;
    for(int i = 0; i < 64; ++i) {
		int pt = NerdChess::board::get_full_piece_type(board, i);
		if(pt == EMPTY)
			str += ".";
		else
			str += piece_chars[pt];
	}
    return str;
}

// Prints out a vector with a JavaScript-like format.
// Example output:
// [1, 143, 92]
void NerdChess::board::debug::print_vec(std::vector<int> vec)
{
	if(vec.size() > 0) {
		std::cout << "[";
		for(int i = 0; i < vec.size()-1; ++i)
			std::cout << vec[i] << ", ";
		std::cout << vec[vec.size()-1] << "]\n";
	} else {
		std::cout << "[]\n";
	}
}

// Prints out the entire collection of bitboards as one board
void NerdChess::board::debug::print_board(struct position board) {
	const std::string piece_symbols[] = {"\033[97mp", "\033[97mN", "\033[97mB", "\033[97mR", "\033[97mQ", "\033[97mK", "\033[90mp", "\033[90mN", "\033[90mB", "\033[90mR", "\033[90mQ", "\033[90mK"};
	for(int i = 0; i < 8; ++i) {
		for(int j = 0; j < 8; ++j) {
			const int piece_type = get_full_piece_type(board, i*8+j);
			if(piece_type == EMPTY)
				std::cout << "\033[90m. ";
			else
				std::cout << piece_symbols[piece_type] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\033[97m\n";
}
