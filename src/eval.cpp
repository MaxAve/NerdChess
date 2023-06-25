#include <iostream>
#include <math.h>
#include "eval.h"

int NerdChess::board_control_value_map_w[64] = {0};
int NerdChess::board_control_value_map_b[64] = {0};

void NerdChess::generate_board_control_value_map(int* buf, bool piece_color) {
    if(!piece_color) {
        // For white pieces
        for(int i = 0; i < 8; ++i) {
            for(int j = 0; j < 8; ++j) {
                buf[i*8+j] = (int)pow(sqrt(8-i)*2 + (5 - sqrt(pow(j - 3.5, 2) + pow(i - 3, 2))), 2);
            }
        }
    } else {
        // For black pieces
        for(int i = 0; i < 8; ++i) {
            for(int j = 0; j < 8; ++j) {
                buf[63 - (i*8+j)] = (int)pow(sqrt(8-i)*2 + (5 - sqrt(pow(j - 3.5, 2) + pow(i - 3, 2))), 2);
            }
        }
    }
}

int NerdChess::eval::get_winner(struct board::position pos) {
    const int wKLocation = NerdChess::board::find_piece(pos.pieces[KING]);
    const int bKLocation = NerdChess::board::find_piece(pos.pieces[KING+_BLACK]);
    const bitboard wControlMap = NerdChess::board::get_control_map(pos, WHITE);
    const bitboard bControlMap = NerdChess::board::get_control_map(pos, BLACK);

    if(wKLocation == -1) {
        return WINNER_BLACK;
    } else if(bKLocation == -1) {
        return WINNER_WHITE;
    }

    if(NerdChess::bitb::get_bit(bControlMap, wKLocation) && NerdChess::board::get_moves(pos, wKLocation, KING, WHITE, false).size() == 0) {
        return WINNER_BLACK;
    } else if(NerdChess::bitb::get_bit(wControlMap, bKLocation) && NerdChess::board::get_moves(pos, bKLocation, KING, BLACK, false).size() == 0) {
        return WINNER_WHITE;
    }

    return WINNER_NONE;
}

int NerdChess::eval::eval_material(struct NerdChess::board::position pos, int piece_map[]) {
    int eval = 0;
    const NerdChess::bitb::bitboard _piece_map = board::map_pieces(pos);
    const int piece_values[] = {
        PAWN_VALUE,
        KNIGHT_VALUE,
        BISHOP_VALUE,
        ROOK_VALUE,
        QUEEN_VALUE,
        1000000000,
        -PAWN_VALUE,
        -KNIGHT_VALUE,
        -BISHOP_VALUE,
        -ROOK_VALUE,
        -QUEEN_VALUE,
        -1000000000
    };

    for(int i = 0; i < 64; ++i) {
        if(NerdChess::bitb::get_bit(_piece_map, i)) {
            eval += piece_values[piece_map[i]];
        }
    }
    return eval;
}

int NerdChess::eval::eval_structure(struct NerdChess::board::position board, int piece_map[]) {
    int eval = 0;

    for(int i = 0; i < 64; ++i) {
        switch(piece_map[i]) {
            case EMPTY:
                break;

            case PAWN: {
                // Pawns in the center
                if(NEAR_CENTER(i)) {
                    eval += 10;
                    if(IN_CENTER(i))
                        eval += 10;
                }
				// Pawns in the enemy territory
				if(GET_RANK(i) > 3)
					eval += 8;
                break;
            }

            case PAWN+_BLACK: {
                // Pawns in the center
                if(NEAR_CENTER(i)) {
                    eval -= 10;
                    if(IN_CENTER(i))
                        eval -= 10;
                }
				// Pawns in the enemy territory
				if(GET_RANK(i) > 3)
					eval += 8;
                break;
            }

            case KNIGHT: {
                // Knights should be placed in or near the center
                if(NEAR_CENTER(i))
                    eval += 15;
                break;
            }

            case KNIGHT+_BLACK: {
                // Knights should be placed in or near the center
                if(NEAR_CENTER(i))
                    eval -= 15;
                break;
            }

            case QUEEN: {
                // Queens should not be in the center
                if(NEAR_CENTER(i))
                    eval -= 13;
                break;
            }

            case QUEEN+_BLACK: {
                // Queens should not be in the center
                if(NEAR_CENTER(i))
                    eval += 13;
                break;
            }

            case KING: {
                // King safely in the corner
                if(i == 56 || i == 57 || i == 63 || i == 62)
                    eval += 20;
                break;
            }

            case KING+_BLACK: {
                // King safely in the corner
                if(i == 0 || i == 1 || i == 6 || i == 7)
                    eval -= 20;
                break;
            }
        }
    }

    return eval;
}

int NerdChess::eval::middlegame::eval_board_control(struct NerdChess::board::position pos, bool piece_color) {
    int eval = 0;
    const int* value_map = piece_color ? board_control_value_map_b : board_control_value_map_w;
    const NerdChess::bitb::bitboard control_map = board::get_control_map(pos, piece_color);
    for(int i = 0; i < 64; ++i)
        if(get_bit(control_map, i) == 1)
            eval += value_map[i] / 5; // Increases the evaluation based on the value of the squares controlled
    return eval;
}

int NerdChess::eval::eval_position(struct board::position pos) {
    int eval = 0;

    int piece_type_map[64];
    for(int i = 0; i < 64; ++i)
        piece_type_map[i] = NerdChess::board::get_full_piece_type(pos, i);

    eval += eval_material(pos, piece_type_map); // Material
    eval += (middlegame::eval_board_control(pos, WHITE) - middlegame::eval_board_control(pos, BLACK)); // Board control
    eval += eval_structure(pos, piece_type_map); // Piece structure

    return eval;
}
