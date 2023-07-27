#ifndef __PIECES__
#define __PIECES__

#include "board.h"

#include <vector>

//! A struct to work with moves.
struct Move {
  unsigned char from; //!< Move from this tile.
  unsigned char to;   //!< Move to this tile.
  bool isCheck;       //!< Is this move a check?

  //! Default constructor
  /*!
   *  Creates an empty (impossible) move.
   *  Where: from or to coordinates are >=32
   */
  Move()
    : from(-1)
    , to(-1)
    , isCheck(0)
  {}

  Move(unsigned char from, unsigned char to, bool isCheck = 0)
    : from(from)
    , to(to)
    , isCheck(isCheck)
  {}
};

//! An interface for all Pieces
class iPiece {
public:
  //! Every Piece must implement this method (to get its valid moves).
  virtual std::vector<Move>
  getValidMoves(Board* b, const unsigned char pos) const noexcept = 0;
};

class Pawn : public iPiece {
public:
  virtual std::vector<Move>
  getValidMoves(Board* b, const unsigned char pos) const noexcept override;
};

class Knight : public iPiece {
public:
  virtual std::vector<Move>
  getValidMoves(Board* b, const unsigned char pos) const noexcept override;
};

class Bishop : public iPiece {
public:
  virtual std::vector<Move>
  getValidMoves(Board* b, const unsigned char pos) const noexcept override;
};

class Rook : public iPiece {
public:
  virtual std::vector<Move>
  getValidMoves(Board* b, const unsigned char pos) const noexcept override;
};

class Queen : public iPiece {
public:
  virtual std::vector<Move>
  getValidMoves(Board* b, const unsigned char pos) const noexcept override;
};

class King : public iPiece {
public:
  virtual std::vector<Move>
  getValidMoves(Board* b, const unsigned char pos) const noexcept override;
};

#endif

