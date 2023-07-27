#include <iostream>
#include <vector>

#include "chess/board.h"
#include "chess/move.h"
#include "chess/pieces.h"
#include "cpp-logger/logger.h"

int main(int argc, char* argv[]) {
  Logger::set_mode("debug");
  Logger::set_terminal_output(true);
  Logger::debug("Running Nelly v0.0.1");

  Board b;
  if (argc == 2) {
    b.loadFen(argv[1]);
  } else {
    b.loadFen();
  }
	b.print();

  const std::vector<Move>& moves = b.getValidMoves();
  Logger::info("Total move count: " + std::to_string(moves.size()));
  for (const auto& move : moves) {
    const unsigned char& from_i = move.from / 8;
    const unsigned char& from_j = move.from % 8;
    const unsigned char& to_i = move.to / 8;
    const unsigned char& to_j = move.to % 8;
    std::string msg = "Pa0->a0";
    msg[1] += from_j;
    msg[2] += Board::SIZE - from_i;
    msg[5] += to_j;
    msg[6] += Board::SIZE - to_i;
    Logger::debug(msg);
  }
	return 0;
}
