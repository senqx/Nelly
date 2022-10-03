#include <iostream>
#include "cpp-logger/Logger.hpp"
#include "Nelly.hpp"

int main(int argc, const char* argv[]) {
	if(argc != 2) {
		Logger::error("Wrong argument count");
		return -1;
	}

	Logger::set_mode("Warning");
	Logger::set_output_filename("Analyse.log");

	Logger::debug("Creating Nelly unit");
	Nelly nelly;

	// Just testing
	std::string fen = argv[1];
	nelly.load_board_from_fen(fen);
	Logger::info("The board is\n" + nelly.get_board_string());

	Logger::info("Getting all possible moves for current board");
	std::vector<Move> m = nelly.get_valid_moves();

	Logger::info("Valid moves count: " + std::to_string(m.size()));
	for(int i = 0; i < m.size(); ++i) {
		Logger::info(m[i].to_string());
	}

	return 0;
}