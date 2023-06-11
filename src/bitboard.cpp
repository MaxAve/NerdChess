/*
A bitboard is a 64 bit-long variable that represents a specific position.
0 means that the square is empty, 1 means that a piece is on that square.
Since we only have 2 possible values - 0 and 1, we need 7 bitboards for each
of the 7 pieces for each team. That makes a total of 14 bitboards for a
single position. Bitboards take up more RAM but should enable faster
computation.
*/

#include <iostream>
#include "headers/bitboard.h"

namespace NerdChess
{
namespace bitb
{
void move_bit(bitboard& bb, int from, int to)
{
	clear_bit(bb, from);
	set_bit(bb, to);
}

// Prints a bitboard
void print_bitboard(bitboard bb)
{
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
		{
			if(get_bit(bb, j + i*8) == 1)
				std::cout << "\033[97m" << get_bit(bb, j + i * 8) << " ";
			else
				std::cout << "\033[90m" << get_bit(bb, j + i * 8) << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\033[97m";
}
} // namespace bitb
} // namespace NerdChess
