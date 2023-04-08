/*
Contains functions responsible for generating output for the user,
such as UI, debug, etc.
*/

#include <iostream>
#include <vector>
#include "position.h"

using namespace NerdChess::board;

namespace NerdChess
{
namespace debug
{
void print_vec(std::vector<int> vec);
void print_board(struct position pos);
} // namespace debug
} // namespace NerdChess
