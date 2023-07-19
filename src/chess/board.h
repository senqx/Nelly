#ifndef __BOARD__
#define __BOARD__

#include <exception>
#include <string>

class FenException : public std::exception {
  const char* _msg;

public:
  FenException(const char* msg)
    : _msg(msg)
  {}

  const char* what() const noexcept { return _msg; }
};

//! The chess board
/*!
 *  Must be kept as small as possible,
 *  because we are going to have millions of boards
 *  for each possible move in depth analysis.
 */
class Board {
public:
  static constexpr unsigned int SIZE = 8;

private:
  char _board[SIZE * SIZE];                //!< Contains the board info.
  unsigned char _piecePositions[SIZE * 4]; //!< The pieces' position array.
  unsigned char _pieceCount;               //!< Total piece count on the board.

  unsigned char _QKqk;       //!< The castle info: 2 bits [QKqk]
                             //!< White: [Q]ueen-side [K]ing-side
                             //!< Black: [q]ueen-side [k]ing-side
  bool _isWhitesMove;        //!< Whose move is it?
  unsigned char _enPass;     //!< Position of the en-passant.
  unsigned char _halfMoves;  //!< The half moves info.
  unsigned short _fullMoves; //!< The full moves info.

public:
  //! Default ctor. Creates a completely empty board.
  Board();

  //! Updates the board according to FEN.
  void loadFen(const std::string& fen =
                   "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR") noexcept;

  //! Get info for a specific position.
  char getVal(const unsigned char pos) const noexcept;

  //! Prints the board
  void print() const noexcept;

private:
  //! Place pieces based on FEN
  unsigned char place(const std::string& fen);

  //! Load who's moving from FEN
  void loadWhoseMove(unsigned char& r_i, const std::string& fen);

  //! Load Castles from FEN
  void loadCastles(unsigned char& r_i, const std::string& fen);

  //! Load EnPass from FEN
  void loadEnPass(unsigned char& r_i, const std::string& fen);

  //! Load Moves from FEN
  void loadMoves(unsigned char& r_i, const std::string& fen);
};

#endif

