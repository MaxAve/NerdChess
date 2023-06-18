#ifndef OPENING_H
#define OPENING_H

#include <iostream>
#include <vector>
#include <unordered_map>

namespace NerdChess
{
namespace opening
{
extern std::unordered_map<std::string, std::vector<int>> book;

void init_opening_book();
} // namespace opening
} // namespace NerdChess

#endif
