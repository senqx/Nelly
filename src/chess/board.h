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

#ifndef __BOARD__
#define __BOARD__

#include <cassert>
#include <exception>
#include <string>
#include <list>

#include "chess.h"

class Move;

/*!
 *  @class FenException
 *  @brief Exception class for handling issues caused by FEN parsing.
 */
class FenException : public std::exception {
  const char* _msg;

public:
  //! Constructs a FEN exception with the given message.
  FenException(const char* msg)
    : _msg(msg)
  {}

  //! Returns the exception message.
  const char* what() const noexcept override { return _msg; }
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
  static constexpr unsigned int HEIGHT = 12;  //!< Extended board's height.
  static constexpr unsigned int WIDTH = 10;   //!< Extended board's width.

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
  char m_board[HEIGHT * WIDTH]; //!< Flat array holding board contents.
  BoardSquare m_pieces[32];     //!< Array of piece positions.
  BoardSquare m_enPass;         //!< En-passant target square.

  struct {
    unsigned m_pieceCount : 6;    //!< Number of active pieces.
    unsigned m_castleInfo : 4;    //!< Castling rights encoded as [QKqk].
    unsigned m_isWhitesMove : 1;  //!< Moving side.
    unsigned m_halfMoves : 7;     //!< Halfmove clock.
    unsigned m_fullMoves : 16;    //!< Fullmove number.
  } m_flags;

public:
  //! Default constructor. Creates an empty board.
  Board();

  //! Default copy constructor.
  Board(const Board& b) = default;

public:
  /*!
   * Loads board state from FEN.
   * If no FEN is given, loads standard chess starting position.
   */
  void loadFen(const std::string& fen =
                   "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR") noexcept;

  //! Returns a new board resulting from applying the given move.
  Board makeMove(const Move& move) const noexcept;

  //! Returns true if white can castle long.
  bool canWhiteLongCastle() const noexcept {
    return m_flags.m_castleInfo & 0b1000;
  }

  //! Returns true if white can castle short.
  bool canWhiteShortCastle() const noexcept {
    return m_flags.m_castleInfo & 0b0100;
  }

  //! Returns true if black can castle long.
  bool canBlackLongCastle() const noexcept {
    return m_flags.m_castleInfo & 0b0010;
  }

  //! Returns true if black can castle short.
  bool canBlackShortCastle() const noexcept {
    return m_flags.m_castleInfo & 0b0001;
  }

  //! Returns true if long castling is available to either side.
  bool isLongCastleAvailable() const noexcept {
    return m_flags.m_castleInfo & 0b1010;
  }

  //! Returns true if short castling is available to either side.
  bool isShortCastleAvailable() const noexcept {
    return m_flags.m_castleInfo & 0b0101;
  }

  //! Checks if the given square is valid (not out-of-bounds).
  bool isValid(const BoardSquare& sqr) const noexcept {
    return m_board[sqr] != '?';
  }

  //! Checks if the square is empty (false if invalid).
  bool isEmpty(const BoardSquare& sqr) const noexcept {
    return m_board[sqr] == ' ';
  }

  //! Returns true if it's white's move.
  bool isWhitesMove() const noexcept {
    return m_flags.m_isWhitesMove;
  }

  //! Returns true if square contains an enemy piece.
  bool isEnemyPiece(const BoardSquare& sqr) const noexcept {
    return !isEmpty(sqr) && (isWhite(sqr) ^ m_flags.m_isWhitesMove);
  }

  //! Returns true if square is the en-passant target.
  bool isEnPass(const BoardSquare& sqr) const noexcept {
    return m_enPass == sqr;
  }

  /*!
   * Returns true if the piece on the square is white.
   * The square must be valid and not empty.
   */
  bool isWhite(const BoardSquare& sqr) const noexcept {
    assert(m_board[sqr] != ' ' || m_board[sqr] != '?');
    return !(m_board[sqr] & 0b00100000);
  }

  //! Returns true if it is a Pawn on the provided square.
  bool isPawn(const BoardSquare& sqr) const noexcept {
    return (m_board[sqr] & 0b11011111) == 'P';
  }

  //! Returns true if it is a Knight on the provided square.
  bool isKnight(const BoardSquare& sqr) const noexcept {
    return (m_board[sqr] & 0b11011111) == 'N';
  }

  //! Returns true if it is a Bishop on the provided square.
  bool isBishop(const BoardSquare& sqr) const noexcept {
    return (m_board[sqr] & 0b11011111) == 'B';
  }

  //! Returns true if it is a Rook on the provided square.
  bool isRook(const BoardSquare& sqr) const noexcept {
    return (m_board[sqr] & 0b11011111) == 'R';
  }

  //! Returns true if it is a Queen on the provided square.
  bool isQueen(const BoardSquare& sqr) const noexcept {
    return (m_board[sqr] & 0b11011111) == 'Q';
  }

  //! Returns true if it is a King on the provided square.
  bool isKing(const BoardSquare& sqr) const noexcept {
    return (m_board[sqr] & 0b11011111) == 'K';
  }

  /*!
   * Returns the character at the given square.
   * ' ' if empty, '?' if invalid.
   */
  char getVal(const BoardSquare sqr) const noexcept {
    assert(sqr < WIDTH * HEIGHT && "Trying to access an out of range square");
    return m_board[sqr];
  }

  //! Returns current en-passant square.
  BoardSquare getEnPass() const noexcept {
    return m_enPass;
  }

  //! Returns all valid moves for the current board.
  std::list<Move> getValidMoves() const noexcept;

  //! Returns all valid moves for the piece at the given square.
  std::list<Move> getValidMoves(const BoardSquare& sqr) const noexcept;

  //! Prints the board to stdout.
  void print() const noexcept;

private:
  //! Removes the piece at the given square from the board.
  void removePiece(const BoardSquare& sqr) noexcept;

  /*!
   * Parses and places pieces from FEN string.
   * Returns the index of the FEN string where the placement ended.
   */
  unsigned int place(const std::string& fen);

  //! Loads side-to-move from FEN string.
  void loadWhoseMove(unsigned int& r_i, const std::string& fen);

  //! Loads castling rights from the FEN string.
  void loadCastles(unsigned int& r_i, const std::string& fen);

  //! Loads en-passant square from the FEN string.
  void loadEnPass(unsigned int& r_i, const std::string& fen);

  //! Loads move counters from the FEN string.
  void loadMoves(unsigned int& r_i, const std::string& fen);
};

#endif

