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
