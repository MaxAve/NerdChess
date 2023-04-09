/*
Here, position refers to a combination of (14) bitboards which define a specific
chess piece layout. This file contains all necessary function for working with
a position.
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
// Returns true if the bit at position 'square' of EVERY bitboard is 0 (empty)
bool is_empty(struct position pos, uint8_t square_location)
{
    return !(get_bit(pos.pawn_w, square_location) | get_bit(pos.pawn_b, square_location) | get_bit(pos.knight_w, square_location) | get_bit(pos.knight_b, square_location) | get_bit(pos.bishop_w, square_location) | get_bit(pos.bishop_b, square_location) | get_bit(pos.rook_w, square_location) | get_bit(pos.rook_b, square_location) | get_bit(pos.queen_w, square_location) | get_bit(pos.queen_b, square_location) | get_bit(pos.king_w, square_location) | get_bit(pos.king_b, square_location));
}

// Is quite similar to is_empty() but instead of returning whether a square
// is empty or not, piece_color_at() returns whether a piece of a specific
// color (white/black) is on the square.
bool piece_color_at(struct position pos, uint8_t square_location, uint8_t piece_color)
{
    return (piece_color == WHITE ? (get_bit(pos.pawn_w, square_location) | get_bit(pos.knight_w, square_location) | get_bit(pos.bishop_w, square_location) | get_bit(pos.rook_w, square_location) | get_bit(pos.queen_w, square_location) | get_bit(pos.king_w, square_location)) : (get_bit(pos.pawn_b, square_location) | get_bit(pos.knight_b, square_location) | get_bit(pos.bishop_b, square_location) | get_bit(pos.rook_b, square_location) | get_bit(pos.queen_b, square_location) | get_bit(pos.king_b, square_location)));
}

// Returns a vector of the locations of all squares a piece at the specified location can move to
// Note: since we can't store the type of each piece on a bitboard, you must specify which piece
// type you are currently selecting.
std::vector<int> get_moves(struct position pos, uint8_t piece_location, uint8_t piece_type)
{
    std::vector<int> legal_moves; // Vector of legal moves
    switch(piece_type)
    {
        case PAWN_W: // White pawn
        if(is_empty(pos, piece_location - 8) && piece_location > 7)
            legal_moves.push_back(piece_location - 8); // 1 square forward
        if(is_empty(pos, piece_location - 16) && is_empty(pos, piece_location - 8) && piece_location <= 55 && piece_location >= 48)
            legal_moves.push_back(piece_location - 16); // 2 squares forward (if the pawn is still on it's starting square)
        if(piece_color_at(pos, piece_location - 9, BLACK) && piece_location % 8 != 0)
            legal_moves.push_back(piece_location - 9); // Piece capture (left)
        if(piece_color_at(pos, piece_location - 7, BLACK) && piece_location+1 % 8 != 0)
            legal_moves.push_back(piece_location - 9); // Piece capture (right)
        break;

        case PAWN_B: // Black pawn
        if(is_empty(pos, piece_location + 8) && piece_location < 56)
            legal_moves.push_back(piece_location + 8); // 1 square forward
        if(is_empty(pos, piece_location + 16) && is_empty(pos, piece_location + 8) && piece_location <= 15 && piece_location >= 8)
            legal_moves.push_back(piece_location + 16); // 2 squares forward (if the pawn is still on it's starting square)
        if(piece_color_at(pos, piece_location + 9, WHITE) && piece_location+1 % 8 != 0)
            legal_moves.push_back(piece_location + 9); // Piece capture (right)
        if(piece_color_at(pos, piece_location + 7, WHITE) && piece_location % 8 != 0)
            legal_moves.push_back(piece_location + 7); // Piece capture (left)
        break;

		case KNIGHT_W: // White knight
        if((is_empty(pos, piece_location - 15) || piece_color_at(pos, piece_location - 15, BLACK)) && piece_location+1 % 8 != 0 && piece_location > 15)
            legal_moves.push_back(piece_location - 15); // 2 up, 1 right
		if((is_empty(pos, piece_location - 17) || piece_color_at(pos, piece_location - 17, BLACK)) && piece_location % 8 != 0 && piece_location > 15)
            legal_moves.push_back(piece_location - 17); // 2 up, 1 left
		if((is_empty(pos, piece_location - 6) || piece_color_at(pos, piece_location - 6, BLACK)) && piece_location+2 % 8 != 0 && piece_location+1 % 8 != 0 && piece_location > 7)
            legal_moves.push_back(piece_location - 6); // 1 up, 2 right
		if((is_empty(pos, piece_location - 10) || piece_color_at(pos, piece_location - 10, BLACK)) && piece_location % 8 != 0 && piece_location-1 % 8 != 0 && piece_location > 7)
            legal_moves.push_back(piece_location - 10); // 1 up, 2 left
		if((is_empty(pos, piece_location + 10) || piece_color_at(pos, piece_location + 10, BLACK)) && piece_location+2 % 8 != 0 && piece_location+1 % 8 != 0 && piece_location < 56)
            legal_moves.push_back(piece_location + 10); // 1 down, 2 right
		if((is_empty(pos, piece_location + 6) || piece_color_at(pos, piece_location + 6, BLACK)) && piece_location % 8 != 0 && piece_location-1 % 8 != 0 && piece_location < 56)
            legal_moves.push_back(piece_location + 6); // 1 down, 2 left
		if((is_empty(pos, piece_location + 17) || piece_color_at(pos, piece_location + 17, BLACK)) && piece_location+1 % 8 != 0 && piece_location < 48)
            legal_moves.push_back(piece_location + 17); // 2 down, 1 right
		if((is_empty(pos, piece_location + 15) || piece_color_at(pos, piece_location + 15, BLACK)) && piece_location % 8 != 0 && piece_location < 48)
            legal_moves.push_back(piece_location + 15); // 2 down, 1 left
		break;

		case KNIGHT_B: // Black knight
        if((is_empty(pos, piece_location - 15) || piece_color_at(pos, piece_location - 15, WHITE)) && piece_location+1 % 8 != 0 && piece_location > 15)
            legal_moves.push_back(piece_location - 15); // 2 up, 1 right
		if((is_empty(pos, piece_location - 17) || piece_color_at(pos, piece_location - 17, WHITE)) && piece_location % 8 != 0 && piece_location > 15)
            legal_moves.push_back(piece_location - 17); // 2 up, 1 left
		if((is_empty(pos, piece_location - 6) || piece_color_at(pos, piece_location - 6, WHITE)) && piece_location+2 % 8 != 0 && piece_location+1 % 8 != 0 && piece_location > 7)
            legal_moves.push_back(piece_location - 6); // 1 up, 2 right
		if((is_empty(pos, piece_location - 10) || piece_color_at(pos, piece_location - 10, WHITE)) && piece_location % 8 != 0 && piece_location-1 % 8 != 0 && piece_location > 7)
            legal_moves.push_back(piece_location - 10); // 1 up, 2 left
		if((is_empty(pos, piece_location + 10) || piece_color_at(pos, piece_location + 10, WHITE)) && piece_location+2 % 8 != 0 && piece_location+1 % 8 != 0 && piece_location < 56)
            legal_moves.push_back(piece_location + 10); // 1 down, 2 right
		if((is_empty(pos, piece_location + 6) || piece_color_at(pos, piece_location + 6, WHITE)) && piece_location % 8 != 0 && piece_location-1 % 8 != 0 && piece_location < 56)
            legal_moves.push_back(piece_location + 6); // 1 down, 2 left
		if((is_empty(pos, piece_location + 17) || piece_color_at(pos, piece_location + 17, WHITE)) && piece_location+1 % 8 != 0 && piece_location < 48)
            legal_moves.push_back(piece_location + 17); // 2 down, 1 right
		if((is_empty(pos, piece_location + 15) || piece_color_at(pos, piece_location + 15, WHITE)) && piece_location % 8 != 0 && piece_location < 48)
            legal_moves.push_back(piece_location + 15); // 2 down, 1 left
        break;
    }
    return legal_moves;
}

// Sets up pieces (e.g. pawns across the 2nd and 7th ranks, rooks in the corners, etc.)
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
