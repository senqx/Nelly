#ifndef __BOARD__
#define __BOARD__

#include <cassert>
#include <exception>
#include <string>
#include <list>

#include "../cpp-logger/logger.h"
#include "chess.h"
#include "move.h"

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

private:
  char _board[HEIGHT * WIDTH];            //!< Contains the board info.
  std::list<BoardSquare> _piecePositions; //!< The pieces' position array.

  unsigned char _QKqk;       //!< The castle info: 2 bits [QKqk]
                             //!< White: [Q]ueen-side [K]ing-side
                             //!< Black: [q]ueen-side [k]ing-side
  bool _isWhitesMove;        //!< Whose move is it?
  BoardSquare _enPass;       //!< Position of the en-passant.
  unsigned char _halfMoves;  //!< The half moves info.
  unsigned short _fullMoves; //!< The full moves info.

public:
  //! Default constructor. Creates a completely empty board.
  Board();

  //! Updates the board according to FEN.
  void loadFen(const std::string& fen =
                   "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR") noexcept;

  //! Get info for a specific position.
  /*
   *  @returns char representetion of a piece,
   *  @returns ' ' if empty
   *  @returns '?' if oulinie
   */
  char getVal(const BoardSquare sqr) const noexcept {
    assert(sqr < WIDTH * HEIGHT);
    /*
    if (sqr >= WIDTH * HEIGHT) {
      Logger::error("Trying to access a square out of board's range");
      exit(1);
    }
    */
    return _board[sqr];
  }

  //! Get castling info
  unsigned char getCastlesInfo() const noexcept {
    return _QKqk;
  }

  //! Get all valid moves of current position
  std::list<Move> getValidMoves() const noexcept;

  //! Prints the board
  void print() const noexcept;

private:
  //! Place pieces based on FEN
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

