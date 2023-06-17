#include <iostream>
#include "include/bitboard.h"

void NerdChess::bitb::move_bit(bitboard& bb, int from, int to) {
	clear_bit(bb, from);
	set_bit(bb, to);
}

// Prints a bitboard
void NerdChess::bitb::print_bitboard(bitboard bb) {
	for(int i = 0; i < 8; ++i) {
		for(int j = 0; j < 8; ++j) {
			if(get_bit(bb, j + i*8) == 1)
				std::cout << "\033[97m" << get_bit(bb, j + i * 8) << " ";
			else
				std::cout << "\033[90m" << get_bit(bb, j + i * 8) << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\033[97m";
}
