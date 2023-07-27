#ifndef __CHESS_MOVE__
#define __CHESS_MOVE__

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

  Move(const unsigned char from, const unsigned char to, const bool isCheck = 0)
    : from(from)
    , to(to)
    , isCheck(isCheck)
  {}
};

#endif

