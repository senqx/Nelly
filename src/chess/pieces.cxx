#include "pieces.h"

#include <cassert>
#include <vector>

std::vector<Move> Pawn::getValidMoves(
    Board* b, const unsigned char pos) const noexcept
{
  // Pawn can't be at first and last line
  assert((pos < b->SIZE * b->SIZE - 8) && (pos >= 8));

  bool isWhite = b->getVal(pos) == 'P';
  std::vector<Move> moves;
  moves.reserve(4);

  if (isWhite) {
    if (b->getVal(pos - 8) == ' ') {
      moves.push_back(Move(pos, pos - 8));
      if (((pos / 8) == 6) && (b->getVal(pos - 16) == ' ')) {
        moves.push_back(Move(pos, pos - 16));
      }
    }
    if (b->getVal(pos - 7) > 'a') {
      moves.push_back(Move(pos, pos - 7));
    }
    if (b->getVal(pos - 9) > 'a') {
      moves.push_back(Move(pos, pos - 9));
    }
  } else {
    if (b->getVal(pos + 8) == ' ') {
      moves.push_back(Move(pos, pos + 8));
      if (((pos / 8) == 1) && (b->getVal(pos + 16) == ' ')) {
        moves.push_back(Move(pos, pos + 16));
      }
    }
    if (b->getVal(pos + 7) > 'a') {
      moves.push_back(Move(pos, pos + 7));
    }
    if (b->getVal(pos + 9) > 'a') {
      moves.push_back(Move(pos, pos + 9));
    }
  }
  return moves;
}

std::vector<Move> Knight::getValidMoves(
    Board* b, const unsigned char pos) const noexcept
{
  return std::vector<Move>(1);
}

std::vector<Move> Bishop::getValidMoves(
    Board* b, const unsigned char pos) const noexcept
{
  return std::vector<Move>(1);
}

std::vector<Move> Rook::getValidMoves(
    Board* b, const unsigned char pos) const noexcept
{
  return std::vector<Move>(1);
}

std::vector<Move> Queen::getValidMoves(
    Board* b, const unsigned char pos) const noexcept
{
  return std::vector<Move>(1);
}

std::vector<Move> King::getValidMoves(
    Board* b, const unsigned char pos) const noexcept
{
  return std::vector<Move>(1);
}

