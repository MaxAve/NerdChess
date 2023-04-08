/*
A bitboard is a 64 bit-long variable that represents a specific position.
0 means that the square is empty, 1 means that a piece is on that square.
Since we only have 2 possible values - 0 and 1, we need 7 bitboards for each
of the 7 pieces for each team. That makes a total of 14 bitboards for a
single position. Bitboards take up more RAM but should enable faster
computation.
*/

#include <iostream>
#include "bitboard.h"

namespace NerdChess
{
namespace bitb
{
// Returns the bit value (1 or 0) at the specified position in a bitboard
int get_bit(bitboard bb, int pos) { return (bb >> pos) & 1ULL; }

// Sets a bit at the specified position in a referenced bitboard
void set_bit(bitboard& bb, bool bitval, int pos) { bb |= 1ULL << pos; }

// Prints a bitboard
void print_bitboard(bitboard bb)
{
	for(int i = 0; i < 8; ++i)
	{
		for(int j = 0; j < 8; ++j)
			std::cout << get_bit(bb, j + i * 8) << " ";
		std::cout << "\n";
	}
}
} // namespace bitb
} // namespace NerdChess
