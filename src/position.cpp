/*
Here, position refers to a combination of (12) bitboards which define a specific
chess piece layout. This file contains all necessary function for working with
a position, such as retrieving piece types from squares and working with them.
*/

#include <iostream>
#include <vector>
#include "position.h"

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
int get_piece_type(struct position pos, uint8_t square_location)
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
	else
		return EMPTY;
}

/**
 * @brief Removes a piece from the board
 * 
 * @param pos 
 * @param location 
 */
void remove_piece(struct position& pos, int location)
{
	clear_bit(pos.pawn_w, location);
	clear_bit(pos.pawn_b, location);
	clear_bit(pos.knight_w, location);
	clear_bit(pos.knight_b, location);
	clear_bit(pos.bishop_w, location);
	clear_bit(pos.bishop_b, location);
	clear_bit(pos.rook_w, location);
	clear_bit(pos.rook_b, location);
	clear_bit(pos.queen_w, location);
	clear_bit(pos.queen_b, location);
	clear_bit(pos.king_w, location);
	clear_bit(pos.king_b, location);
}

/**
 * @brief Moves a piece from location to new_location
 * 
 * @param pos 
 * @param new_location 
 */
void move_piece(struct position& pos, int location, int new_location)
{
	uint8_t piece_type = get_piece_type(pos, location);
	bool piece_color = piece_color_at(pos, location, BLACK);

	remove_piece(pos, new_location); // Capture any pieces previously located at new_location

	switch(piece_type)
	{
		case PAWN:
		if(!piece_color)
		{
			// White pawn
			move_bit(pos.pawn_w, location, new_location);
			if((location - new_location) == 16)
				pos.en_pessant_squares[BLACK] = new_location + 8;

			// En pessant capture
			if(!piece_color_at(pos, new_location, BLACK))
				bitb::clear_bit(pos.pawn_b, new_location + 8);

			// Promote to queen
			if(new_location < 8)
			{
				remove_piece(pos, new_location);
				set_bit(pos.queen_w, new_location);
			}
		}
		else
		{
			// Black pawn
			move_bit(pos.pawn_b, location, new_location);
			if((new_location - location) == 16)
				pos.en_pessant_squares[WHITE] = new_location - 8;

			// En croissant capture
			if(!piece_color_at(pos, new_location, WHITE))
				bitb::clear_bit(pos.pawn_b, new_location - 8);

			// Promote to queen
			if(new_location > 55)
			{
				remove_piece(pos, new_location);
				set_bit(pos.queen_b, new_location);
			}
		}

		// Reset en pessant rule
		pos.en_pessant_squares[WHITE] = INT_MIN;
		pos.en_pessant_squares[BLACK] = INT_MIN;

		break;

		case KNIGHT:
		move_bit((piece_color ? pos.knight_b : pos.knight_w), location, new_location);
		break;

		case BISHOP:
		move_bit((piece_color ? pos.bishop_b : pos.bishop_w), location, new_location);
		break;

		case ROOK:
		move_bit((piece_color ? pos.rook_b : pos.rook_w), location, new_location);
		break;

		case QUEEN:
		move_bit((piece_color ? pos.queen_b : pos.queen_w), location, new_location);
		break;

		case KING:
		move_bit((piece_color ? pos.king_b : pos.king_w), location, new_location);
		break;
	}
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
		set_bit(bb, vec[i]);
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
 * @brief Maps all pieces in a position on a bitboard
 * 
 * @param pos 
 * @return bitboard 
 */
bitboard map_pieces(struct position pos)
{
    return pos.pawn_w | pos.pawn_b | pos.knight_w | pos.knight_b | pos.bishop_w | pos.bishop_b | pos.rook_w | pos.rook_b | pos.queen_w | pos.queen_b | pos.king_w | pos.king_b;
}

/**
 * @brief Returns the number of positive bits in a bitboard (ones)
 * 
 * @param bb 
 * @return Number of bits set to "1"
 */
int count_bits(bitboard bb)
{
	uint8_t num_pieces = 0;
	for(int i = 0; i < 64; ++i)
		if(get_bit(bb, i) == 1)
			num_pieces++;
	return num_pieces;
}

/**
 * @brief Returns the number of pieces on the board
 * 
 * @param pos 
 * @return Number of pieces 
 */
int count_pieces(struct position pos)
{
	bitboard pos_map = pos.pawn_w | pos.pawn_b | pos.knight_w | pos.knight_b | pos.bishop_w | pos.bishop_b | pos.rook_w | pos.rook_b | pos.queen_w | pos.queen_b | pos.king_w | pos.king_b;
	uint8_t num_pieces = 0;
	for(int i = 0; i < 64; ++i)
		if(get_bit(pos_map, i) == 1)
			num_pieces++;
	return num_pieces;
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
	bitboard piece_map = map_pieces(pos); // Squares on which there are pieces
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
					if(pos.en_pessant_squares[WHITE] >= 0 && ((piece_location - 9) == pos.en_pessant_squares[WHITE] || (piece_location - 7) == pos.en_pessant_squares[WHITE]))
						legal_moves.push_back(pos.en_pessant_squares[WHITE]); // En pessant
					if(!get_bit(piece_map, piece_location - 8) && piece_location > 7)
						legal_moves.push_back(piece_location - 8); // 1 square forward
					if(!get_bit(piece_map, piece_location - 16) && !get_bit(piece_map, piece_location - 8) && piece_location <= 55 && piece_location >= 48)
						legal_moves.push_back(piece_location - 16); // 2 squares forward (if the pawn is still on it's starting square)
					if(piece_color_at(pos, piece_location - 9, opposite_piece_color) && piece_location % 8 != 0)
						legal_moves.push_back(piece_location - 9); // Piece capture (left)
					if(piece_color_at(pos, piece_location - 7, opposite_piece_color) && (piece_location+1) % 8 != 0)
						legal_moves.push_back(piece_location - 7); // Piece capture (right)
				}
				else // Black pawns
				{
					if(pos.en_pessant_squares[BLACK] >= 0 && ((piece_location + 9) == pos.en_pessant_squares[BLACK] || (piece_location + 7) == pos.en_pessant_squares[BLACK]))
						legal_moves.push_back(pos.en_pessant_squares[BLACK]); // En pessant
					if(get_bit(piece_map, piece_location + 8) == 0 && piece_location < 56)
						legal_moves.push_back(piece_location + 8); // 1 square forward
					if(get_bit(piece_map, piece_location + 16) == 0 && get_bit(piece_map, piece_location + 8) == 0 && piece_location <= 15 && piece_location >= 8)
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
			if((!get_bit(piece_map, piece_location - 15) || piece_color_at(pos, piece_location - 15, opposite_piece_color)) && (piece_location+1) % 8 != 0 && piece_location > 15)
				legal_moves.push_back(piece_location - 15); // 2 up, 1 right
			if((!get_bit(piece_map, piece_location - 17) || piece_color_at(pos, piece_location - 17, opposite_piece_color)) && piece_location % 8 != 0 && piece_location > 15)
				legal_moves.push_back(piece_location - 17); // 2 up, 1 left
			if((!get_bit(piece_map, piece_location - 6) || piece_color_at(pos, piece_location - 6, opposite_piece_color)) && (piece_location+2) % 8 != 0 && (piece_location+1) % 8 != 0 && piece_location > 7)
				legal_moves.push_back(piece_location - 6); // 1 up, 2 right
			if((!get_bit(piece_map, piece_location - 10) || piece_color_at(pos, piece_location - 10, opposite_piece_color)) && piece_location % 8 != 0 && (piece_location-1) % 8 != 0 && piece_location > 7)
				legal_moves.push_back(piece_location - 10); // 1 up, 2 left
			if((!get_bit(piece_map, piece_location + 10) || piece_color_at(pos, piece_location + 10, opposite_piece_color)) && (piece_location+2) % 8 != 0 && (piece_location+1) % 8 != 0 && piece_location < 56)
				legal_moves.push_back(piece_location + 10); // 1 down, 2 right
			if((!get_bit(piece_map, piece_location + 6) || piece_color_at(pos, piece_location + 6, opposite_piece_color)) && (piece_location % 8) != 0 && (piece_location-1) % 8 != 0 && piece_location < 56)
				legal_moves.push_back(piece_location + 6); // 1 down, 2 left
			if((!get_bit(piece_map, piece_location + 17) || piece_color_at(pos, piece_location + 17, opposite_piece_color)) && (piece_location+1) % 8 != 0 && piece_location < 48)
				legal_moves.push_back(piece_location + 17); // 2 down, 1 right
			if((!get_bit(piece_map, piece_location + 15) || piece_color_at(pos, piece_location + 15, opposite_piece_color)) && (piece_location % 8) != 0 && piece_location < 48)
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
					if(!get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(get_bit(piece_map, i) || i < 8 || i % 8 == 0)
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
					if(!get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(get_bit(piece_map, i) || i >= 56 || (i+1) % 8 == 0)
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
					if(!get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(get_bit(piece_map, i) || i < 8 || (i+1) % 8 == 0)
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
					if(!get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(get_bit(piece_map, i) || i % 8 == 0 || i >= 56)
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
					if(get_bit(piece_map, i) || i < 8 || i % 8 == 0)
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
					if(get_bit(piece_map, i) || i >= 56 || (i+1) % 8 == 0)
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
					if(get_bit(piece_map, i) || i < 8 || (i+1) % 8 == 0)
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
					if(get_bit(piece_map, i) || i < 8 || i % 8 == 0)
						break;
					i += 7;
				}
			}
		}
		break;

		// ======================================
		//
		//                THE ROOK!                  
		//
		// ======================================
		case ROOK:
		if(!control)
		{
			// Up
			if(piece_location > 7)
			{
				for(int i = piece_location - 8; i > -1; i -= 8)
				{
					if(!get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(get_bit(piece_map, i))
						break;
				}
			}
			// Down
			if(piece_location < 56)
			{
				for(int i = piece_location + 8; i < 56; i += 8)
				{
					if(!get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(get_bit(piece_map, i))
						break;
				}
			}
			// Left
			if(piece_location % 8 != 0)
			{
				for(int i = piece_location - 1; i > -1; --i)
				{
					if(!get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(get_bit(piece_map, i) || i % 8 == 0)
						break;
				}
			}
			// Right
			if((piece_location+1) % 8 != 0)
			{
				for(int i = piece_location + 1; i < 63; ++i)
				{
					if(!get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(get_bit(piece_map, i) || (i+1) % 8 == 0)
						break;
				}
			}
		}
		else
		{
			// Up
			if(piece_location > 7)
			{
				for(int i = piece_location - 8; i > -1; i -= 8)
				{
					legal_moves.push_back(i);
					if(get_bit(piece_map, i))
						break;
				}
			}
			// Down
			if(piece_location < 56)
			{
				for(int i = piece_location + 8; i < 56; i += 8)
				{
					legal_moves.push_back(i);
					if(get_bit(piece_map, i))
						break;
				}
			}
			// Left
			if(piece_location % 8 != 0)
			{
				for(int i = piece_location - 1; i > -1; --i)
				{
					legal_moves.push_back(i);
					if(get_bit(piece_map, i) || i % 8 == 0)
						break;
				}
			}
			// Right
			if((piece_location+1) % 8 != 0)
			{
				for(int i = piece_location + 1; i < 63; ++i)
				{
					legal_moves.push_back(i);
					if(get_bit(piece_map, i) || (i+1) % 8 == 0)
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
		if(!control)
		{
			// ROOK MOVEMENT
			// Up
			if(piece_location > 7)
			{
				for(int i = piece_location - 8; i > -1; i -= 8)
				{
					if(!get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(get_bit(piece_map, i))
						break;
				}
			}
			// Down
			if(piece_location < 56)
			{
				for(int i = piece_location + 8; i < 56; i += 8)
				{
					if(!get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(get_bit(piece_map, i))
						break;
				}
			}
			// Left
			if(piece_location % 8 != 0)
			{
				for(int i = piece_location - 1; i > -1; --i)
				{
					if(!get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(get_bit(piece_map, i) || i % 8 == 0)
						break;
				}
			}
			// Right
			if((piece_location+1) % 8 != 0)
			{
				for(int i = piece_location + 1; i < 63; ++i)
				{
					if(!get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(get_bit(piece_map, i) || (i+1) % 8 == 0)
						break;
				}
			}

			// BISHOP MOVEMENT
			// North-west
			if(piece_location > 7 && (piece_location % 8) != 0)
			{
				int i = piece_location - 9;
				while(1)
				{
					if(!get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(get_bit(piece_map, i) || i < 8 || i % 8 == 0)
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
					if(!get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(get_bit(piece_map, i) || i >= 56 || (i+1) % 8 == 0)
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
					if(!get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(get_bit(piece_map, i) || i < 8 || (i+1) % 8 == 0)
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
					if(!get_bit(piece_map, i) || piece_color_at(pos, i, opposite_piece_color))
						legal_moves.push_back(i);
					if(get_bit(piece_map, i) || i % 8 != 0 || i >= 56)
						break;
					i += 7;
				}
			}
		}
		else
		{
			// ROOK MOVEMENT
			// Up
			if(piece_location > 7)
			{
				for(int i = piece_location - 8; i > -1; i -= 8)
				{
					legal_moves.push_back(i);
					if(get_bit(piece_map, i))
						break;
				}
			}
			// Down
			if(piece_location < 56)
			{
				for(int i = piece_location + 8; i < 56; i += 8)
				{
					legal_moves.push_back(i);
					if(get_bit(piece_map, i))
						break;
				}
			}
			// Left
			if(piece_location % 8 != 0)
			{
				for(int i = piece_location - 1; i > -1; --i)
				{
					legal_moves.push_back(i);
					if(get_bit(piece_map, i) || i % 8 == 0)
						break;
				}
			}
			// Right
			if((piece_location+1) % 8 != 0)
			{
				for(int i = piece_location + 1; i < 63; ++i)
				{
					legal_moves.push_back(i);
					if(get_bit(piece_map, i) || (i+1) % 8 == 0)
						break;
				}
			}

			// BISHOP MOVEMENT
			// North-west
			if(piece_location > 7 && (piece_location % 8) != 0)
			{
				int i = piece_location - 9;
				while(1)
				{
					legal_moves.push_back(i);
					if(get_bit(piece_map, i) || i < 8 || i % 8 == 0)
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
					if(get_bit(piece_map, i) || i >= 56 || (i+1) % 8 == 0)
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
					if(get_bit(piece_map, i) || i < 8 || (i+1) % 8 == 0)
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
					if(get_bit(piece_map, i) || i < 8 || i % 8 != 0)
						break;
					i += 7;
				}
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
	pos.castling_rights[WHITE] = true;
	pos.castling_rights[BLACK] = true;

	pos.en_pessant_squares[WHITE] = INT_MIN;
	pos.en_pessant_squares[BLACK] = INT_MIN;

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
		set_bit(pos.pawn_w, i + 48); // Black pawns
		set_bit(pos.pawn_b, i + 8); // Black pawns
	}

	// Knights
	// White knights
	set_bit(pos.knight_w, 62);
	set_bit(pos.knight_w, 57);
	// Black knights
	set_bit(pos.knight_b, 6);
	set_bit(pos.knight_b, 1);

	// Bishops
	// White bishops
	set_bit(pos.bishop_w, 61);
	set_bit(pos.bishop_w, 58);
	// Black bishops
	set_bit(pos.bishop_b, 5);
	set_bit(pos.bishop_b, 2);

	// THE ROOK
	// White rook
	set_bit(pos.rook_w, 63);
	set_bit(pos.rook_w, 56);
	// Black rook
	set_bit(pos.rook_b, 7);
	set_bit(pos.rook_b, 0);

	// Queen
	set_bit(pos.queen_w, 59); // White queen
	set_bit(pos.queen_b, 3); // Black queen

	// King
	set_bit(pos.king_w, 60); // White king
	set_bit(pos.king_b, 4); // Black king
}

/**
 * @brief Returns the location of a piece on a bitboard. Returns -1 if piece was not found. This function is specifically optimized to work for single pieces, will return last occurence if multiple pieces are pesent on the bitboard.
 * 
 * @param bb 
 * @return int 
 */
int find_piece(bitboard bb)
{
	bitboard b = bb;
	for(int i = 0; i < 64; ++i)
	{
		if(b == 1ULL) return i+1;
		b = b >> 1;
	}
	return -1;
}

// Debugging
namespace debug
{
// Prints out a vector with a JavaScript-like format.
// Example output:
// [1, 143, 92]
void print_vec(std::vector<int> vec)
{
	if(vec.size() >= 1)
	{
		std::cout << "[";
		for(int i = 0; i < vec.size()-1; ++i)
			std::cout << vec[i] << ", ";
		std::cout << vec[vec.size()-1] << "]\n";
	}
	else
		std::cout << "[]\n";
}

// Prints out the entire collection of bitboards as one board
void print_board(struct position pos)
{
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
		if(bitb::get_bit(pos.pawn_w, j + i*8) == 1)
			std::cout << "\033[97mp ";
		else if(bitb::get_bit(pos.pawn_b, j + i*8) == 1)
			std::cout << "\033[90mp ";
		else if(bitb::get_bit(pos.knight_w, j + i*8) == 1)
			std::cout << "\033[97mN ";
		else if(bitb::get_bit(pos.knight_b, j + i*8) == 1)
			std::cout << "\033[90mN ";
		else if(bitb::get_bit(pos.bishop_w, j + i*8) == 1)
			std::cout << "\033[97mB ";
		else if(bitb::get_bit(pos.bishop_b, j + i*8) == 1)
			std::cout << "\033[90mB ";
		else if(bitb::get_bit(pos.rook_w, j + i*8) == 1)
			std::cout << "\033[97mR ";
		else if(bitb::get_bit(pos.rook_b, j + i*8) == 1)
			std::cout << "\033[90mR ";
		else if(bitb::get_bit(pos.queen_w, j + i*8) == 1)
			std::cout << "\033[97mQ ";
		else if(bitb::get_bit(pos.queen_b, j + i*8) == 1)
			std::cout << "\033[90mQ ";
		else if(bitb::get_bit(pos.king_w, j + i*8) == 1)
			std::cout << "\033[97mK ";
		else if(bitb::get_bit(pos.king_b, j + i*8) == 1)
			std::cout << "\033[90mK ";
		else
			std::cout << "\033[90m. ";
		}
		std::cout << "\n";
	}
	std::cout << "\033[97m\n";
}
} // namespace debug
} // namespace board
} // namespace NerdChess
