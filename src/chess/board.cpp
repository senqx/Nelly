/*
 * Nelly, a UCI chess playing engine
 * Copyright (C) 2023 senqx
 *
 * Nelly is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nelly is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "board.h"

#include "../cpp-logger/logger.h"

#include <cassert>
#include <list>
#include <iostream>
#include <string>

#include "chess.h"
#include "pieces.h"
#include "move.h"

//! Offset to skip outline squares.
static constexpr unsigned int OFFSET = 2 * Board::WIDTH + 1;

Board::Board()
  : m_board{'?', '?', '?', '?', '?', '?', '?', '?', '?', '?', //
            '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', //
            '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '?', // 8
            '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '?', // 7
            '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '?', // 6
            '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '?', // 5
            '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '?', // 4
            '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '?', // 3
            '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '?', // 2
            '?', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '?', // 1
            '?', '?', '?', '?', '?', '?', '?', '?', '?', '?', //
            '?', '?', '?', '?', '?', '?', '?', '?', '?', '?'} //
          //      A    B    C    D    E    F    G    H        //
  , m_pieces{ 0 }
  , m_enPass(0)
  , m_flags{0, 0, 1, 0, 0}
{}

void Board::loadFen(const std::string& fen) noexcept {
  Logger::debug("Loading fen: " + fen);
  try {
    unsigned int i = place(fen);
    if (i == fen.size()) {
      Logger::debug("As no additional parameters were given, using defaults");
      m_enPass = 0;
      m_flags.m_isWhitesMove = true;
      m_flags.m_castleInfo = 0b1111;
      m_flags.m_halfMoves = 0;
      m_flags.m_fullMoves = 1;
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

std::list<Move> Board::getValidMoves(const BoardSquare& sqr) const noexcept {
  const char val = m_board[sqr];
  std::list<Move> moves;
  switch(val) {
    case 'P':
    case 'p':
      moves.splice(moves.end(), Pawn::getValidMoves(*this, sqr));
      break;
    case 'N':
    case 'n':
      moves.splice(moves.end(), Knight::getValidMoves(*this, sqr));
      break;
    case 'B':
    case 'b':
      moves.splice(moves.end(), Bishop::getValidMoves(*this, sqr));
      break;
    case 'R':
    case 'r':
      moves.splice(moves.end(), Rook::getValidMoves(*this, sqr));
      break;
    case 'Q':
    case 'q':
      moves.splice(moves.end(), Queen::getValidMoves(*this, sqr));
      break;
    case 'K':
    case 'k':
      moves.splice(moves.end(), King::getValidMoves(*this, sqr));
      break;
  }

  return moves;
}

std::list<Move> Board::getValidMoves() const noexcept {
  std::string msg;
  for (int i = 0; i < m_flags.m_pieceCount; ++i) {
    msg += std::to_string(int(m_pieces[i])) + ", ";
  }
  msg.pop_back();
  msg.pop_back();
  Logger::debug("Pieces: " + msg);
  std::list<Move> moves;
  for (int i = 0; i < m_flags.m_pieceCount; ++i) {
    const BoardSquare& sqr = m_pieces[i];
    if (!isValid(sqr)) {
      break;
    }

    if (isWhite(sqr) == m_flags.m_isWhitesMove) {
      moves.splice(moves.end(), getValidMoves(sqr));
    }
  }

  return moves;
}

void Board::removePiece(const BoardSquare& sqr) noexcept {
  for (int i = 0; i < m_flags.m_pieceCount; ++i) {
    if (m_pieces[i] == sqr) {
      m_pieces[i] = 0;
      m_board[sqr] = ' ';
      --m_flags.m_pieceCount;
      std::swap(m_pieces[i], m_pieces[m_flags.m_pieceCount]);
      return;
    }
  }
}

Board Board::makeMove(const Move& move) const noexcept {
  Board board(*this);
  assert(isValid(move.to) && "Making an invalid move");

  if (!board.isEmpty(move.to))
  {
    board.removePiece(move.to);
  } else if (move.to == m_enPass) {
    const BoardSquare& enemyPawnSqr = move.to + (m_flags.m_isWhitesMove? WIDTH : -WIDTH);
    board.removePiece(enemyPawnSqr);
    board.m_enPass = 0;
  } else if (isKing(move.from)) {
    const int diff = move.to - move.from;
    if (std::abs(diff) >= 2) {
      const BoardSquare& rookPos = move.from + ((diff > 0)? 3 : -4);
      const BoardSquare& newRookPos = move.to - 1 + (diff < 0) * 2;
      for (int i = 0; i < m_flags.m_pieceCount; ++i) {
        if (m_pieces[i] == rookPos) {
          board.m_pieces[i] = newRookPos;
          break;
        }
      }
      std::swap(board.m_board[newRookPos], board.m_board[rookPos]);
    }
  } else if (isPawn(move.from) && (move.from / 10 == (m_flags.m_isWhitesMove? 8 : 3))) {
    board.m_enPass = move.to + (m_flags.m_isWhitesMove? WIDTH : -WIDTH);
  }

  board.m_board[move.to] = m_board[move.from];
  board.m_board[move.from] = ' ';
  board.m_flags.m_isWhitesMove ^= 1;
  return board;
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
          m_board[OFFSET + i * WIDTH + j];
    }
  }

  for (unsigned int i = 0; i < RES_WIDTH * RES_HEIGHT; ++i) {
    std::cout << res[i];
  }
  std::cout << std::endl;
}

unsigned int Board::place(const std::string& fen) {
  Logger::debug("Starting piece placement...");
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
        m_board[sqr] = val;
        m_pieces[m_flags.m_pieceCount] = sqr;
        ++m_flags.m_pieceCount;
        const char chessField[3] = {char('a' + j), char('8' - i), '\0'};
        const std::string& msg = "Placing " + std::string(1, val) +
                                 " on: " + chessField;
        Logger::debug(msg);
        ++j;
        break;
      }
      default:
        std::string errMsg = "Wrong FEN: Unknown symbol: ";
        errMsg.push_back(val);
        throw FenException(errMsg.c_str());
    }
  }

  Logger::debug("Loaded pieces total count: " + std::to_string(m_flags.m_pieceCount));
  assert(i * 8 + j == 64);
  return idx;
}

void Board::loadWhoseMove(unsigned int& r_i, const std::string& fen) {
  Logger::debug("Loading whose move is it...");
  assert(r_i < fen.size());

  if (fen[r_i] == 'w') {
    m_flags.m_isWhitesMove = true;
    Logger::debug("It is white's move");
  } else if (fen[r_i] == 'b') {
    m_flags.m_isWhitesMove = false;
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
      m_flags.m_castleInfo |= 0b1000;
      Logger::debug("White can castle queen side");
      break;
    case 'K':
      m_flags.m_castleInfo |= 0b0100;
      Logger::debug("White can castle king side");
      break;
    case 'q':
      m_flags.m_castleInfo |= 0b0010;
      Logger::debug("Black can castle queen side");
      break;
    case 'k':
      m_flags.m_castleInfo |= 0b0001;
      Logger::debug("Black can castle king side");
      break;
    case '-':
      m_flags.m_castleInfo = 0;
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
    m_enPass = 0;
    ++r_i;
    return;
  }
  ++r_i;
  assert(r_i < fen.size());
  const char enPss[3] = {fen[r_i - 1], fen[r_i], 0};
  m_enPass = OFFSET + WIDTH * 5              // EnPass can only be on 6 or 3!
             + fen[r_i - 1] - 'a'                     // Which file?
             - (WIDTH * 3 * m_flags.m_isWhitesMove);  // 6th or 3rd line?
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
    m_flags.m_halfMoves *= 10;
    m_flags.m_halfMoves += fen[r_i] - '0';
    ++r_i;
    assert(r_i < fen.size());
  }
  Logger::debug("Half moves: " + std::to_string(m_flags.m_halfMoves));

  ++r_i;

  Logger::debug("Loading full moves...");
  while (r_i < fen.size()) {
    if (fen[r_i] < '0' || fen[r_i] > '9') {
      throw FenException("Wrong FEN: Invalid halfMoves");
    }
    m_flags.m_fullMoves *= 10;
    m_flags.m_fullMoves += fen[r_i] - '0';
    ++r_i;
  }
  Logger::debug("Full moves: " + std::to_string(m_flags.m_fullMoves));
}

