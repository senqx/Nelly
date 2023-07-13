#include <iostream>

#include "chess/board.h"
#include "chess/pieces.h"
#include "cpp-logger/logger.h"

int main(int argc, char* argv[]) {
  Logger::set_mode("debug");
  Logger::set_terminal_output(true);

  Board b;
  if(argc == 2) {
    b.loadFen(argv[1]);
  } else {
    b.loadFen();
  }
	b.print();
	return 0;
}
