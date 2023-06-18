#include <iostream>
#include "opening.h"

std::unordered_map<std::string, std::vector<int>> NerdChess::opening::book;

// Initialize opening book
// P.S: I don't care what you think about this. It works.
void NerdChess::opening::init_opening_book() {
    // e4
    book["rnbqkbnrpppppppp................................PPPPPPPPRNBQKBNR"] = std::vector<int>{52, 36}; // 1. e4
    book["rnbqkbnrpppppppp....................P...........PPPP.PPPRNBQKBNR"] = std::vector<int>{12, 28}; // 1. e4 e5
    book["rnbqkbnrpppp.ppp............p.......P...........PPPP.PPPRNBQKBNR"] = std::vector<int>{62, 45}; // 1. e4 e5; 2. Nf3
    book["rnbqkbnrpppp.ppp............p.......P........N..PPPP.PPPRNBQKB.R"] = std::vector<int>{1, 18}; // 1. e4 e5; 2. Nf3 Nc6
    book["r.bqkbnrpppp.ppp..n.........p.......P........N..PPPP.PPPRNBQKB.R"] = std::vector<int>{61, 34}; // 1. e4 e5; 2. Nf3 Nc6; 3. Bc4
    book["r.bqkbnrpppp.ppp..n.........p.....B.P........N..PPPP.PPPRNBQK..R"] = std::vector<int>{6, 21}; // 1. e4 e5; 2. Nf3 Nc6; 3. Bc4 Nf6

    // d4
    book["rnbqkbnrpppppppp...................P............PPP.PPPPRNBQKBNR"] = std::vector<int>{11, 27}; // 1. d4 d5
    // Queen's gambit and Nf3 are both very promising moves
    if(rand() % 2 == 0) {
        // Queen's gambit
        book["rnbqkbnrppp.pppp...........p.......P............PPP.PPPPRNBQKBNR"] = std::vector<int>{50, 34}; // 1. d4 d5; 2. c4
    } else {
        // Nf3
        book["rnbqkbnrppp.pppp...........p.......P............PPP.PPPPRNBQKBNR"] = std::vector<int>{62, 45}; // 1. d4 d5; 2. Nf3
    }

    book["rnbqkbnrppp.pppp...........p......PP............PP..PPPPRNBQKBNR"] = std::vector<int>{12, 20}; // 1. d4 d5; 2. c4 e6
    book["rnbqkbnrppp.pppp...........p.......P.........N..PPP.PPPPRNBQKB.R"] = std::vector<int>{2, 29}; // 1. d4 d5; 2. Nf3 Bf5;

    /* book["
        rnbqkbnr
        pppppppp
        ........
        ........
        ........
        ........
        PPPPPPPP
        RNBQKBNR"] = std::vector<int>{6, 21}; */
}
