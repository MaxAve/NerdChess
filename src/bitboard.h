/*
A bitboard is a 64 bit-long variable that represents a specific position.
0 means that the square is empty, 1 means that a piece is on that square.
Since we only have 2 possible values - 0 and 1, we need 7 bitboards for each
of the 7 pieces for each team. That makes a total of 14 bitboards for a
single position. Bitboards take up more RAM but should enable faster
computation.
*/

#include <iostream>

namespace NerdChess
{
namespace bitb
{
// Type for bitboard
// When declaring or initializing a bitboard, use the ULL suffix after the position number.
// Adding ULL allows us to access all 64 bits. Avoiding the ULL suffix will cause the program
// to believe that the bitboard is actually 32 bits in total.
typedef uint64_t bitboard;

/**
 * @brief Get the bit at location pos
 * 
 * @param bb bitboard
 * @param pos bit location
 * @return true 
 * @return false 
 */
inline bool get_bit(bitboard bb, int pos)
{
    return (bb >> pos) & 1ULL;
}

/**
 * @brief Set a single bit at position pos in the bitboard to 1
 * 
 * @param bb bitboard reference
 * @param pos bit position to set
 */
inline void set_bit(bitboard& bb, int pos)
{
    bb |= (1ULL << pos);
}

/**
 * @brief Set a single bit at position pos in the bitboard to 0
 * 
 * @param bb bitboard reference
 * @param pos bit position to set
 */
inline void clear_bit(bitboard& bb, int pos)
{
    bb &= ~(1ULL << pos);
}

void move_bit(bitboard& bb, int from, int to);
void print_bitboard(bitboard bb);
} // namespace bitb
} // namespace NerdChess
