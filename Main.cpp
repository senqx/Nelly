#include "Pieces.hpp"

int main() {
	Board b;
	std::cout << "Created board" << std::endl;
	b.loadFen();
	std::cout << "Loaded FEN" << std::endl;
	b.print();
	return 0;
}
