#ifndef __CHESS_MOVE__
#define __CHESS_MOVE__

#include "chess.h"

//! A struct to work with moves.
struct Move {
  BoardSquare from; //!< Move from this tile.
  BoardSquare to;   //!< Move to this tile.
  bool isCheck;    //!< Is this move a check?

  //! Default constructor
  /*!
   *  Creates an empty (impossible) move.
   *  Where: from or to coordinates are not valid
   */
  Move()
    : from(0)
    , to(0)
    , isCheck(0)
  {}

  Move(const BoardSquare from, const BoardSquare to, const bool isCheck = 0)
    : from(from)
    , to(to)
    , isCheck(isCheck)
  {}
};

#endif

