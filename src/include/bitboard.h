#ifndef BITBOARD_H
#define BITBOARD_H

#include <iostream>

#define SQUARE(x)((x)*(x))

namespace NerdChess
{
namespace bitb
{
// Type for bitboard
// When declaring or initializing a bitboard, use the ULL suffix after the position number.
// Adding ULL allows us to access all 64 bits. Avoiding the ULL suffix will cause the program
// to believe that the bitboard is actually 32 bits in total.
typedef uint64_t bitboard;

inline bool get_bit(bitboard bb, int pos)
{
    return (bb >> pos) & 1ULL;
}

inline void set_bit(bitboard& bb, int pos)
{
    bb |= (1ULL << pos);
}

inline void clear_bit(bitboard& bb, int pos)
{
    bb &= ~(1ULL << pos);
}

void move_bit(bitboard& bb, int from, int to);
void print_bitboard(bitboard bb);
} // namespace bitb
} // namespace NerdChess

#endif
