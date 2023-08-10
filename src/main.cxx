#include <iostream>
#include <list>

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

  const std::list<Move>& moves = b.getValidMoves();
  Logger::info("Total move count: " + std::to_string(moves.size()));
  for (auto it = moves.begin(); it != moves.end(); ++it) {
    const unsigned char& from_i = it->from / 10 - 2;
    const unsigned char& from_j = it->from % 10 - 1;
    const unsigned char& to_i = it->to / 10 - 2;
    const unsigned char& to_j = it->to % 10 - 1;
    std::string msg = "Pa0->a0";
    msg[0] = b.getVal(it->from);
    msg[1] += from_j;
    msg[2] += 8 - from_i;
    msg[5] += to_j;
    msg[6] += 8 - to_i;
    Logger::debug(msg);
  }
	return 0;
}
