#include <iostream>
#include "cpp-logger/logger.hpp"
#include "Nelly.hpp"


// Default settings
static bool isHelp = false;
static bool isVerbose = false;
static bool isDebug = false;
static unsigned int depth = 1;
static std::string outFileName = "Anylize.log";
static std::string fen = "";
//	"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

static const char* helpMessage = 
	"Usage: Nelly [options...] [FEN]\n\
	 Options:\n\
	 --help,		-h	->	show help\n\
	 --verbose,		-v	->  enable terminal output\n\
	 --debug,		-d	->	enable debug mode\n\
	 --outfile,		-o	->	specify log file name (next argument)\n\
	 --debug,		-D	->	specify depth for depth analyze\n";

// Help functions
void set_help() {
	isHelp = true;
}

void set_verbose() {
	isVerbose = true;
}

void set_debug() {
	isDebug = true;
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

void set_depth(int &argc, const char** argv, int &i) {
	++i;
	if(i != argc) {
		char c = argv[i][0];
		if(c >= '1' && c <= '9') {
			depth = std::stoi(argv[i]);
		} else {
			std::cerr << "Wrong depth value" << std::endl;
			exit(1);
		}
	} else {
		std::cerr << "Expected depth value, but got nothing" << std::endl;
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
				std::string arg(argv[i]);
				if(arg ==  "--help") {
					set_help();
				} else if(arg == "--verbose") {
					set_verbose();
				} else if(arg == "--outfile") {
					set_filename(argc, argv, i);
				} else if(arg == "--debug") {
					set_debug();	
				} else if(arg == "--depth") {
					set_depth(argc, argv, i);
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
						char arg = argv[i][j];	
						if(arg == 'v') {
							set_verbose();
						} else if(arg == 'd') {
							set_debug();		
						} else {
							// Errors
							if(arg == 'o') {
								std::cerr << 
									"Output file name parameter -o "<<
									"must be used seperately" << std::endl;
							} else if(arg == 'h') {
								std::cerr << "Help parameter must be lone " <<
									"-h or --help" << std::endl;
							} else if(arg == 'D') {
								std::cerr << "Depth parameter must be lone " <<
									"-D or --depth" << std::endl;	
							} else {
								std::cerr << "Unknown parameter -" <<
									arg << std::endl;
							}
							exit(1);
						}
					}
				} else {
					char arg = argv[i][1];
					if(arg == 'h') {
						set_help();
					} else if(arg == 'o') {
						set_filename(argc, argv, i);
					} else if(arg == 'v') {
						set_verbose();
					} else if(arg == 'd') {
						set_debug();	
					} else if(arg == 'D') {
						set_depth(argc, argv, i);	
					} else {
						std::cerr << "Unknown parameter -" <<
							arg << std::endl;
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
		Logger::set_terminal_output(true);
	}

	if(isDebug) {
		Logger::set_mode("Debug");
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

	Logger::info("Calculating valid moves count for depth " + std::to_string(depth));
	size_t count = nelly.get_valid_moves_count_for_depth(depth);
	Logger::info("Move count for depth " + std::to_string(depth) + " is " + std::to_string(count));
	return 0;
}

