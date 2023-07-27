#ifndef __PIECES__
#define __PIECES__

#include "board.h"

#include <vector>

#include "move.h"

class Pawn {
public:
  static std::vector<Move>
  getValidMoves(const Board* b, const unsigned char pos) noexcept;
};

class Knight {
public:
  static std::vector<Move>
  getValidMoves(const Board* b, const unsigned char pos) noexcept;
};

class Bishop {
public:
  static std::vector<Move>
  getValidMoves(const Board* b, const unsigned char pos) noexcept;
};

class Rook {
public:
  static std::vector<Move>
  getValidMoves(const Board* b, const unsigned char pos) noexcept;
};

class Queen {
public:
  static std::vector<Move>
  getValidMoves(const Board* b, const unsigned char pos) noexcept;
};

class King {
public:
  static std::vector<Move>
  getValidMoves(const Board* b, const unsigned char pos) noexcept;
};

#endif

