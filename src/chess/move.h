#ifndef __CHESS_MOVE__
#define __CHESS_MOVE__

#include "chess.h"

#include <string>

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

  std::string toString() const noexcept {
    const BoardSquare& from_i = from / 10 - 2;
    const BoardSquare& from_j = from % 10 - 1;
    const BoardSquare& to_i = to / 10 - 2;
    const BoardSquare& to_j = to % 10 - 1;
    std::string str = "a0->a0";
    str[0] += from_j;
    str[1] += 8 - from_i;
    str[4] += to_j;
    str[5] += 8 - to_i;
    return str;
  }
};

#endif

