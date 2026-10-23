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

#include <cassert>
#include <cctype>
#include <iostream>
#include <list>
#include <limits>
#include <string>
#include <string_view>

#include "chess.h"
#include "move.h"
#include "pieces.h"
#include "logger.h"

//! Offset to skip outline squares.
static constexpr unsigned int offset = 2 * Board::width + 1;

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
  , m_kings{ 0, 0 }
  , m_enPassantTarget(0)
  , m_flags{0, 0, 1, 0, 0}
{}

void Board::setFen(std::string_view fen) {
  Logger::debug("Loading FEN: " + std::string(fen));
  Board parsed;
  std::size_t i = parsed.parseBoard(fen);
  if (i == fen.size()) {
    throw FenError("Wrong FEN: missing position fields");
  }

  parsed.parseTurn(++i, fen);
  parsed.parseCastling(++i, fen);
  parsed.parseEnPassant(++i, fen);
  parsed.parseClocks(++i, fen);

  unsigned whiteKings = 0, blackKings = 0;
  for (unsigned n = 0; n < parsed.m_flags.m_pieceCount; ++n) {
    const Square sqr = parsed.m_pieces[n];
    if (parsed.isKing(sqr)) {
      (parsed.isWhite(sqr) ? whiteKings : blackKings)++;
    }
  }
  if (whiteKings != 1 || blackKings != 1) {
    throw FenError("Wrong FEN: position must contain exactly one king per side");
  }

  if (parsed.isAttackedBy(parsed.kingSquare(parsed.enemy()), parsed.turn()))
    throw FenError("Wrong FEN: the side that just moved is in check");

  *this = parsed;
}

std::list<Move> Board::pseudoMoves(Square sqr) const {
  assert(isOnBoard(sqr));
  const char val = m_board[sqr];
  std::list<Move> moves;
  switch(val) {
    case 'P':
    case 'p':
      moves.splice(moves.end(), Pawn::moves(*this, sqr));
      break;
    case 'N':
    case 'n':
      moves.splice(moves.end(), Knight::moves(*this, sqr));
      break;
    case 'B':
    case 'b':
      moves.splice(moves.end(), Bishop::moves(*this, sqr));
      break;
    case 'R':
    case 'r':
      moves.splice(moves.end(), Rook::moves(*this, sqr));
      break;
    case 'Q':
    case 'q':
      moves.splice(moves.end(), Queen::moves(*this, sqr));
      break;
    case 'K':
    case 'k':
      moves.splice(moves.end(), King::moves(*this, sqr));
      break;
  }

  return moves;
}

std::list<Move> Board::legalMoves() const {
  std::list<Move> moves;
  for (int i = 0; i < m_flags.m_pieceCount; ++i) {
    const Square sqr = m_pieces[i];
    assert(isOnBoard(sqr));
    assert(!isEmpty(sqr));

    if (isWhite(sqr) == m_flags.m_whiteToMove) {
      std::list<Move> candidates = pseudoMoves(sqr);
      for (Move move : candidates) {
        Board next = makeMove(move);
        const Color movingSide = turn();
        if (!next.isAttackedBy(next.kingSquare(movingSide), next.turn())) {
          move.givesCheck = next.isInCheck();
          moves.push_back(move);
        }
      }
    }
  }

  return moves;
}

void Board::movePiece(Square from, Square to) noexcept {
  for (unsigned i = 0; i < m_flags.m_pieceCount; ++i) {
    if (m_pieces[i] == from) {
      m_pieces[i] = to;
      return;
    }
  }
  assert(false && "Moving piece is missing from piece list");
}

void Board::removePiece(Square sqr) noexcept {
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

Board Board::makeMove(const Move& move) const {
  Board board(*this);
  assert(isOnBoard(move.from) && isOnBoard(move.to) && !isEmpty(move.from));
  assert(isWhite(move.from) == m_flags.m_whiteToMove);
  assert(isEmpty(move.to) || isEnemy(move.to));

  const char movingPiece = m_board[move.from];
  const bool white = isWhite(move.from);
  const bool pawnMove = isPawn(move.from);
  const bool kingMove = isKing(move.from);
  const bool capture = !board.isEmpty(move.to) || (pawnMove && move.to == m_enPassantTarget);

  board.m_enPassantTarget = 0;

  if (!board.isEmpty(move.to))
  {
    board.removePiece(move.to);
  } else if (pawnMove && move.to == m_enPassantTarget) {
    const Square enemyPawnSqr = move.to
        + (m_flags.m_whiteToMove ? width : -width);
    assert(isOnBoard(enemyPawnSqr));
    assert(!isEmpty(enemyPawnSqr));
    assert(isPawn(enemyPawnSqr));
    assert(isWhite(enemyPawnSqr) != white);
    board.removePiece(enemyPawnSqr);
  } else if (kingMove) {
    const int diff = move.to - move.from;
    if (std::abs(diff) == 2) {
      const Square rookPos = move.from + ((diff > 0) ? 3 : -4);
      const Square newRookPos = move.to - 1 + (diff < 0) * 2;
      assert(pieceAt(rookPos) == (white ? 'R' : 'r'));
      for (int i = 0; i < m_flags.m_pieceCount; ++i) {
        if (m_pieces[i] == rookPos) {
          board.m_pieces[i] = newRookPos;
          break;
        }
      }
      std::swap(board.m_board[newRookPos], board.m_board[rookPos]);
    }
  }

  if (pawnMove && std::abs(int(move.to) - int(move.from)) == int(2 * width)) {
    board.m_enPassantTarget = move.to
        + (m_flags.m_whiteToMove ? width : -width);
  }

  board.movePiece(move.from, move.to);
  board.m_board[move.to] = movingPiece;
  board.m_board[move.from] = ' ';

  if (pawnMove && (move.to / width == 2 || move.to / width == 9)) {
    assert(move.promotion == 'Q' || move.promotion == 'R'
        || move.promotion == 'B' || move.promotion == 'N');
    char promoted = move.promotion ? move.promotion : 'Q';
    if (!white) {
      promoted = static_cast<char>(std::tolower(promoted));
    }
    board.m_board[move.to] = promoted;
  }

  // [QKqk]: king moves clear both rights; rook moves/captures clear one.
  if (kingMove) {
    board.m_kings[colorIndex(white ? Color::White : Color::Black)] = move.to;
    board.m_flags.m_castlingRights &= white ? 0b0011 : 0b1100;
  }
  if (move.from == 91 || move.to == 91) {
    board.m_flags.m_castlingRights &= 0b0111;
  }
  if (move.from == 98 || move.to == 98) {
    board.m_flags.m_castlingRights &= 0b1011;
  }
  if (move.from == 21 || move.to == 21) {
    board.m_flags.m_castlingRights &= 0b1101;
  }
  if (move.from == 28 || move.to == 28) {
    board.m_flags.m_castlingRights &= 0b1110;
  }

  board.m_flags.m_halfmoveClock = (pawnMove || capture)
      ? 0
      : m_flags.m_halfmoveClock + 1;
  if (!m_flags.m_whiteToMove) {
    ++board.m_flags.m_fullmoveNumber;
  }
  board.m_flags.m_whiteToMove ^= 1;
  return board;
}

bool Board::isAttackedBy(Square sqr, Color attacker) const noexcept {
  const bool byWhite = attacker == Color::White;
  const int pawnStep = byWhite ? width : -width;
  for (int side : {-1, 1}) {
    Square from = static_cast<Square>(sqr + pawnStep + side);
    if (isOnBoard(from) && !isEmpty(from)
        && isWhite(from) == byWhite && isPawn(from)) {
      return true;
    }
  }

  const int knightOffsets[] = {-21, -19, -12, -8, 8, 12, 19, 21};
  for (int offset : knightOffsets) {
    Square from = static_cast<Square>(int(sqr) + offset);
    if (isOnBoard(from) && !isEmpty(from)
        && isWhite(from) == byWhite && isKnight(from)) {
      return true;
    }
  }

  const int directions[] = {-11, -10, -9, -1, 1, 9, 10, 11};
  for (int direction : directions) {
    for (int pos = int(sqr) + direction; pos >= 0 && pos < int(width * height); pos += direction) {
      Square from = static_cast<Square>(pos);
      if (!isOnBoard(from)) {
        break;
      }
      if (isEmpty(from)) {
        continue;
      }
      if (isWhite(from) == byWhite) {
        const bool diagonal = direction != -10 && direction != -1
            && direction != 1 && direction != 10;
        if (isQueen(from) || (diagonal && isBishop(from))
            || (!diagonal && isRook(from))
            || (pos == int(sqr) + direction && isKing(from))) {
          return true;
        }
      }
      break;
    }
  }
  return false;
}

Square Board::kingSquare(Color side) const noexcept {
  const Square king = m_kings[colorIndex(side)];
  assert(isOnBoard(king));
  assert(isKing(king));
  assert(isWhite(king) == (side == Color::White));

  return king;
}

bool Board::isInCheck() const noexcept {
  return isAttackedBy(kingSquare(turn()), enemy());
}

bool Board::canCastle(CastleSide castleSide) const {
  const Color side = turn();
  if (!hasCastleRight(side, castleSide)) {
    return false;
  }

  const Square king = kingSquare(side);
  const Square start = side == Color::White ? 95 : 25;
  const char kingPiece = side == Color::White ? 'K' : 'k';
  const char rookPiece = side == Color::White ? 'R' : 'r';
  const int direction = castleSide == CastleSide::King ? 1 : -1;
  const Square rook = static_cast<Square>(
      king + (castleSide == CastleSide::King ? 3 : -4));
  const Color attacker = enemy();

  if (king != start
      || pieceAt(king) != kingPiece
      || pieceAt(rook) != rookPiece) {
    return false;
  }
  if (!isEmpty(king + direction) || !isEmpty(king + 2 * direction)) {
    return false;
  }
  if (castleSide == CastleSide::Queen && !isEmpty(king - 3)) {
    return false;
  }
  for (int step = 0; step <= 2; ++step) {
    const Square square = static_cast<Square>(king + step * direction);
    if (isAttackedBy(square, attacker)) {
      return false;
    }
  }

  return true;
}

void Board::print() const {
  const char* border = "  +---+---+---+---+---+---+---+---+\n";
  std::cout << "    a   b   c   d   e   f   g   h\n" << border;
  for (unsigned rank = 0; rank < 8; ++rank) {
    std::cout << 8 - rank << " |";
    for (unsigned file = 0; file < 8; ++file) {
      std::cout << ' ' << m_board[offset + rank * width + file] << " |";
    }
    std::cout << ' ' << 8 - rank << '\n' << border;
  }
  std::cout << "    a   b   c   d   e   f   g   h\n";
}

std::size_t Board::parseBoard(std::string_view fen) {
  Logger::debug("Starting piece placement...");
  std::size_t idx = 0;

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
        if (j > 8) {
          throw FenError("Wrong FEN: rank exceeds eight files");
        }
        break;
      case '/':
        if (j != 8 || i >= 7) {
          throw FenError("Wrong FEN: invalid rank width/count");
        }
        ++i;
        j = 0;
        break;
      case ' ':
        if (i != 7 || j != 8) throw FenError("Wrong FEN: board must contain eight complete ranks");
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
        if (i >= 8 || j >= 8) {
          throw FenError("Wrong FEN: too many squares");
        }
        if (m_flags.m_pieceCount >= 64) {
          throw FenError("Wrong FEN: too many pieces");
        }
        if ((val == 'p' || val == 'P') && (i == 0 || i == 7))
          throw FenError("Wrong FEN: pawn on a promotion rank");
        const Square sqr = offset + i * width + j;
        m_board[sqr] = val;
        m_pieces[m_flags.m_pieceCount] = sqr;
        if (val == 'K') {
          m_kings[colorIndex(Color::White)] = sqr;
        } else if (val == 'k') {
          m_kings[colorIndex(Color::Black)] = sqr;
        }
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
        throw FenError(errMsg);
    }
  }

  Logger::debug("Loaded pieces total count: " + std::to_string(m_flags.m_pieceCount));
  if (i != 7 || j != 8) {
    throw FenError(
        "Wrong FEN: board must contain eight complete ranks");
  }
  return idx;
}

void Board::parseTurn(std::size_t& r_i, std::string_view fen) {
  Logger::debug("Loading whose move is it...");
  if (r_i >= fen.size()) {
    throw FenError("Wrong FEN: missing side to move");
  }

  if (fen[r_i] == 'w') {
    m_flags.m_whiteToMove = true;
    Logger::debug("It is white's move");
  } else if (fen[r_i] == 'b') {
    m_flags.m_whiteToMove = false;
    Logger::debug("It is black's move");
  } else {
    throw FenError("Wrong FEN: Who's move");
  }
  ++r_i;
  if (r_i >= fen.size() || fen[r_i] != ' ') {
    throw FenError("Wrong FEN: invalid side field");
  }
}

void Board::parseCastling(std::size_t& r_i, std::string_view fen) {
  Logger::debug("Loading castle status...");
  if (r_i >= fen.size()) {
    throw FenError("Wrong FEN: missing castling field");
  }
  const std::size_t castleStart = r_i;
  while (r_i < fen.size() && fen[r_i] != ' ') {
    if (fen[r_i] != '-' && fen.substr(castleStart, r_i - castleStart).find(fen[r_i]) != std::string_view::npos)
      throw FenError("Wrong FEN: duplicate castling right");
    switch (fen[r_i]) {
    case 'Q':
      m_flags.m_castlingRights |= 0b1000;
      Logger::debug("White can castle queen side");
      break;
    case 'K':
      m_flags.m_castlingRights |= 0b0100;
      Logger::debug("White can castle king side");
      break;
    case 'q':
      m_flags.m_castlingRights |= 0b0010;
      Logger::debug("Black can castle queen side");
      break;
    case 'k':
      m_flags.m_castlingRights |= 0b0001;
      Logger::debug("Black can castle king side");
      break;
    case '-':
      if (r_i != castleStart || (r_i + 1 < fen.size() && fen[r_i + 1] != ' ')) {
        throw FenError("Wrong FEN: '-' must be the entire castling field");
      }
      m_flags.m_castlingRights = 0;
      break;
    default:
      throw FenError("Wrong FEN: Castle");
    }
    ++r_i;
  }
  if (r_i >= fen.size() || r_i == castleStart) {
    throw FenError("Wrong FEN: invalid castling field");
  }
}

void Board::parseEnPassant(std::size_t& r_i, std::string_view fen) {
  Logger::debug("Loading en-passant info...");
  if (r_i >= fen.size()) {
    throw FenError("Wrong FEN: missing en-passant field");
  }
  if (fen[r_i] == '-') {
    Logger::debug("No en-passant available");
    m_enPassantTarget = 0;
    ++r_i;
    if (r_i >= fen.size() || fen[r_i] != ' ') {
      throw FenError("Wrong FEN: invalid en-passant field");
    }
    return;
  }
  if (r_i + 2 >= fen.size() || fen[r_i] < 'a' || fen[r_i] > 'h'
      || (fen[r_i + 1] != '3' && fen[r_i + 1] != '6')
      || fen[r_i + 2] != ' ') {
    throw FenError("Wrong FEN: invalid en-passant square");
  }
  if ((m_flags.m_whiteToMove && fen[r_i + 1] != '6') ||
      (!m_flags.m_whiteToMove && fen[r_i + 1] != '3')) {
    throw FenError("Wrong FEN: en-passant rank disagrees with side to move");
  }
  ++r_i;
  const char enPss[3] = {fen[r_i - 1], fen[r_i], 0};
  m_enPassantTarget = offset + width * 5              // EnPass can only be on 6 or 3!
             + fen[r_i - 1] - 'a'                     // Which file?
             - (width * 3 * m_flags.m_whiteToMove);  // 6th or 3rd line?
  const Square pawnSquare = m_enPassantTarget
      + (m_flags.m_whiteToMove ? width : -width);
  const char expectedPawn = m_flags.m_whiteToMove ? 'p' : 'P';
  if (!isEmpty(m_enPassantTarget) || !isOnBoard(pawnSquare) || pieceAt(pawnSquare) != expectedPawn) {
    throw FenError("Wrong FEN: en-passant pawn is missing");
  }
  Logger::debug("En-passant available on " + std::string(enPss));
  ++r_i;
}

void Board::parseClocks(std::size_t& r_i, std::string_view fen) {
  Logger::debug("Loading half moves...");
  if (r_i >= fen.size()) {
    throw FenError("Wrong FEN: missing halfmove clock");
  }

  const std::size_t halfStart = r_i;
  unsigned long halfmoveClock = 0;
  constexpr unsigned long maxClock =
      std::numeric_limits<unsigned short>::max();
  while (r_i < fen.size() && fen[r_i] != ' ') {
    if (fen[r_i] < '0' || fen[r_i] > '9') {
      throw FenError("Wrong FEN: invalid halfmove clock");
    }

    const unsigned digit = fen[r_i] - '0';
    if (halfmoveClock > (maxClock - digit) / 10) {
      throw FenError("Wrong FEN: halfmove clock is too large");
    }
    halfmoveClock = halfmoveClock * 10 + digit;
    ++r_i;
  }

  if (r_i == halfStart || r_i >= fen.size()) {
    throw FenError("Wrong FEN: invalid halfmove clock");
  }
  m_flags.m_halfmoveClock = halfmoveClock;
  Logger::debug("Half moves: " + std::to_string(m_flags.m_halfmoveClock));

  ++r_i;

  Logger::debug("Loading full moves...");
  const std::size_t fullStart = r_i;
  unsigned long fullmoveNumber = 0;
  while (r_i < fen.size()) {
    if (fen[r_i] < '0' || fen[r_i] > '9') {
      throw FenError("Wrong FEN: invalid fullmove number");
    }

    const unsigned digit = fen[r_i] - '0';
    if (fullmoveNumber > (maxClock - digit) / 10) {
      throw FenError("Wrong FEN: fullmove number is too large");
    }
    fullmoveNumber = fullmoveNumber * 10 + digit;
    ++r_i;
  }

  if (r_i == fullStart || fullmoveNumber == 0) {
    throw FenError("Wrong FEN: invalid fullmove number");
  }
  m_flags.m_fullmoveNumber = fullmoveNumber;
  Logger::debug("Full moves: " + std::to_string(m_flags.m_fullmoveNumber));
}
