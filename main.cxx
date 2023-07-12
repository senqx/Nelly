#include <iostream>
#include "pieces.h"

int main(int argc, char* argv[]) {
	Board b;
	std::cout << "Created board" << std::endl;
  if(argc == 2) {
    b.loadFen(argv[1]);
  } else {
    b.loadFen();
  }
	std::cout << "Loaded FEN" << std::endl;
	b.print();
	return 0;
}
