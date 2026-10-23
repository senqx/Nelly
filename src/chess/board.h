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

#ifndef NELLY_CHESS_BOARD_H
#define NELLY_CHESS_BOARD_H

#include <array>
#include <cassert>
#include <list>
#include <stdexcept>
#include <string>
#include <string_view>

#include "chess.h"

class Move;

/*!
 *  @class FenError
 *  @brief Exception class for handling issues caused by FEN parsing.
 */
class FenError final : public std::invalid_argument {
public:
  using std::invalid_argument::invalid_argument;
};

/*!
 *  @class Board
 *  @brief Represents a chess board using an extended 10x12 array layout.
 *
 *  Stores piece positions and count, castling rights, en-passant square,
 *  moving side and move counters.
 *  Designed to be memory efficient for deep search trees.
 */
class Board {
public:
  static constexpr unsigned int height = 12;  //!< Extended board's height.
  static constexpr unsigned int width = 10;   //!< Extended board's width.

  /*!
   *  @enum Notation
   *  @brief Represents internal piece notation.
   */
  enum class Notation : char
  {
    WhitePawn = 'P',
    WhiteKnight = 'N',
    WhiteBishop = 'B',
    WhiteRook = 'R',
    WhiteQueen = 'Q',
    WhiteKing = 'K',
    BlackPawn = 'p',
    BlackKnight = 'n',
    BlackBishop = 'b',
    BlackRook = 'r',
    BlackQueen = 'q',
    BlackKing = 'k',

    Empty = ' ',
    Invalid = '?'
  };

private:
  char m_board[height * width]; //!< Flat array holding board contents.
  Square m_pieces[64];          //!< Array of piece positions.
  std::array<Square, 2> m_kings; //!< King positions indexed by color.
  Square m_enPassantTarget; //!< En-passant target square.

  struct {
    unsigned m_pieceCount : 7;    //!< Number of active pieces.
    unsigned m_castlingRights : 4; //!< Castling rights encoded as [QKqk].
    unsigned m_whiteToMove : 1;    //!< Moving side.
    unsigned m_halfmoveClock : 16;  //!< Halfmove clock.
    unsigned m_fullmoveNumber : 16; //!< Fullmove number.
  } m_flags;

public:
  static constexpr std::string_view defaultFen =
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

  //! Default constructor. Creates an empty board.
  Board();

  //! Default copy constructor.
  Board(const Board& b) = default;

public:
  /*!
   * Loads board state from FEN.
   * If no FEN is given, loads standard chess starting position.
   */
  void setFen(std::string_view fen = defaultFen);

  //! Returns a new board resulting from applying the given move.
  Board makeMove(const Move& move) const;

  //! Returns true if the side to move is in check.
  bool isInCheck() const noexcept;

  //! Returns whether the side retains the requested castling right.
  bool hasCastleRight(Color side, CastleSide castleSide) const noexcept {
    const unsigned mask = side == Color::White
        ? (castleSide == CastleSide::Queen ? 0b1000 : 0b0100)
        : (castleSide == CastleSide::Queen ? 0b0010 : 0b0001);
    return m_flags.m_castlingRights & mask;
  }

  //! Returns whether the side to move can legally castle as requested.
  bool canCastle(CastleSide castleSide) const;

  //! Checks if the given square is valid (not out-of-bounds).
  bool isOnBoard(Square sqr) const noexcept {
    return sqr < width * height && m_board[sqr] != '?';
  }

  //! Checks if the square is empty (false if invalid).
  bool isEmpty(Square sqr) const noexcept {
    assert(sqr < width * height);
    return m_board[sqr] == ' ';
  }

  //! Returns the side to move.
  Color turn() const noexcept {
    return m_flags.m_whiteToMove ? Color::White : Color::Black;
  }

  //! Returns the opponent of the side to move.
  Color enemy() const noexcept {
    return m_flags.m_whiteToMove ? Color::Black : Color::White;
  }

  //! Returns true if square contains an enemy piece.
  bool isEnemy(Square target) const noexcept {
    assert(isOnBoard(target));
    return !isEmpty(target) &&
           (isWhite(target) != (turn() == Color::White));
  }

  //! Returns true if square is the en-passant target.
  bool isEnPassantTarget(Square target) const noexcept {
    return m_enPassantTarget == target;
  }

  /*!
   * Returns true if the piece on the square is white.
   * The square must be valid and not empty.
   */
  bool isWhite(Square sqr) const noexcept {
    assert(sqr < width * height);
    assert(m_board[sqr] != ' ' && m_board[sqr] != '?');
    return !(m_board[sqr] & 0b00100000);
  }

  //! Returns true if it is a Pawn on the provided square.
  bool isPawn(Square sqr) const noexcept {
    assert(sqr < width * height);
    return (m_board[sqr] & 0b11011111) == 'P';
  }

  //! Returns true if it is a Knight on the provided square.
  bool isKnight(Square sqr) const noexcept {
    assert(sqr < width * height);
    return (m_board[sqr] & 0b11011111) == 'N';
  }

  //! Returns true if it is a Bishop on the provided square.
  bool isBishop(Square sqr) const noexcept {
    assert(sqr < width * height);
    return (m_board[sqr] & 0b11011111) == 'B';
  }

  //! Returns true if it is a Rook on the provided square.
  bool isRook(Square sqr) const noexcept {
    assert(sqr < width * height);
    return (m_board[sqr] & 0b11011111) == 'R';
  }

  //! Returns true if it is a Queen on the provided square.
  bool isQueen(Square sqr) const noexcept {
    assert(sqr < width * height);
    return (m_board[sqr] & 0b11011111) == 'Q';
  }

  //! Returns true if it is a King on the provided square.
  bool isKing(Square sqr) const noexcept {
    assert(sqr < width * height);
    return (m_board[sqr] & 0b11011111) == 'K';
  }

  /*!
   * Returns the character at the given square.
   * ' ' if empty, '?' if invalid.
   */
  char pieceAt(const Square sqr) const noexcept {
    assert(sqr < width * height && "Trying to access an out of range square");
    return m_board[sqr];
  }

  //! Returns current en-passant square.
  Square enPassantTarget() const noexcept {
    return m_enPassantTarget;
  }

  //! Returns all valid moves for the current board.
  std::list<Move> legalMoves() const;

  //! Returns pseudo-legal moves for the piece at the given square.
  std::list<Move> pseudoMoves(Square sqr) const;

  //! Prints the board to stdout.
  void print() const;

private:
  //! Removes the piece at the given square from the board.
  void removePiece(Square sqr) noexcept;

  //! Updates a piece-list entry after a move.
  void movePiece(Square from, Square to) noexcept;

  //! Returns true if the square is attacked by the requested side.
  bool isAttackedBy(Square target, Color attacker) const noexcept;

  //! Returns the requested side's king square.
  Square kingSquare(Color side) const noexcept;

  //! Returns the array index for a color.
  static constexpr std::size_t colorIndex(Color color) noexcept {
    return color == Color::White ? 0 : 1;
  }

  /*!
   * Parses and places pieces from FEN string.
   * Returns the index of the FEN string where the placement ended.
   */
  std::size_t parseBoard(std::string_view fen);

  //! Parses side-to-move from a FEN string.
  void parseTurn(std::size_t& r_i, std::string_view fen);

  //! Parses castling rights from a FEN string.
  void parseCastling(std::size_t& r_i, std::string_view fen);

  //! Parses the en-passant target from a FEN string.
  void parseEnPassant(std::size_t& r_i, std::string_view fen);

  //! Parses move counters from a FEN string.
  void parseClocks(std::size_t& r_i, std::string_view fen);
};

#endif
