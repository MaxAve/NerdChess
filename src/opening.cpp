#include <iostream>
#include "headers/opening.h"

namespace NerdChess
{
namespace opening
{
std::unordered_map<std::string, std::vector<int>> book;

void init_opening_book()
{
    // e4
    book["rnbqkbnrpppppppp................................PPPPPPPPRNBQKBNR"] = std::vector<int>{52, 36}; // 1. e4
    book["rnbqkbnrpppppppp....................P...........PPPP.PPPRNBQKBNR"] = std::vector<int>{12, 28}; // 1. e4 e5
    book["rnbqkbnrpppp.ppp............p.......P...........PPPP.PPPRNBQKBNR"] = std::vector<int>{62, 45}; // 1. e4 e5; 2. Nf3
    book["rnbqkbnrpppp.ppp............p.......P........N..PPPP.PPPRNBQKB.R"] = std::vector<int>{1, 18}; // 1. e4 e5; 2. Nf3 Nc6
    book["r.bqkbnrpppp.ppp..n.........p.......P........N..PPPP.PPPRBNQKB.R"] = std::vector<int>{61, 34}; // 1. e4 e5; 2. Nf3 Nc6; 3. Bc4
    book["r.bqkbnrpppp.ppp..n.........p.....B.P........N..PPPP.PPPRBNQK..R"] = std::vector<int>{6, 21}; // 1. e4 e5; 2. Nf3 Nc6; 3. Bc4 Nf6

    // d4
    book["rnbqkbnrpppppppp...................P............PPP.PPPPRBNQKBNR"] = std::vector<int>{11, 27}; // 1. d4 d5


    /* book["
        r.bqkbnr
        pppp.ppp
        ..n.....
        ....p...
        ..B.P...
        .....N..
        PPPP.PPP
        RBNQK..R"] = std::vector<int>{6, 21}; // 1. e4 e5; 2. Nf3 Nc6; 3. Bc4 Nf6 */
}
} // namespace opening
} // namespace NerdChess
