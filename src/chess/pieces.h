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

#ifndef __PIECES__
#define __PIECES__

#include <list>

#include "chess.h"

class Move;
class Board;

/*!
 *  @struct Pawn
 *  @brief Contains logic for Pawn piece moves.
 */
struct Pawn {
  /*!
   *  @brief Get all valid moves for a Pawn.
   *  @param board Current board.
   *  @param sqr Current square of the Pawn.
   *  @return A list of valid moves.
   */
  static std::list<Move>
    getValidMoves(const Board& board, const BoardSquare& sqr) noexcept;
};

/*!
 *  @struct Knight
 *  @brief Contains logic for Knight piece moves.
 */
struct Knight {
  /*!
   *  @brief Get all valid moves for a Knight.
   *  @param board Current board.
   *  @param sqr Current square of the Knight.
   *  @return A list of valid moves.
   */
  static std::list<Move>
    getValidMoves(const Board& board, const BoardSquare& sqr) noexcept;
};

/*!
 *  @struct Bishop
 *  @brief Contains logic for Bishop piece moves.
 */
struct Bishop {
  /*!
   *  @brief Get all valid moves for a Bishop.
   *  @param board Current board.
   *  @param sqr Current square of the Pawn.
   *  @param sqr Current square of the Bishop.
   *  @return A list of valid moves.
   */
  static std::list<Move>
    getValidMoves(const Board& board, const BoardSquare& sqr) noexcept;
};

/*!
 *  @struct Rook
 *  @brief Contains logic for Rook piece moves.
 */
struct Rook {
  /*!
   *  @brief Get all valid moves for a Rook.
   *  @param board Current board.
   *  @param sqr Current square of the Rook.
   *  @return A list of valid moves.
   */
  static std::list<Move>
    getValidMoves(const Board& board, const BoardSquare& sqr) noexcept;
};

/*!
 *  @struct Queen
 *  @brief Contains logic for Queen piece moves.
 */
struct Queen {
  /*!
   *  @brief Get all valid moves for a Queen.
   *  @param board Current board.
   *  @param sqr Current square of the Queen.
   *  @return A list of valid moves.
   */
  static std::list<Move>
    getValidMoves(const Board& board, const BoardSquare& sqr) noexcept;
};

/*!
 *  @struct King
 *  @brief Contains logic for King piece moves.
 */
struct King {
  /*!
   *  @brief Get all valid moves for a King.
   *  @param board Current board.
   *  @param sqr Current square of the King.
   *  @return A list of valid moves.
   */
  static std::list<Move>
    getValidMoves(const Board& board, const BoardSquare& sqr) noexcept;
};

#endif

