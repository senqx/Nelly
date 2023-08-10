#include "pieces.h"

#include <list>

#include "../cpp-logger/logger.h"
#include "chess.h"
#include "move.h"

std::list<Move>
Pawn::getValidMoves(const Board* b, const BoardSquare sqr) noexcept {
  Logger::debug("Getting moves for Pawn...");
  const bool& isWhite = b->getVal(sqr) == 'P';
  const char& enemy = 'A' + 32 * isWhite;

  std::list<Move> moves;

  if (isWhite) {
    if (b->getVal(sqr - Board::WIDTH) == ' ') {
      moves.push_back(Move(sqr, sqr - Board::WIDTH));

      if ((sqr / 10 == 8) && (b->getVal(sqr - 2 * Board::WIDTH) == ' ')) {
        moves.push_back(Move(sqr, sqr - 2 * Board::WIDTH));
      }
    }
    if (const char& val = b->getVal(sqr - (Board::WIDTH - 1));
        val > enemy && val < enemy + ('Z' - 'A'))
    {
      moves.push_back(Move(sqr, sqr - (Board::WIDTH - 1)));
    }
    if (const char& val = b->getVal(sqr - (Board::WIDTH + 1));
        val > enemy && val < enemy + ('Z' - 'A'))
    {
      moves.push_back(Move(sqr, sqr - (Board::WIDTH + 1)));
    }
  } else {
    if (b->getVal(sqr + Board::WIDTH) == ' ') {
      moves.push_back(Move(sqr, sqr + Board::WIDTH));
      if ((sqr / 10 == 3) && (b->getVal(sqr + Board::WIDTH * 2) == ' ')) {
        moves.push_back(Move(sqr, sqr + Board::WIDTH * 2));
      }
    }
    if (const char& val = b->getVal(sqr + (Board::WIDTH - 1));
        val > enemy && val < enemy + ('Z' - 'A'))
    {
      moves.push_back(Move(sqr, sqr + (Board::WIDTH - 1)));
    }
    if (const char& val = b->getVal(sqr + (Board::WIDTH + 1));
        val > enemy && val < enemy + ('Z' - 'A'))
    {
      moves.push_back(Move(sqr, sqr + (Board::WIDTH + 1)));
    }
  }
  return moves;
}

std::list<Move>
Knight::getValidMoves(const Board* b, const BoardSquare sqr) noexcept {
  Logger::debug("Getting moves for Knight...");
  const bool& isWhite = b->getVal(sqr) == 'N';
  const char& enemy = 'A' + 32 * isWhite;

  std::list<Move> moves;

  if (const char& val = b->getVal(sqr - (Board::WIDTH * 2 + 1));
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(sqr, sqr - (Board::WIDTH * 2 + 1)));
  }
  if (const char& val = b->getVal(sqr - (Board::WIDTH * 2 - 1));
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(sqr, sqr - (Board::WIDTH * 2 - 1)));
  }
  if (const char& val = b->getVal(sqr - (Board::WIDTH + 2));
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(sqr, sqr - (Board::WIDTH + 2)));
  }
  if (const char& val = b->getVal(sqr - (Board::WIDTH - 2));
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(sqr, sqr - (Board::WIDTH - 2)));
  }
  if (const char& val = b->getVal(sqr + (Board::WIDTH - 2));
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(sqr, sqr + (Board::WIDTH - 2)));
  }
  if (const char& val = b->getVal(sqr + (Board::WIDTH + 2));
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(sqr, sqr + (Board::WIDTH + 2)));
  }
  if (const char& val = b->getVal(sqr + (Board::WIDTH * 2 - 1));
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(sqr, sqr + (Board::WIDTH * 2 - 1)));
  }
  if (const char& val = b->getVal(sqr + (Board::WIDTH * 2 + 1));
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(sqr, sqr + (Board::WIDTH * 2 + 1)));
  }
  return moves;
}

std::list<Move>
Bishop::getValidMoves(const Board* b, const BoardSquare sqr) noexcept {
  Logger::debug("Getting moves for Bishop...");
  // Not == 'B' but < 'a' because Queen also uses this method
  const bool& isWhite = b->getVal(sqr) < 'a';
  const char& enemy = 'A' + 32 * isWhite;

  std::list<Move> moves;
  // Main diagonal
  constexpr unsigned int offsetMainDiag = Board::WIDTH + 1;
  for(unsigned int i = 1; ; ++i) {
    const BoardSquare& target = sqr - i * offsetMainDiag;
    const char& val = b->getVal(target);
    if (val == ' ') {
      moves.push_back(Move(sqr, target));
    } else {
      if (val > enemy && val < enemy + ('Z' - 'A')) {
        moves.push_back(Move(sqr, target));
      }
      break;
    }
  }
  for(unsigned int i = 1; ; ++i) {
    const BoardSquare& target = sqr + i * offsetMainDiag;
    const char& val = b->getVal(target);
    if (val == ' ') {
      moves.push_back(Move(sqr, target));
    } else {
      if (val > enemy && val < enemy + ('Z' - 'A')) {
        moves.push_back(Move(sqr, target));
      }
      break;
    }
  }

  // Secondary diagonal
  constexpr unsigned int offsetScndDiag = Board::WIDTH - 1;
  for(unsigned int i = 1; ; ++i) {
    const BoardSquare& target = sqr - i * offsetScndDiag;
    const char& val = b->getVal(target);
    if (val == ' ') {
      moves.push_back(Move(sqr, target));
    } else {
      if (val > enemy && val < enemy + ('Z' - 'A')) {
        moves.push_back(Move(sqr, target));
      }
      break;
    }
  }
  for(unsigned int i = 1; ; ++i) {
    const BoardSquare& target = sqr + i * offsetScndDiag;
    const char& val = b->getVal(target);
    if (val == ' ') {
      moves.push_back(Move(sqr, target));
    } else {
      if (val > enemy && val < enemy + ('Z' - 'A')) {
        moves.push_back(Move(sqr, target));
      }
      break;
    }
  }
  return moves;
}

std::list<Move>
Rook::getValidMoves(const Board* b, const BoardSquare sqr) noexcept {
  Logger::debug("Getting moves for Rook...");
  // Not == 'R' but < 'a' because Queen uses this method
  const bool& isWhite = b->getVal(sqr) < 'a';
  const char enemy = 'A' + 32 * isWhite;

  std::list<Move> moves;

  // Vertical
  constexpr unsigned int vOffset = Board::WIDTH;
  for(unsigned int i = 0; ; ++i) {
    const BoardSquare& target = sqr - i * vOffset;
    const char& val = b->getVal(target);
    if (val == ' ') {
      moves.push_back(Move(sqr, target));
    } else {
      if (val > enemy && val < enemy + ('Z' - 'A')) {
        moves.push_back(Move(sqr, target));
      }
      break;
    }
  }
  for(unsigned int i = 0; ; ++i) {
    const BoardSquare& target = sqr + i * vOffset;
    const char& val = b->getVal(target);
    if (val == ' ') {
      moves.push_back(Move(sqr, target));
    } else {
      if (val > enemy && val < enemy + ('Z' - 'A')) {
        moves.push_back(Move(sqr, target));
      }
      break;
    }
  }

  // Horizontal
  constexpr unsigned int hOffset = 1;
  for(unsigned int i = 0; ; ++i) {
    const BoardSquare& target = sqr - i * hOffset; // Compiler should ignore *
    const char& val = b->getVal(target);
    if (val == ' ') {
      moves.push_back(Move(sqr, target));
    } else {
      if (val > enemy && val < enemy + ('Z' - 'A')) {
        moves.push_back(Move(sqr, target));
      }
      break;
    }
  }
  for(unsigned int i = 0; ; ++i) {
    const BoardSquare& target = sqr + i * hOffset; // Compiler should ignore *
    const char& val = b->getVal(target);
    if (val == ' ') {
      moves.push_back(Move(sqr, target));
    } else {
      if (val > enemy && val < enemy + ('Z' - 'A')) {
        moves.push_back(Move(sqr, target));
      }
      break;
    }
  }
  return moves;
}

std::list<Move>
Queen::getValidMoves(const Board* b, const BoardSquare sqr) noexcept {
  Logger::debug("Getting moves for Queen...");
  std::list<Move> bishopMoves = Bishop::getValidMoves(b, sqr);
  std::list<Move> rookMoves = Rook::getValidMoves(b, sqr);

  bishopMoves.splice(bishopMoves.end(), rookMoves);
  return bishopMoves;
}

std::list<Move>
King::getValidMoves(const Board* b, const BoardSquare sqr) noexcept {
  Logger::debug("Getting moves for King...");
  const bool& isWhite = b->getVal(sqr) == 'K';
  const char& enemy = 'A' + 32 * isWhite;

  std::list<Move> moves;
  if (const char& val = b->getVal(sqr - (Board::WIDTH + 1));
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(sqr, sqr - (Board::WIDTH + 1)));
  }
  if (const char& val = b->getVal(sqr - Board::WIDTH);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(sqr, sqr - Board::WIDTH));
  }
  if (const char& val = b->getVal(sqr - (Board::WIDTH - 1));
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(sqr, sqr - (Board::WIDTH - 1)));
  }
  if (const char& val = b->getVal(sqr - 1);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(sqr, sqr - 1));
  }
  if (const char& val = b->getVal(sqr + 1);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(sqr, sqr + 1));
  }
  if (const char& val = b->getVal(sqr + (Board::WIDTH - 1));
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(sqr, sqr + (Board::WIDTH - 1)));
  }
  if (const char& val = b->getVal(sqr + Board::WIDTH);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(sqr, sqr + Board::WIDTH));
  }
  if (const char& val = b->getVal(sqr + (Board::WIDTH + 1));
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(sqr, sqr + (Board::WIDTH + 1)));
  }

  // Castle moves
  // Castle info is stored i 1 byte as 4 2-bit bools
  // [_Q_K_q_k]
  const unsigned char& castleK = 1 << (isWhite * 4);
  const unsigned char& castleQ = 1 << (isWhite * 6);
  if ((b->getCastlesInfo() & castleK) &&
      (b->getVal(sqr + 1) == ' ') &&
      (b->getVal(sqr + 2) == ' '))
  {
    moves.push_back(Move(sqr, sqr + 2));
  }

  if ((b->getCastlesInfo() & castleQ) &&
      (b->getVal(sqr - 1) == ' ') &&
      (b->getVal(sqr - 2) == ' ') &&
      (b->getVal(sqr - 3) == ' '))
  {
    moves.push_back(Move(sqr, sqr - 2));
  }

  return moves;
}

