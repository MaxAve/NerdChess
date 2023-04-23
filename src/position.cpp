/*
Here, position refers to a combination of (12) bitboards which define a specific
chess piece layout. This file contains all necessary function for working with
a position, such as retrieving piece types from squares and working with them.
*/

#include <iostream>
#include <vector>
#include "position.h"
#include "bitboard.h"

using namespace NerdChess::bitb;

namespace NerdChess
{
namespace board
{
/**
 * @brief Returns true if the square at position square_location is empty
 * @param pos position (board state)
 * @param square_location the location on the board at which to check for pieces
 */
bool is_empty(struct position pos, uint8_t square_location)
{
    return !(get_bit(pos.pawn_w, square_location) | get_bit(pos.pawn_b, square_location) | get_bit(pos.knight_w, square_location) | get_bit(pos.knight_b, square_location) | get_bit(pos.bishop_w, square_location) | get_bit(pos.bishop_b, square_location) | get_bit(pos.rook_w, square_location) | get_bit(pos.rook_b, square_location) | get_bit(pos.queen_w, square_location) | get_bit(pos.queen_b, square_location) | get_bit(pos.king_w, square_location) | get_bit(pos.king_b, square_location));
}

/**
 * @brief 
 * Is quite similar to is_empty() but instead of returning whether a square
 * is empty or not, piece_color_at() returns whether a piece of a specific
 * color (white/black) is on the square.
 * @param pos position (board state)
 * @param square_location the location on the board at which to check
 * @param piece_color piece color to check for
 */
bool piece_color_at(struct position pos, uint8_t square_location, uint8_t piece_color)
{
    return (piece_color == WHITE ? (get_bit(pos.pawn_w, square_location) | get_bit(pos.knight_w, square_location) | get_bit(pos.bishop_w, square_location) | get_bit(pos.rook_w, square_location) | get_bit(pos.queen_w, square_location) | get_bit(pos.king_w, square_location)) : (get_bit(pos.pawn_b, square_location) | get_bit(pos.knight_b, square_location) | get_bit(pos.bishop_b, square_location) | get_bit(pos.rook_b, square_location) | get_bit(pos.queen_b, square_location) | get_bit(pos.king_b, square_location)));
}

/**
 * @brief Returns the piece type at the selected square. Use macros declared in position.h (PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING)
 * 
 * @param pos 
 * @param square_location 
 */
uint8_t get_piece_type(struct position pos, uint8_t square_location)
{
	if(get_bit(pos.pawn_w, square_location) || get_bit(pos.pawn_b, square_location))
		return PAWN;
	else if(get_bit(pos.knight_w, square_location) || get_bit(pos.knight_b, square_location))
		return KNIGHT;
	else if(get_bit(pos.bishop_w, square_location) || get_bit(pos.bishop_b, square_location))
		return BISHOP;
	else if(get_bit(pos.rook_w, square_location) || get_bit(pos.rook_b, square_location))
		return ROOK;
	else if(get_bit(pos.queen_w, square_location) || get_bit(pos.queen_b, square_location))
		return QUEEN;
	else if(get_bit(pos.king_w, square_location) || get_bit(pos.king_b, square_location))
		return KING;
	return EMPTY;
}

/**
 * @brief 
 * Iterates through vec and sets the bits of the returned bitboard at the positions of vec[i] to 1
 * e.g.: if vec is {0, 3, 17, 62}, then the returned bitboard will have values 1 at the bits 0, 3, 17, and 62.
 * The returned bitboard should look like 1001000000000000010...010
 * @param vec vector of locations at which to set bits to 1
 */
bitboard map_bitboard(std::vector<int> vec)
{
	bitboard bb = 0ULL;
	for(int i = 0; i < vec.size(); ++i)
		set_bit(bb, 1, vec[i]);
	return bb;
}

/**
 * @brief
 * Returns a bitboard of all squares at which ALL pieces of a specific color control.
 * This can be useful for figuring out whether a king is allowed to move to a specific square.
 * @param pos position (board state)
 * @param piece_color
 * @return unsigned long long (bitboard)
*/
bitboard get_control_map(struct position pos, bool piece_color)
{
	bitboard map = 0ULL;
	for(int i = 0; i < 64; ++i)
	{
		if(get_bit((piece_color ? pos.pawn_b : pos.pawn_w), i))
			map = map | map_bitboard(get_moves(pos, i, PAWN, piece_color, true));
		else if(get_bit((piece_color ? pos.knight_b : pos.knight_w), i))
			map = map | map_bitboard(get_moves(pos, i, KNIGHT, piece_color, true));
		else if(get_bit((piece_color ? pos.bishop_b : pos.bishop_w), i))
			map = map | map_bitboard(get_moves(pos, i, BISHOP, piece_color, true));
		else if(get_bit((piece_color ? pos.rook_b : pos.rook_w), i))
			map = map | map_bitboard(get_moves(pos, i, ROOK, piece_color, true));
		else if(get_bit((piece_color ? pos.queen_b : pos.queen_w), i))
			map = map | map_bitboard(get_moves(pos, i, QUEEN, piece_color, true));
		else if(get_bit((piece_color ? pos.king_b : pos.king_w), i))
			map = map | map_bitboard(get_moves(pos, i, KING, piece_color, true));
	}
	return map;
}

/**
 @brief
 * Returns a vector of the locations of all squares a piece at the specified location can move to.
 * Note: since we can't store the type of each piece on a bitboard, you must specify which piece
 * type you are currently selecting. This function is used by the engine to predict possible moves.
 * @param pos position (board state)
 * @param piece_location where the piece is located on the bitboard (0-63)
 * @param piece_type tells the function which characteristics the selected piece has (PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING)
 * @param piece_color tells the function which piece color you are currently checking for
 * @param control if true, will return all squares that a piece controls, if false, returns squares to which the piece is allowed to move to
*/
std::vector<int> get_moves(struct position pos, uint8_t piece_location, uint8_t piece_type, bool piece_color, bool control)
{
	std::vector<int> legal_moves; // Vector of legal moves
	const uint8_t opposite_piece_color = !piece_color; // This is used to determine what types of pieces the selected piece is allowed to capture
	switch(piece_type)
	{
		// ======================================
		//
		//                 PAWN                  
		//
		// ======================================
		case PAWN:
			if(!control)
			{
				if(!piece_color) // White pawns
				{
					if(is_empty(pos, piece_location - 8) && piece_location > 7)
						legal_moves.push_back(piece_location - 8); // 1 square forward
					if(is_empty(pos, piece_location - 16) && is_empty(pos, piece_location - 8) && piece_location <= 55 && piece_location >= 48)
						legal_moves.push_back(piece_location - 16); // 2 squares forward (if the pawn is still on it's starting square)
					if(piece_color_at(pos, piece_location - 9, opposite_piece_color) && piece_location % 8 != 0)
						legal_moves.push_back(piece_location - 9); // Piece capture (left)
					if(piece_color_at(pos, piece_location - 7, opposite_piece_color) && (piece_location+1) % 8 != 0)
						legal_moves.push_back(piece_location - 7); // Piece capture (right)
				}
				else
				{
					if(is_empty(pos, piece_location + 8) && piece_location < 56)
						legal_moves.push_back(piece_location + 8); // 1 square forward
					if(is_empty(pos, piece_location + 16) && is_empty(pos, piece_location + 8) && piece_location <= 15 && piece_location >= 8)
						legal_moves.push_back(piece_location + 16); // 2 squares forward (if the pawn is still on it's starting square)
					if(piece_color_at(pos, piece_location + 7, opposite_piece_color) && piece_location % 8 != 0)
						legal_moves.push_back(piece_location + 7); // Piece capture (left)
					if(piece_color_at(pos, piece_location + 9, opposite_piece_color) && (piece_location+1) % 8 != 0)
						legal_moves.push_back(piece_location + 9); // Piece capture (right)
				}
			}
			else
			{
				if(!piece_color) // White pawns
				{
					if(piece_location % 8 != 0)
						legal_moves.push_back(piece_location - 9); // Piece capture (left)
					if((piece_location+1) % 8 != 0)
						legal_moves.push_back(piece_location - 7); // Piece capture (right)
				}
				else // Black pawns
				{
					if(piece_location % 8 != 0)
						legal_moves.push_back(piece_location + 7); // Piece capture (left)
					if((piece_location+1) % 8 != 0)
						legal_moves.push_back(piece_location + 9); // Piece capture (right)
				}
			}
		break;

		// ======================================
		//
		//                 KNIGHT                  
		//
		// ======================================
		case KNIGHT:
		if(!control)
		{
			if((is_empty(pos, piece_location - 15) || piece_color_at(pos, piece_location - 15, opposite_piece_color)) && (piece_location+1) % 8 != 0 && piece_location > 15)
				legal_moves.push_back(piece_location - 15); // 2 up, 1 right
			if((is_empty(pos, piece_location - 17) || piece_color_at(pos, piece_location - 17, opposite_piece_color)) && piece_location % 8 != 0 && piece_location > 15)
				legal_moves.push_back(piece_location - 17); // 2 up, 1 left
			if((is_empty(pos, piece_location - 6) || piece_color_at(pos, piece_location - 6, opposite_piece_color)) && (piece_location+2) % 8 != 0 && (piece_location+1) % 8 != 0 && piece_location > 7)
				legal_moves.push_back(piece_location - 6); // 1 up, 2 right
			if((is_empty(pos, piece_location - 10) || piece_color_at(pos, piece_location - 10, opposite_piece_color)) && piece_location % 8 != 0 && (piece_location-1) % 8 != 0 && piece_location > 7)
				legal_moves.push_back(piece_location - 10); // 1 up, 2 left
			if((is_empty(pos, piece_location + 10) || piece_color_at(pos, piece_location + 10, opposite_piece_color)) && (piece_location+2) % 8 != 0 && (piece_location+1) % 8 != 0 && piece_location < 56)
				legal_moves.push_back(piece_location + 10); // 1 down, 2 right
			if((is_empty(pos, piece_location + 6) || piece_color_at(pos, piece_location + 6, opposite_piece_color)) && (piece_location % 8) != 0 && (piece_location-1) % 8 != 0 && piece_location < 56)
				legal_moves.push_back(piece_location + 6); // 1 down, 2 left
			if((is_empty(pos, piece_location + 17) || piece_color_at(pos, piece_location + 17, opposite_piece_color)) && (piece_location+1) % 8 != 0 && piece_location < 48)
				legal_moves.push_back(piece_location + 17); // 2 down, 1 right
			if((is_empty(pos, piece_location + 15) || piece_color_at(pos, piece_location + 15, opposite_piece_color)) && (piece_location % 8) != 0 && piece_location < 48)
				legal_moves.push_back(piece_location + 15); // 2 down, 1 left
		}
		else
		{
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

		// ======================================
		//
		//                 BISHOP                  
		//
		// ======================================
		case BISHOP:
		if(!control)
		{
			// North-west
			if(piece_location > 7 && (piece_location % 8) != 0)
			{
				int i = piece_location - 9;
				while(1)
				{
					if(is_empty(pos, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(!is_empty(pos, i) || i < 8 || i % 8 == 0)
						break;
					i -= 9;
				}
			}
			// South-east
			if(piece_location < 56 && ((piece_location+1) % 8) != 0)
			{
				int i = piece_location + 9;
				while(1)
				{
					if(is_empty(pos, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(!is_empty(pos, i) || i >= 56 || (i+1) % 8 == 0)
						break;
					i += 9;
				}
			}
			// North-east
			if(piece_location > 7 && ((piece_location+1) % 8) != 0)
			{
				int i = piece_location - 7;
				while(1)
				{
					if(is_empty(pos, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(!is_empty(pos, i) || i < 8 || (i+1) % 8 == 0)
						break;
					i -= 7;
				}
			}

			// South-west
			if(piece_location < 56 && ((piece_location+1) % 8) != 0)
			{
				int i = piece_location + 7;
				while(1)
				{
					if(is_empty(pos, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(!is_empty(pos, i) || i < 8 || (i+1) % 8 == 0)
						break;
					i += 7;
				}
			}
		}
		else
		{
			// North-west
			if(piece_location > 7 && (piece_location % 8) != 0)
			{
				int i = piece_location - 9;
				while(1)
				{
					legal_moves.push_back(i);
					if(!is_empty(pos, i) || i < 8 || i % 8 == 0)
						break;
					i -= 9;
				}
			}
			// South-east
			if(piece_location < 56 && ((piece_location+1) % 8) != 0)
			{
				int i = piece_location + 9;
				while(1)
				{
					legal_moves.push_back(i);
					if(!is_empty(pos, i) || i >= 56 || (i+1) % 8 == 0)
						break;
					i += 9;
				}
			}
			// North-east
			if(piece_location > 7 && ((piece_location+1) % 8) != 0)
			{
				int i = piece_location - 7;
				while(1)
				{
					legal_moves.push_back(i);
					if(!is_empty(pos, i) || i < 8 || (i+1) % 8 == 0)
						break;
					i -= 7;
				}
			}

			// South-west
			if(piece_location < 56 && ((piece_location+1) % 8) != 0)
			{
				int i = piece_location + 7;
				while(1)
				{
					legal_moves.push_back(i);
					if(!is_empty(pos, i) || i < 8 || (i+1) % 8 == 0)
						break;
					i += 7;
				}
			}
		}
		break;

		// ======================================
		//
		//                THE ROOK                  
		//
		// ======================================
		case ROOK:
		if(!control)
		{
			// Up
			if(piece_location > 7)
			{
				for(int i = piece_location - 8; i > 7; i -= 8)
				{
					if(is_empty(pos, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(!is_empty(pos, i))
						break;
				}
			}
			// Down
			if(piece_location < 56)
			{
				for(int i = piece_location + 8; i < 56; i += 8)
				{
					if(is_empty(pos, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(!is_empty(pos, i))
						break;
				}
			}
			// Left
			if(piece_location % 8 != 0)
			{
				for(int i = piece_location - 1; i > -1; --i)
				{
					if(is_empty(pos, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(!is_empty(pos, i) || i % 8 == 0)
						break;
				}
			}
			// Right
			if((piece_location+1) % 8 != 0)
			{
				for(int i = piece_location + 1; i < 63; ++i)
				{
					if(is_empty(pos, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(!is_empty(pos, i) || (i+1) % 8 == 0)
						break;
				}
			}
		}
		else
		{
			// Up
			if(piece_location > 7)
			{
				for(int i = piece_location - 8; i > 7; i -= 8)
				{
					legal_moves.push_back(i);
					if(!is_empty(pos, i))
						break;
				}
			}
			// Down
			if(piece_location < 56)
			{
				for(int i = piece_location + 8; i < 56; i += 8)
				{
					legal_moves.push_back(i);
					if(!is_empty(pos, i))
						break;
				}
			}
			// Left
			if(piece_location % 8 != 0)
			{
				for(int i = piece_location - 1; i > -1; --i)
				{
					legal_moves.push_back(i);
					if(!is_empty(pos, i) || i % 8 == 0)
						break;
				}
			}
			// Right
			if((piece_location+1) % 8 != 0)
			{
				for(int i = piece_location + 1; i < 63; ++i)
				{
					legal_moves.push_back(i);
					if(!is_empty(pos, i) || (i+1) % 8 == 0)
						break;
				}
			}
		}
		break;

		// ======================================
		//
		//                 QUEEN                  
		//
		// ======================================
		case QUEEN:
		// Bishop movement (diagonal)
		// North-west
		if(piece_location > 7 && (piece_location % 8) != 0)
		{
			int i = piece_location - 9;
			while(1)
			{
				legal_moves.push_back(i);
				if(!is_empty(pos, i) || i < 8 || i % 8 == 0)
					break;
				i -= 9;
			}
		}
		// South-east
		if(piece_location < 56 && ((piece_location+1) % 8) != 0)
		{
			int i = piece_location + 9;
			while(1)
			{
				legal_moves.push_back(i);
				if(!is_empty(pos, i) || i >= 56 || (i+1) % 8 == 0)
					break;
				i += 9;
			}
		}
		// North-east
		if(piece_location > 7 && ((piece_location+1) % 8) != 0)
		{
			int i = piece_location - 7;
			while(1)
			{
				legal_moves.push_back(i);
				if(!is_empty(pos, i) || i < 8 || (i+1) % 8 == 0)
					break;
				i -= 7;
			}
		}

		// South-west
		if(piece_location < 56 && ((piece_location+1) % 8) != 0)
		{
			int i = piece_location + 7;
			while(1)
			{
				legal_moves.push_back(i);
				if(!is_empty(pos, i) || i < 8 || (i+1) % 8 == 0)
					break;
				i += 7;
			}
		}
		// Rook movement (horizontal+vertical)
		// Up
		if(piece_location > 7)
		{
			for(int i = piece_location - 8; i > 7; i -= 8)
			{
				legal_moves.push_back(i);
				if(!is_empty(pos, i))
					break;
			}
		}
		// Down
		if(piece_location < 56)
		{
			for(int i = piece_location + 8; i < 56; i += 8)
			{
				legal_moves.push_back(i);
				if(!is_empty(pos, i))
					break;
			}
		}
		// Left
		if(piece_location % 8 != 0)
		{
			for(int i = piece_location - 1; i > -1; --i)
			{
				legal_moves.push_back(i);
				if(!is_empty(pos, i) || i % 8 == 0)
					break;
			}
		}
		// Right
		if((piece_location+1) % 8 != 0)
		{
			for(int i = piece_location + 1; i < 63; ++i)
			{
				legal_moves.push_back(i);
				if(!is_empty(pos, i) || (i+1) % 8 == 0)
					break;
			}
		}
		break;

		// ======================================
		//
		//                 KING                  
		//
		// ======================================
		case KING:
		if(!control)
		{
			// Control map for enemy pieces
			// Used to map squares the king CANNOT move to
			bitboard control_map = get_control_map(pos, opposite_piece_color);

			// Loops in a way that creates a 3x3 area around the piece_location (ignores piece_locations itself since the king can't control it's own square)
			for(int i = piece_location - 9; i < piece_location + 9; i += 8)
				for(int j = 0; j < 3; ++j)
					if(get_bit(control_map, i+j) == 0 && i+j != piece_location && i+j >= 0 && i+j < 64)
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

/**
 * @brief Set the up position struct (puts pieces on their according squares)
 * @param pos 
 */
void setup_position(struct position& pos)
{
	// Clear each bitboard (since they are not initialized during definition, they are all randomized)
	pos.pawn_w = 0ULL;
	pos.pawn_b = 0ULL;
	pos.knight_w = 0ULL;
	pos.knight_b = 0ULL;
	pos.bishop_w = 0ULL;
	pos.bishop_b = 0ULL;
	pos.rook_w = 0ULL;
	pos.rook_b = 0ULL;
	pos.queen_w = 0ULL;
	pos.queen_b = 0ULL;
	pos.king_w = 0ULL;
	pos.king_b = 0ULL;

	// Pawns
	for(int i = 0; i < 8; ++i)
	{
		set_bit(pos.pawn_w, 1, i + 48); // Black pawns
		set_bit(pos.pawn_b, 1, i + 8); // Black pawns
	}

	// Knights
	// White knights
	set_bit(pos.knight_w, 1, 62);
	set_bit(pos.knight_w, 1, 57);
	// Black knights
	set_bit(pos.knight_b, 1, 6);
	set_bit(pos.knight_b, 1, 1);

	// Bishops
	// White bishops
	set_bit(pos.bishop_w, 1, 61);
	set_bit(pos.bishop_w, 1, 58);
	// Black bishops
	set_bit(pos.bishop_b, 1, 5);
	set_bit(pos.bishop_b, 1, 2);

	// THE ROOK
	// White rook
	set_bit(pos.rook_w, 1, 63);
	set_bit(pos.rook_w, 1, 56);
	// Black rook
	set_bit(pos.rook_b, 1, 7);
	set_bit(pos.rook_b, 1, 0);

	// Queen
	set_bit(pos.queen_w, 1, 59); // White queen
	set_bit(pos.queen_b, 1, 3); // Black queen

	// King
	set_bit(pos.king_w, 1, 60); // White king
	set_bit(pos.king_b, 1, 4); // Black king
}
} // namespace board
} // namespace NerdChess
