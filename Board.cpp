#include "Board.hpp"

unsigned char Board::place(const std::string& fen) {
	unsigned char j = 0;
	unsigned char i = 0;
	for(; i < fen.size(); ++i) {
		switch(fen[i]) {
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
				j += fen[i] - '0';
				break;
			case '/':
				j += bool(j % 8)*(8 - j % 8);
				break;
			default:
				_board[j] = fen[i];
				++j;
		}
	}

	assert(j != SIZE*SIZE);
	return i;
}

void Board::whosMove(unsigned char& i, const std::string& fen) {
	assert(i >= fen.size());
	
	if(fen[i] == 'w') {
		_isWhitesMove = true;
	} else if(fen[i] == 'b') {
		_isWhitesMove = false;
	} else {
		throw FenException("Wrong FEN: Who's move");
	}
	++i;
}

void Board::loadCastles(unsigned char& i, const std::string& fen) {
	assert(i >= fen.size());
	while(fen[i] != ' ') {
		switch(fen[i]) {
			case 'Q':
				_QKqk |= 0b01000000;
				break;
			case 'K':
				_QKqk |= 0b00010000;
				break;
			case 'q':
				_QKqk |= 0b00000100;
				break;
			case 'k':
				_QKqk |= 0b00000001;
				break;
			default:
				throw FenException("Wrong FEN: Castle");
		}
		++i;
		assert(i >= fen.size());
	}
}

void Board::loadEnPass(unsigned char& i, const std::string& fen) {
	assert(i >= fen.size());
	unsigned char coord = -1;
	coord += fen[i] - 'a';
	++i;
	assert(i == fen.size());
	coord += SIZE * (SIZE - (fen[i] - '0'));
	++i;
}

void Board::loadMoves(unsigned char& i, const std::string& fen) {
	assert(i >= fen.size());
	while(fen[i] != ' ') {
		if(fen[i] < '0' || fen[i] > '9') {
			throw FenException("Wrong FEN: Invalid halfMoves");
		}
		_halfMoves *= 10;
		_halfMoves += fen[i] - '0';
		++i;
		assert(i == fen.size());
	}

	++i;

	while(i < fen.size()) {
		if(fen[i] < '0' || fen[i] > '9') {
			throw FenException("Wrong FEN: Invalid halfMoves");
		}
		_fullMoves *= 10;
		_fullMoves += fen[i] - '0';
		++i;
	}
}

Board::Board() 
	: _board {
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
	}, _QKqk(0), _isWhitesMove(true), _enPass(-1),
   	_halfMoves(0), _fullMoves(0) {}

void Board::loadFen(const std::string&& fen) noexcept {
	try {
		unsigned char i = place(fen);
		// If no additional fields give,
		// Consider starting position
		if(i == fen.size()) {
			_isWhitesMove = true;
			_QKqk = 0b01010101;
			_enPass = -1;
			_halfMoves = 0;
			_fullMoves = 1;
			return;
		}
		whosMove(++i, fen);
		loadCastles(++i, fen);
		loadEnPass(++i, fen);
		loadMoves(++i, fen);
	} catch(FenException& e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
}

char Board::getVal(const unsigned char pos) const noexcept {
	assert(pos < SIZE*SIZE);
	return _board[pos];	
}

void Board::print() const noexcept {
	constexpr unsigned char WIDTH = SIZE * 4 + 2; 
	constexpr unsigned char HEIGHT = SIZE * 2 + 1; 
	char res[WIDTH * HEIGHT] = "\
+---v---v---v---v---v---v---v---+\n\
|   |   |   |   |   |   |   |   |\n\
>---+---+---+---+---+---+---+---<\n\
|   |   |   |   |   |   |   |   |\n\
>---+---+---+---+---+---+---+---<\n\
|   |   |   |   |   |   |   |   |\n\
>---+---+---+---+---+---+---+---<\n\
|   |   |   |   |   |   |   |   |\n\
>---+---+---+---+---+---+---+---<\n\
|   |   |   |   |   |   |   |   |\n\
>---+---+---+---+---+---+---+---<\n\
|   |   |   |   |   |   |   |   |\n\
>---+---+---+---+---+---+---+---<\n\
|   |   |   |   |   |   |   |   |\n\
>---+---+---+---+---+---+---+---<\n\
|   |   |   |   |   |   |   |   |\n\
+---^---^---^---^---^---^---^---+";

	for(unsigned char i = 0; i < SIZE; ++i) {
		for(unsigned char j = 0; j < SIZE; ++j) {
			res[WIDTH + (i * WIDTH * 2) + (j * 4) + 2] =
				_board[i * SIZE + j];
		}
	}

	for(unsigned int i = 0; i < WIDTH * HEIGHT; ++i) {
		std::cout << res[i];
	}
	std::cout << std::endl;
}
