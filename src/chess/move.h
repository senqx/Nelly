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

#ifndef __CHESS_MOVE__
#define __CHESS_MOVE__

#include "chess.h"

#include <string>

//! A struct to work with moves.
struct Move {
  BoardSquare from; //!< Move from this tile.
  BoardSquare to;   //!< Move to this tile.
  bool isCheck;     //!< Is this move a check?

  //! Default constructor
  /*!
   *  Creates an empty (impossible) move.
   *  Where: from or to coordinates are not valid.
   */
  Move()
    : from(0)
    , to(0)
    , isCheck(0)
  {}

  //! Construct a move manually.
  Move(const BoardSquare from, const BoardSquare to, const bool isCheck = 0)
    : from(from)
    , to(to)
    , isCheck(isCheck)
  {}

  //! Set isCheck field (Sets to true by default).
  void setCheck(const bool val = true) noexcept {
    isCheck = val;
  }

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

