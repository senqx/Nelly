#include "board.h"

#include <cassert>
#include <iostream>

#include "../cpp-logger/logger.h"

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
  }
  , _QKqk(0)
  , _isWhitesMove(true)
  , _enPass(-1)
  , _halfMoves(0)
  , _fullMoves(0)
{}

void Board::loadFen(const std::string& fen) noexcept {
  Logger::debug("Loading fen: " + fen);
  try {
		unsigned char i = place(fen);
		if(i == fen.size()) {
      Logger::debug("As no additional parameters were given, use defaults");
			_isWhitesMove = true;
			_QKqk = 0b01010101;
			_enPass = -1;
			_halfMoves = 0;
			_fullMoves = 1;
			return;
		}

		loadWhoseMove(++i, fen);
		loadCastles(++i, fen);
		loadEnPass(++i, fen);
		loadMoves(++i, fen);
	} catch(FenException& e) {
    Logger::error(e.what());
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
			res[WIDTH + (i * WIDTH * 2) + (j * 4) + 2] = _board[i * SIZE + j];
		}
	}

	for(unsigned int i = 0; i < WIDTH * HEIGHT; ++i) {
		std::cout << res[i];
	}
	std::cout << std::endl;
}

unsigned char Board::place(const std::string& fen) {
  Logger::debug("Placing pieces");
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
				j += bool(j % 8) * (8 - j % 8);
			  break;
      case ' ':
        return i;
			default:
				_board[j] = fen[i];
				++j;
		}
	}

	assert(j == SIZE*SIZE);
	return i;
}

void Board::loadWhoseMove(unsigned char& r_i, const std::string& fen) {
  Logger::debug("Loading whose move is it");
	assert(r_i < fen.size());

	if(fen[r_i] == 'w') {
		_isWhitesMove = true;
	} else if(fen[r_i] == 'b') {
		_isWhitesMove = false;
	} else {
		throw FenException("Wrong FEN: Who's move");
	}
	++r_i;
}

void Board::loadCastles(unsigned char& r_i, const std::string& fen) {
  Logger::debug("Loading castle status");
	assert(r_i < fen.size());
	while(fen[r_i] != ' ') {
		switch(fen[r_i]) {
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
      case '-':
        // _QKqk = 0;
        break;
			default:
				throw FenException("Wrong FEN: Castle");
		}
		++r_i;
		assert(r_i < fen.size());
	}
}

void Board::loadEnPass(unsigned char& r_i, const std::string& fen) {
  Logger::debug("Loading en-passant info");
	assert(r_i < fen.size());
	unsigned char coord = -1;
	coord += fen[r_i] - 'a';
	++r_i;
	assert(r_i < fen.size());
	coord += SIZE * (SIZE - (fen[r_i] - '0'));
	++r_i;
}

void Board::loadMoves(unsigned char& r_i, const std::string& fen) {
  Logger::debug("Loading half moves");
	assert(r_i < fen.size());
	while(fen[r_i] != ' ') {
		if(fen[r_i] < '0' || fen[r_i] > '9') {
			throw FenException("Wrong FEN: Invalid halfMoves");
		}
		_halfMoves *= 10;
		_halfMoves += fen[r_i] - '0';
		++r_i;
		assert(r_i < fen.size());
	}

	++r_i;

  Logger::debug("Loading total moves");
	while(r_i < fen.size()) {
		if(fen[r_i] < '0' || fen[r_i] > '9') {
			throw FenException("Wrong FEN: Invalid halfMoves");
		}
		_fullMoves *= 10;
		_fullMoves += fen[r_i] - '0';
		++r_i;
	}
}

