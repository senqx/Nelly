#include "board.h"

#include "../cpp-logger/logger.h"

#include <cassert>
#include <iostream>
#include <list>
#include <unordered_map>

#include "chess.h"
#include "pieces.h"
#include "move.h"

// Skip outline squares
static constexpr unsigned int OFFSET = 2 * Board::WIDTH + 1;

Board::Board()
  : _board{'?', '?', '?', '?', '?', '?', '?', '?', '?', '?', //
           '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', //
           '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '?', //
           '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '?', //
           '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '?', //
           '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '?', //
           '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '?', //
           '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '?', //
           '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '?', //
           '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '?', //
           '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', //
           '?', '?', '?', '?', '?', '?', '?', '?', '?', '?'} //
  , _pieces()
  , _QKqk(0)
  , _isWhitesMove(true)
  , _enPass(-1)
  , _halfMoves(0)
  , _fullMoves(0)
{}

void Board::loadFen(const std::string& fen) noexcept {
  Logger::debug("Loading fen: " + fen);
  try {
    unsigned int i = place(fen);
    if (i == fen.size()) {
      Logger::debug("As no additional parameters were given, using defaults");
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

std::list<Move> Board::getValidMoves() const noexcept {
  std::list<Move> moves;

  for (auto it = _pieces.begin(); it != _pieces.end(); ++it) {
    const BoardSquare& sqr = it->first;
    const char& val = _board[sqr];
    switch (val) {
    case 'p':
      if (!_isWhitesMove) {
        std::list<Move> mvs = Pawn::getValidMoves(this, sqr);
        moves.splice(moves.end(), mvs);
      }
      break;
    case 'P':
      if (_isWhitesMove) {
        std::list<Move> mvs = Pawn::getValidMoves(this, sqr);
        moves.splice(moves.end(), mvs);
      }
      break;
    case 'n':
      if (!_isWhitesMove) {
        std::list<Move> mvs = Knight::getValidMoves(this, sqr);
        moves.splice(moves.end(), mvs);
      }
      break;
    case 'N':
      if (_isWhitesMove) {
        std::list<Move> mvs = Knight::getValidMoves(this, sqr);
        moves.splice(moves.end(), mvs);
      }
      break;
    case 'b':
      if (!_isWhitesMove) {
        std::list<Move> mvs = Bishop::getValidMoves(this, sqr);
        moves.splice(moves.end(), mvs);
      }
      break;
    case 'B':
      if (_isWhitesMove) {
        std::list<Move> mvs = Bishop::getValidMoves(this, sqr);
        moves.splice(moves.end(), mvs);
      }
      break;
    case 'r':
      if (!_isWhitesMove) {
        std::list<Move> mvs = Rook::getValidMoves(this, sqr);
        moves.splice(moves.end(), mvs);
      }
      break;
    case 'R':
      if (_isWhitesMove) {
        std::list<Move> mvs = Rook::getValidMoves(this, sqr);
        moves.splice(moves.end(), mvs);
      }
      break;
    case 'q':
      if (!_isWhitesMove) {
        std::list<Move> mvs = Queen::getValidMoves(this, sqr);
        moves.splice(moves.end(), mvs);
      }
      break;
    case 'Q':
      if (_isWhitesMove) {
        std::list<Move> mvs = Queen::getValidMoves(this, sqr);
        moves.splice(moves.end(), mvs);
      }
      break;
    case 'k':
      if (!_isWhitesMove) {
        std::list<Move> mvs = King::getValidMoves(this, sqr);
        moves.splice(moves.end(), mvs);
      }
      break;
    case 'K':
      if (_isWhitesMove) {
        std::list<Move> mvs = King::getValidMoves(this, sqr);
        moves.splice(moves.end(), mvs);
      }
      break;
    }
  }

  return moves;
}

Board Board::makeMove(const Move& move) const noexcept {
  Board b = *this;
  b._isWhitesMove ^= 1;
  if (const char& val = _board[move.to];
      val != ' ')
  {
    // Check if it is a capture
    const char& enemy = 'A' + 32 * _isWhitesMove;
    if (val > enemy && val < enemy + ('Z' - 'A')) {
      b._pieces.erase(move.to);
    } else if (const char& who = _board[move.from];
              (who == 'K' || who == 'k'))
    {
      const int& diff = move.to - move.from;
      if (diff == -2 || diff == 2) {
        const BoardSquare& rookPos = move.from + ((diff > 0)? 3 : -4);
        const BoardSquare& newRookPos = move.to - 1 + (diff < 0) * 2;
        const char& rook = _isWhitesMove? 'R' : 'r';
        b._board[newRookPos] = rook;
        b._board[rookPos] = ' ';
        b._pieces.insert({newRookPos, rook});
        b._pieces.erase(rookPos);
      }
    }
  }
  b._board[move.to] = _board[move.from];
  b._board[move.from] = ' ';
  return b;
}

void Board::print() const noexcept {
  constexpr unsigned int SIZE = 8;
  constexpr unsigned int RES_WIDTH = SIZE * 4 + 2;
  constexpr unsigned int RES_HEIGHT = SIZE * 2 + 1;
  char res[RES_WIDTH * RES_HEIGHT] = "\
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

  for (unsigned int i = 0; i < SIZE; ++i) {
    for (unsigned int j = 0; j < SIZE; ++j) {
      res[RES_WIDTH + i * (RES_WIDTH * 2) + j * 4 + 2] =
          _board[OFFSET + i * WIDTH + j];
    }
  }

  for (unsigned int i = 0; i < RES_WIDTH * RES_HEIGHT; ++i) {
    std::cout << res[i];
  }
  std::cout << std::endl;
}

unsigned int Board::place(const std::string& fen) {
  Logger::debug("Starting piece placement...");
  unsigned int pieceCount = 0;
  unsigned int idx = 0;

  unsigned int j = 0;
  unsigned int i = 0;
  for (; idx < fen.size(); ++idx) {
    const char& val = fen[idx];
    switch (val) {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
      j += val - '0';
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
      const BoardSquare& sqr = OFFSET + i * WIDTH + j;
      _board[sqr] = val;
      _pieces.insert({sqr, val});
      ++pieceCount;
      const char chessField[3] = {char('a' + j), char('8' - i), '\0'};
      const std::string& msg = "Placing " + std::string(1, val) +
                               " on: " + chessField;
      Logger::debug(msg);
      ++j;
      break;
    }
    default:
      throw FenException("Wrong FEN: Unknown symbol");
    }
  }

  Logger::debug("Loaded pieces total count: " + std::to_string(pieceCount));
  assert(i * 8 + j == 64);
  return idx;
}

void Board::loadWhoseMove(unsigned int& r_i, const std::string& fen) {
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

void Board::loadCastles(unsigned int& r_i, const std::string& fen) {
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

void Board::loadEnPass(unsigned int& r_i, const std::string& fen) {
  Logger::debug("Loading en-passant info...");
  assert(r_i < fen.size());
  if (fen[r_i] == '-') {
    Logger::debug("No en-passant available");
    ++r_i;
    return;
  }
  ++r_i;
  assert(r_i < fen.size());
  char enPss[2] = {fen[r_i - 1], fen[r_i]};
  Logger::debug("En-passant available on " + std::string(enPss));
  ++r_i;
}

void Board::loadMoves(unsigned int& r_i, const std::string& fen) {
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

