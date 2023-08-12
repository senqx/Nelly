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

#include "chess/board.h"
#include "chess/move.h"
#include "chess/pieces.h"
#include "cpp-logger/logger.h"

int main(int argc, char* argv[]) {
  Logger::set_mode("debug");
  Logger::set_terminal_output(true);
  Logger::info("Running Nelly v0.0.1");

  Board b;
  if (argc == 2) {
    b.loadFen(argv[1]);
  } else {
    b.loadFen();
  }
	b.print();

  const std::list<Move>& moves = b.getValidMoves();
  Logger::info("Total move count: " + std::to_string(moves.size()));
  for (auto it = moves.begin(); it != moves.end(); ++it) {
    const std::string& mv = b.getVal(it->from) + it->toString();
    const Board& boardAfterMove = b.makeMove(*it);
    Logger::info("Made the move " + mv);
    boardAfterMove.print();
  }
	return 0;
}
