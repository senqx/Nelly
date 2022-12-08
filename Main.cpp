#include <iostream>
#include "cpp-logger/logger.hpp"
#include "Nelly.hpp"


// Default settings
static bool isHelp = false;
static bool isVerbose = false;
static std::string outFileName = "Anylize.log";
static std::string fen = "";
//	"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

static const char* helpMessage = 
	"	Usage: Nelly [options...] [FEN] \n\
	 Options:\n\
	 --help,		-h	->	show help\n\
	 --verbose,		-v	->	set logger to debug mode (show moves in detail)\n\
	 						and enable terminal output\n\
	 --outfile,		-o	->	specify log file name (next argument)\n";

// Help functions
void set_help() {
	isHelp = true;
}

void set_verbose() {
	isVerbose = true;
}

void set_filename(int &argc, const char** argv, int &i) {
	++i;
	if(i != argc) {
		outFileName = argv[i];
		if(outFileName[0] == '-') {
			std::cerr << "Given output file name starts with '-'. " <<
				"Did you miss argument?" << std::endl;
			exit(1);
		}
	} else {
		std::cerr << 
			"Expected output file name, but got nothing" << std::endl;
		exit(1);
	}
}

void handle_arguments(int argc, const char** argv) {
	for(int i = 1; i < argc; ++i) {
		if(i == argc - 1) {
			std::string arg(argv[i]);
			if(arg[0] == '-') {
				if(arg == "-h" || arg == "--help" ) {
					set_help();
					// as it is last argument we can freely return
					return;
				} else {
					std::cerr << "Bad argument usage " << arg << std::endl;
					exit(1);
				}
			// The length of a FEN with only 2 kings (black and white) is 29
			} else if(arg.size() < 30) {
				std::cerr << "Wrong FEN argument " << arg << std::endl;
				exit(1);
			}
			fen = arg;
			return;
		}

		if(argv[i][0] == '-') {
			// It is a word argument
			if(argv[i][1] == '-') {
				if(std::string(argv[i]) ==  "--help") {
					set_help();
				} else if(std::string(argv[i]) == "--verbose") {
					set_verbose();
				} else if(std::string(argv[i]) == "--outfile") {
					set_filename(argc, argv, i);
				} else {
					std::cerr << "Unknown parameter " <<
						std::string(argv[i]) << std::endl;
					exit(1);
				}
			// Is it 1-letter argument or argument list
			// For example (-a or -lha)
			} else {
				int size = std::string(argv[i]).size();
				bool isArray = size > 2;

				if(isArray) {
					for(int j = 1; j < size; ++j) {	
						if(argv[i][j] == 'v') {
							set_verbose();
						} else {
							if(argv[i][j] == 'o') {
								std::cerr << 
									"Output file name parameter -o "<<
									"must be used seperately" << std::endl;
							} else if(argv[i][j] == 'h') {
								std::cerr << "Help parameter must be lone " <<
									"-h or --help" << std::endl;
							} else {
								std::cerr << "Unknown parameter -" <<
									argv[i][j] << std::endl;
							}
							exit(1);
						}
					}
				} else {
					if(argv[i][1] == 'h') {
						set_help();
					} else if(argv[i][1] == 'o') {
						set_filename(argc, argv, i);
					} else if(argv[i][1] == 'v') {
						set_verbose();
					} else {
						std::cerr << "Unknown parameter " <<
							std::string(argv[i]) << std::endl;
						exit(1);
					}
				}
			}
		} else {
			std::cerr << "Unknown parameter " <<
				std::string(argv[i]) << std::endl;
			exit(1);
		}
	}
}

int main(int argc, const char* argv[]) {
	handle_arguments(argc, argv);
	
	// Overriding arguments check
	if(isHelp) {
		std::cout << helpMessage;
		exit(0);
	}
	

	// Normal check
	if(isVerbose) {
		Logger::set_mode("Debug");
		Logger::set_terminal_output(true);
	}
	
	if(fen.empty()) {
		std::cerr << "FEN not specified" << std::endl;
		exit(1);
	}

	// Arguments read, now starting main part
	Logger::set_output_filename(outFileName);
	Logger::info("Set output file name to " + outFileName);
	
	Logger::debug("Creating Nelly unit");
	Nelly nelly;

	nelly.load_board_from_fen(fen);
	Logger::info("The board is\n" + nelly.get_board_string());

	Logger::info("Getting all possible moves for current board");
	auto m = nelly.get_valid_moves();

	Logger::info("Valid moves count: " + std::to_string(m.size()));
	for(auto it = m.begin(); it != m.end(); ++it) {
		Logger::info(it->to_string());
	}

	return 0;
}

