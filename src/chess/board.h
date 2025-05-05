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

//! Exception class for handling issues caused by FEN parsing
class FenException : public std::exception {
  const char* _msg;

public:
  FenException(const char* msg)
    : _msg(msg)
  {}

  const char* what() const noexcept override { return _msg; }
};

//! The chess board
/*!
 *  Must be kept as small as possible,
 *  because we are going to have
 *  millions of instances of boards
 *  in depth analysis.
 */
class Board {
public:
  static constexpr unsigned int HEIGHT = 12;
  static constexpr unsigned int WIDTH = 10;

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
  // Trying to keep memory efficiency,
  // As we are going to create millions of Board objects.
  char m_board[HEIGHT * WIDTH]; //!< The board info.
  BoardSquare m_pieces[32];     //!< The pieces' coordnates (0-es in the end).
  unsigned char m_pieceCount;   //!< The count of pieces on the board;

  unsigned char m_castleInfo; //!< The castle info: binary [_Q_K_q_k]
                              // White: [Q]ueen-side [K]ing-side
                              // Black: [q]ueen-side [k]ing-side
  bool m_isWhitesMove;        //!< Whose move is it?
  BoardSquare m_enPass;       //!< Position of the en-passant.
  unsigned char m_halfMoves;  //!< The half moves info.
  unsigned short m_fullMoves; //!< The full moves info.

public:
  //! Default constructor. Creates a completely empty board.
  Board();

  //! Default copy constructor works fine for Boards.
  Board(const Board& b) = default;

public:
  //! Updates the board according to FEN.
  void loadFen(const std::string& fen =
                   "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR") noexcept;

  //! Make a move.
  Board makeMove(const Move& move) const noexcept;

  //! Get castling info
  bool canWhiteLongCastle() const noexcept {
    return m_castleInfo & 0b01000000;
  }

  bool canWhiteShortCastle() const noexcept {
    return m_castleInfo & 0b00010000;
  }

  bool canBlackLongCastle() const noexcept {
    return m_castleInfo & 0b00000100;
  }

  bool canBlackShortCastle() const noexcept {
    return m_castleInfo & 0b00000001;
  }

  //! Is the provided square in [a1 to h8] range?
  bool isValid(const BoardSquare& sqr) const noexcept {
    return m_board[sqr] != '?';
  }

  bool isShortCastleAvailable() const noexcept {
    return m_castleInfo & 0b00010001;
  }

  bool isLongCastleAvailable() const noexcept {
    return m_castleInfo & 0b01000100;
  }
  //! Is the provided square empty? Returns false for invalid square.
  bool isEmpty(const BoardSquare& sqr) const noexcept {
    return m_board[sqr] == ' ';
  }

  bool isWhitesMove() const noexcept {
    return m_isWhitesMove;
  }

  bool isEnemyPiece(const BoardSquare& sqr) const noexcept {
    return !isEmpty(sqr) && (isWhite(sqr) ^ m_isWhitesMove);
  }

  bool isEnPass(const BoardSquare& sqr) const noexcept {
    return m_enPass == sqr;
  }

  //! @returns true if the piece on provided square is white.
  /*!
   *  Note: the provided square must be valid and non empty.
   */
  bool isWhite(const BoardSquare& sqr) const noexcept {
    assert(m_board[sqr] != ' ' || m_board[sqr] != '?');
    return !(m_board[sqr] & 0b00100000);
  }

  //! Checks for specific piece on the provided square.
  bool isPawn(const BoardSquare& sqr) const noexcept {
    return (m_board[sqr] & 0b11011111) == 'P';
  }

  bool isKnight(const BoardSquare& sqr) const noexcept {
    return (m_board[sqr] & 0b11011111) == 'N';
  }

  bool isBishop(const BoardSquare& sqr) const noexcept {
    return (m_board[sqr] & 0b11011111) == 'B';
  }

  bool isRook(const BoardSquare& sqr) const noexcept {
    return (m_board[sqr] & 0b11011111) == 'R';
  }

  bool isQueen(const BoardSquare& sqr) const noexcept {
    return (m_board[sqr] & 0b11011111) == 'Q';
  }

  bool isKing(const BoardSquare& sqr) const noexcept {
    return (m_board[sqr] & 0b11011111) == 'K';
  }

  //! Get info for a specific position.
  /*
   *  @returns char representetion of a piece,
   *  @returns ' ' if empty
   *  @returns '?' if oulinie
   */
  char getVal(const BoardSquare sqr) const noexcept {
    assert(sqr < WIDTH * HEIGHT && "Trying to access an out of range square");
    return m_board[sqr];
  }

  //! Get En passant square.
  BoardSquare getEnPass() const noexcept {
    return m_enPass;
  }

  //! Get all valid moves of current position.
  std::list<Move> getValidMoves() const noexcept;

  //! Get all valid moves of the provided piece.
  std::list<Move> getValidMoves(const BoardSquare& sqr) const noexcept;

  //! Print the board.
  void print() const noexcept;

private:
  //! Remove a piece from the board.
  void removePiece(const BoardSquare& sqr) noexcept;

  //! Place pieces based on FEN.
  /*!
   *  Parses only placement part of the FEN.
   *  @returns the index of the FEN string where it stopped.
   */
  unsigned int place(const std::string& fen);

  //! Load who's moving from FEN
  void loadWhoseMove(unsigned int& r_i, const std::string& fen);

  //! Load Castles from FEN
  void loadCastles(unsigned int& r_i, const std::string& fen);

  //! Load EnPass from FEN
  void loadEnPass(unsigned int& r_i, const std::string& fen);

  //! Load Moves from FEN
  void loadMoves(unsigned int& r_i, const std::string& fen);
};

#endif

