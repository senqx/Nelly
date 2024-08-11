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

struct Pawn {
  static std::list<Move>
    getValidMoves(const Board* p_board, const BoardSquare& sqr) noexcept;
};

struct Knight {
  static std::list<Move>
    getValidMoves(const Board* p_board, const BoardSquare& sqr) noexcept;
};

struct Bishop {
  static std::list<Move>
    getValidMoves(const Board* p_board, const BoardSquare& sqr) noexcept;
};

struct Rook {
  static std::list<Move>
    getValidMoves(const Board* p_board, const BoardSquare& sqr) noexcept;
};

struct Queen {
  static std::list<Move>
    getValidMoves(const Board* p_board, const BoardSquare& sqr) noexcept;
};

struct King {
  static std::list<Move>
    getValidMoves(const Board* p_board, const BoardSquare& sqr) noexcept;
};

#endif

