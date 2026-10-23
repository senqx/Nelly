/*
 * Nelly, a UCI chess playing engine
 * Copyright (C) 2023 senqx
 *
 * Nelly is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nelly is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <list>
#include <string>
#include "chess/board.h"
#include "chess/move.h"

int main(int argc, char* argv[]) {
  if (argc == 2 && std::string(argv[1]) == "--help") {
    std::cout << "Usage: Nelly [\"FEN\"]\nPrint a position and its legal moves.\n"
                 "This is a development tool; UCI and computer play are not implemented.\n";
    return 0;
  }
  if (argc > 2) {
    std::cerr << "Usage: Nelly [\"FEN\"]\n";
    return 1;
  }
  try {
    Board board;
    board.setFen(argc == 2 ? argv[1] : Board::defaultFen);
    std::cout << "Nelly — move-generation development tool\n";
    board.print();
    const auto moves = board.legalMoves();
    std::cout << "Legal moves: " << moves.size() << '\n';
    for (const Move& move : moves) {
      std::cout << move.str() << (move.givesCheck ? " +" : "") << '\n';
    }
  } catch (const FenError& error) {
    std::cerr << error.what() << '\n';
    return 1;
  }
}
