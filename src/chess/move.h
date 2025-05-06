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

/*!
 *  @struct Move
 *  @brief A struct to work with moves.
 */
struct Move {
  BoardSquare from; //!< Move from this tile.
  BoardSquare to;   //!< Move to this tile.
  bool isCheck;     //!< Is this move a check?

  /*!
   *  @brief Default contructor.
   *
   *  Creates an empty (impossible) move.
   *  Where: from or to coordinates are not valid.
   */
  Move()
    : from(0)
    , to(0)
    , isCheck(0)
  {}

  /*!
   *  @brief Construct a move manually.
   *
   *  @param from Source square.
   *  @param to Destination square.
   *  @param isCheck Indicates whether the move results in a check (default: false).
   */
  Move(const BoardSquare from, const BoardSquare to, const bool isCheck = 0)
    : from(from)
    , to(to)
    , isCheck(isCheck)
  {}

  /*!
   *  @brief Set isCheck field.
   *
   *  @param val Boolean value to set isCheck to (default: true).
   */
  void setCheck(const bool val = true) noexcept {
    isCheck = val;
  }

  /*!
   *  @brief Convert the move to a string in algebraic notation.
   *
   *  @return A string representation of the move (e.g., e2->e4).
   */
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

