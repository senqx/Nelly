#include "pieces.h"

#include <cassert>
#include <vector>

#include "move.h"

std::vector<Move>
Pawn::getValidMoves(const Board* b, const unsigned char pos) noexcept {
  const unsigned char& i = pos / Board::SIZE;
  const unsigned char& j = pos % Board::SIZE;

  // Pawn can't be at first and last line
  // Logger::error("Pawn is on first or last line " + std::to_string(pos) + ' ' + std::to_string(i) + ' ' + std::to_string(j));
  assert(i > 0 && i < Board::SIZE - 1);

  const bool& isWhite = b->getVal(pos) == 'P';
  const char& enemy = 'A' + 32 * isWhite;

  std::vector<Move> moves;
  moves.reserve(4);

  if (isWhite) {
    if (b->getVal(pos - Board::SIZE) == ' ') {
      moves.push_back(Move(pos, pos - Board::SIZE));
      if ((i == Board::SIZE - 2) && (b->getVal(pos - Board::SIZE * 2) == ' ')) {
        moves.push_back(Move(pos, pos - Board::SIZE * 2));
      }
    }
    if (const char& val = b->getVal(i - 1, j + 1);
        val > enemy && val < enemy + ('Z' - 'A'))
    {
      moves.push_back(Move(pos, pos - (Board::SIZE - 1)));
    }
    if (const char& val = b->getVal(i - 1, j - 1);
        val > enemy && val < enemy + ('Z' - 'A'))
    {
      moves.push_back(Move(pos, pos - (Board::SIZE + 1)));
    }
  } else {
    if (b->getVal(pos + Board::SIZE) == ' ') {
      moves.push_back(Move(pos, pos + Board::SIZE));
      if ((i == 1) && (b->getVal(pos + Board::SIZE * 2) == ' ')) {
        moves.push_back(Move(pos, pos + Board::SIZE * 2));
      }
    }
    if (const char& val = b->getVal(i + 1, j + 1);
        val > enemy && val < enemy + ('Z' - 'A'))
    {
      moves.push_back(Move(pos, pos + (Board::SIZE - 1)));
    }
    if (const char& val = b->getVal(i + 1, j - 1);
        val > enemy && val < enemy + ('Z' - 'A'))
    {
      moves.push_back(Move(pos, pos + (Board::SIZE - 1)));
    }
  }
  return moves;
}

std::vector<Move>
Knight::getValidMoves(const Board* b, const unsigned char pos) noexcept {
  const unsigned char& i = pos / Board::SIZE;
  const unsigned char& j = pos % Board::SIZE;
  const bool& isWhite = b->getVal(pos) == 'N';
  const char& enemy = 'A' + 32 * isWhite;

  std::vector<Move> moves;
  moves.reserve(8);

  if (const char& val = b->getVal(i - 2, j - 1);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(pos, pos - (Board::SIZE * 2 + 1)));
  }
  if (const char& val = b->getVal(i - 2, j + 1);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(pos, pos - (Board::SIZE * 2 - 1)));
  }
  if (const char& val = b->getVal(i - 1, j - 2);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(pos, pos - (Board::SIZE + 2)));
  }
  if (const char& val = b->getVal(i - 1, j + 2);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(pos, pos - (Board::SIZE - 2)));
  }
  if (const char& val = b->getVal(i + 2, j - 1);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(pos, pos + (Board::SIZE * 2 - 1)));
  }
  if (const char& val = b->getVal(i + 2, j + 1);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(pos, pos + (Board::SIZE * 2 + 1)));
  }
  if (const char& val = b->getVal(i + 1, j - 2);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(pos, pos + (Board::SIZE - 2)));
  }
  if (const char& val = b->getVal(i + 1, j + 2);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(pos, pos + (Board::SIZE + 2)));
  }
  return moves;
}

std::vector<Move>
Bishop::getValidMoves(const Board* b, const unsigned char pos) noexcept {
  const unsigned char& i = pos / Board::SIZE;
  const unsigned char& j = pos % Board::SIZE;
  // Not == 'B' but < 'a' because Queen uses this method
  const bool& isWhite = b->getVal(pos) < 'a';
  const char& enemy = 'A' + 32 * isWhite;

  std::vector<Move> moves;
  moves.reserve(Board::SIZE * 2);

  for(unsigned char offset = 1; i - offset >= 0; ++offset) {
    const char& val = b->getVal(i - offset, j - offset);
    if (val == ' ') {
      moves.push_back(Move(pos, pos - offset * (Board::SIZE + 1)));
    } else {
      if (val > enemy && val < enemy + ('Z' - 'A')) {
        moves.push_back(Move(pos, pos - offset * (Board::SIZE + 1)));
      }
      break;
    }
  }
  for(unsigned char offset = 1; i -offset >= 0; ++offset) {
    const char& val = b->getVal(i - offset, j + offset);
    if (val == ' ') {
      moves.push_back(Move(pos, pos - offset * (Board::SIZE - 1)));
    } else {
      if (val > enemy && val < enemy + ('Z' - 'A')) {
        moves.push_back(Move(pos, pos - offset * (Board::SIZE - 1)));
      }
      break;
    }
  }
  for(unsigned char offset = 1; i + offset < Board::SIZE; ++offset) {
    const char& val = b->getVal(i + offset, j - offset);
    if (val == ' ') {
      moves.push_back(Move(pos, pos + offset * (Board::SIZE - 1)));
    } else {
      if (val > enemy && val < enemy + ('Z' - 'A')) {
        moves.push_back(Move(pos, pos + offset * (Board::SIZE - 1)));
      }
      break;
    }
  }
  for(unsigned char offset = 1; i + offset < Board::SIZE; ++offset) {
    const char& val = b->getVal(i + offset, j + offset);
    if (val == ' ') {
      moves.push_back(Move(pos, pos + offset * (Board::SIZE + 1)));
    } else {
      if (val > enemy && val < enemy + ('Z' - 'A')) {
        moves.push_back(Move(pos, pos + offset * (Board::SIZE + 1)));
      }
      break;
    }
  }
  return moves;
}

std::vector<Move>
Rook::getValidMoves(const Board* b, const unsigned char pos) noexcept {
  const unsigned char& i = pos / Board::SIZE;
  const unsigned char& j = pos % Board::SIZE;
  // Not == 'R' but < 'a' because Queen uses this method
  const bool& isWhite = b->getVal(pos) < 'a';
  const char enemy = 'A' + 32 * isWhite;

  std::vector<Move> moves;
  moves.reserve(Board::SIZE * 2);

  for(unsigned char offset = 1; i - offset >= 0; ++offset) {
    const char& val = b->getVal(i - offset, j);
    if (val == ' ') {
      moves.push_back(Move(pos, pos - offset * Board::SIZE));
    } else {
      if (val > enemy && val < enemy + ('Z' - 'A')) {
        moves.push_back(Move(pos, pos - offset * Board::SIZE));
      }
      break;
    }
  }
  for(unsigned char offset = 1; i + offset < Board::SIZE; ++offset) {
    const char& val = b->getVal(i + offset, j);
    if (val == ' ') {
      moves.push_back(Move(pos, pos + offset * Board::SIZE));
    } else {
      if (val > enemy && val < enemy + ('Z' - 'A')) {
        moves.push_back(Move(pos, pos + offset * Board::SIZE));
      }
      break;
    }
  }
  for(unsigned char offset = 1; j - offset >= 0; ++offset) {
    const char& val = b->getVal(i, j - offset);
    if (val == ' ') {
      moves.push_back(Move(pos, pos - offset));
    } else {
      if (val > enemy && val < enemy + ('Z' - 'A')) {
        moves.push_back(Move(pos, pos - offset));
      }
      break;
    }
  }
  for(unsigned char offset = 1; j + offset < Board::SIZE; ++offset) {
    const char& val = b->getVal(i, j + offset);
    if (val == ' ') {
      moves.push_back(Move(pos, pos + offset));
    } else {
      if (val > enemy && val < enemy + ('Z' - 'A')) {
        moves.push_back(Move(pos, pos + offset));
      }
      break;
    }
  }
  return moves;
}

std::vector<Move>
Queen::getValidMoves(const Board* b, const unsigned char pos) noexcept {
  std::vector<Move> bishopMoves = Bishop::getValidMoves(b, pos);
  const std::vector<Move>& rookMoves = Rook::getValidMoves(b, pos);

  std::vector<Move>& moves = bishopMoves;
  moves.insert(moves.end(), std::make_move_iterator(rookMoves.begin()),
               std::make_move_iterator(rookMoves.end()));

  return bishopMoves;
}

std::vector<Move>
King::getValidMoves(const Board* b, const unsigned char pos) noexcept {
  const unsigned char& i = pos / Board::SIZE;
  const unsigned char& j = pos % Board::SIZE;
  const bool& isWhite = b->getVal(pos) == 'K';
  const char& enemy = 'A' + 32 * isWhite;

  std::vector<Move> moves;
  moves.reserve(10);

  if (const char& val = b->getVal(i, j - 1);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(pos, pos - 1));
  }
  if (const char& val = b->getVal(i, j + 1);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(pos, pos + 1));
  }
  if (const char& val = b->getVal(i - 1, j - 1);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(pos, pos - (Board::SIZE + 1)));
  }
  if (const char& val = b->getVal(i - 1, j + 1);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(pos, pos - (Board::SIZE - 1)));
  }
  if (const char& val = b->getVal(i + 1, j - 1);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(pos, pos + (Board::SIZE - 1)));
  }
  if (const char& val = b->getVal(i + 1, j + 1);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(pos, pos + (Board::SIZE + 1)));
  }
  if (const char& val = b->getVal(i - 1, j);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(pos, pos - Board::SIZE));
  }
  if (const char& val = b->getVal(i + 1, j);
      val == ' ' || (val > enemy && val < enemy + ('Z' - 'A')))
  {
    moves.push_back(Move(pos, pos + Board::SIZE));
  }

  // Castle moves
  // Castle info is stored i 1 byte as 4 2-bit bools
  // [_Q_K_q_k]
  const unsigned char& castleK = (unsigned char)1 << (isWhite * 4);
  const unsigned char& castleQ = (unsigned char)1 << (isWhite * 6);
  if ((b->getCastlesInfo() & castleK) &&
      (b->getVal(pos + 1) == ' ') &&
      (b->getVal(pos + 2) == ' '))
  {
    moves.push_back(Move(pos, pos + 2));
  }

  if ((b->getCastlesInfo() & castleQ) &&
      (b->getVal(pos - 1) == ' ') &&
      (b->getVal(pos - 2) == ' ') &&
      (b->getVal(pos - 3) == ' '))
  {
    moves.push_back(Move(pos, pos - 2));
  }

  return moves;
}

