#include "board.h"

#include "../cpp-logger/logger.h"

#include <cassert>
#include <iostream>

Board::Board()
  : _board{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', //
           ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', //
           ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', //
           ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', // Did like this MANUALLY
           ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', // To be exactly 8x8
           ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', //
           ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', //
           ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '} //
  , _piecePositions{SIZE * SIZE}
  , _pieceCount(0)
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
    if (i == fen.size()) {
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
  } catch (const FenException& e) {
    Logger::error(e.what());
    exit(1);
  }
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

  for (unsigned char i = 0; i < SIZE; ++i) {
    for (unsigned char j = 0; j < SIZE; ++j) {
      res[WIDTH + (i * WIDTH * 2) + (j * 4) + 2] = _board[i * 8 + j];
    }
  }

  for (unsigned int i = 0; i < WIDTH * HEIGHT; ++i) {
    std::cout << res[i];
  }
  std::cout << std::endl;
}

unsigned char Board::place(const std::string& fen) {
  Logger::debug("Starting piece placement...");
  unsigned char idx = 0;

  unsigned char j = 0;
  unsigned char i = 0;
  for (; idx < fen.size(); ++idx) {
    switch (fen[idx]) {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
      j += fen[idx] - '0';
      break;
    case '/':
      ++i;
      j = 0;
      break;
    case ' ':
      return idx;
    case 'p':
    case 'P':
    case 'n':
    case 'N':
    case 'b':
    case 'B':
    case 'r':
    case 'R':
    case 'q':
    case 'Q':
    case 'k':
    case 'K': {
      const unsigned char& pos = i * 8 + j;
      _board[pos] = fen[idx];
      _piecePositions[_pieceCount++] = pos;
      const char chessField[2] = {char('a' + j), char('8' - i)};
      const std::string& msg = "Placing " + std::string(1, char(fen[idx])) +
                               " on: " + chessField;
      Logger::debug(msg);
      ++j;
      break;
    }
    default:
      throw FenException("Wrong FEN: Unknown symbol");
    }
  }

  Logger::debug("Loaded pieces total count: " + std::to_string(_pieceCount));
  assert(i * 8 + j == 64);
  return idx;
}

void Board::loadWhoseMove(unsigned char& r_i, const std::string& fen) {
  Logger::debug("Loading whose move is it...");
  assert(r_i < fen.size());

  if (fen[r_i] == 'w') {
    _isWhitesMove = true;
    Logger::debug("It is white's move");
  } else if (fen[r_i] == 'b') {
    _isWhitesMove = false;
    Logger::debug("It is black's move");
  } else {
    throw FenException("Wrong FEN: Who's move");
  }
  ++r_i;
}

void Board::loadCastles(unsigned char& r_i, const std::string& fen) {
  Logger::debug("Loading castle status...");
  assert(r_i < fen.size());
  while (fen[r_i] != ' ') {
    switch (fen[r_i]) {
    case 'Q':
      _QKqk |= 0b01000000;
      Logger::debug("White can castle queen side");
      break;
    case 'K':
      _QKqk |= 0b00010000;
      Logger::debug("White can castle king side");
      break;
    case 'q':
      _QKqk |= 0b00000100;
      Logger::debug("Black can castle queen side");
      break;
    case 'k':
      _QKqk |= 0b00000001;
      Logger::debug("Black can castle king side");
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
  Logger::debug("Loading en-passant info...");
  assert(r_i < fen.size());
  if (fen[r_i] == '-') {
    Logger::debug("No en-passant available");
    ++r_i;
    return;
  }
  unsigned char coord = -1;
  coord += fen[r_i] - 'a';
  ++r_i;
  assert(r_i < fen.size());
  char enPs[2] = {fen[r_i - 1], fen[r_i]};
  Logger::debug("En-passant available on " + std::string(enPs));
  coord += SIZE * (SIZE - (fen[r_i] - '0'));
  ++r_i;
}

void Board::loadMoves(unsigned char& r_i, const std::string& fen) {
  Logger::debug("Loading half moves...");
  assert(r_i < fen.size());
  while (fen[r_i] != ' ') {
    if (fen[r_i] < '0' || fen[r_i] > '9') {
      throw FenException("Wrong FEN: Invalid halfMoves");
    }
    _halfMoves *= 10;
    _halfMoves += fen[r_i] - '0';
    ++r_i;
    assert(r_i < fen.size());
  }
  Logger::debug("Half moves: " + std::to_string(_halfMoves));

  ++r_i;

  Logger::debug("Loading full moves...");
  while (r_i < fen.size()) {
    if (fen[r_i] < '0' || fen[r_i] > '9') {
      throw FenException("Wrong FEN: Invalid halfMoves");
    }
    _fullMoves *= 10;
    _fullMoves += fen[r_i] - '0';
    ++r_i;
  }
  Logger::debug("Full moves: " + std::to_string(_fullMoves));
}

