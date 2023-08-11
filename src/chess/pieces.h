#ifndef __PIECES__
#define __PIECES__

#include "board.h"

#include <vector>

#include "chess.h"
#include "move.h"

class Pawn {
public:
  static std::list<Move>
  getValidMoves(const Board* b, const BoardSquare s) noexcept;
};

class Knight {
public:
  static std::list<Move>
  getValidMoves(const Board* b, const BoardSquare s) noexcept;
};

class Bishop {
public:
  static std::list<Move>
  getValidMoves(const Board* b, const BoardSquare s) noexcept;
};

class Rook {
public:
  static std::list<Move>
  getValidMoves(const Board* b, const BoardSquare s) noexcept;
};

class Queen {
public:
  static std::list<Move>
  getValidMoves(const Board* b, const BoardSquare s) noexcept;
};

class King {
public:
  static std::list<Move>
  getValidMoves(const Board* b, const BoardSquare s) noexcept;
};

#endif

