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

#include "pieces.h"

#include <list>

#include "logger.h"
#include "board.h"
#include "chess.h"
#include "move.h"

std::list<Move> Pawn::moves(const Board& board,
                                    Square sqr)
{
  Logger::debug("Getting moves for Pawn...");
  const int forward = board.turn() == Color::White
      ? -Board::width
      : Board::width;

  std::list<Move> moves;
  if (board.isWhite(sqr) == (board.turn() == Color::White)) {
    if (board.isEmpty(sqr + forward)) {
      const Square target = sqr + forward;
      if (target / Board::width == 2 || target / Board::width == 9) {
        for (char piece : {'Q', 'R', 'B', 'N'}) {
          moves.push_back(Move(sqr, target, false, piece));
        }
      } else {
        moves.push_back(Move(sqr, target));
      }

      if ((sqr / 10 == (3 + (board.turn() == Color::White) * 5)) &&
           board.isEmpty(sqr + 2 * forward))
      {
        moves.push_back(Move(sqr, sqr + 2 * forward));
      }
    }
    if (board.isOnBoard(sqr + forward - 1) &&
        (board.isEnemy(sqr + forward - 1) ||
         board.isEnPassantTarget(sqr + forward - 1)))
    {
      const Square target = sqr + forward - 1;
      if (target / Board::width == 2 || target / Board::width == 9) {
        for (char piece : {'Q', 'R', 'B', 'N'}) {
          moves.push_back(Move(sqr, target, false, piece));
        }
      } else {
        moves.push_back(Move(sqr, target));
      }
    }
    if (board.isOnBoard(sqr + forward + 1) &&
        (board.isEnemy(sqr + forward + 1) ||
         board.isEnPassantTarget(sqr + forward + 1)))
    {
      const Square target = sqr + forward + 1;
      if (target / Board::width == 2 || target / Board::width == 9) {
        for (char piece : {'Q', 'R', 'B', 'N'}) {
          moves.push_back(Move(sqr, target, false, piece));
        }
      } else {
        moves.push_back(Move(sqr, target));
      }
    }
  }

  return moves;
}

std::list<Move> Knight::moves(const Board& board,
                                      Square sqr)
{
  Logger::debug("Getting moves for Knight...");
  std::list<Move> moves;

  if (board.isWhite(sqr) == (board.turn() == Color::White)) {
    const Square squares[8] = {
      static_cast<Square>(sqr - (Board::width * 2 + 1)),
      static_cast<Square>(sqr - (Board::width * 2 - 1)),
      static_cast<Square>(sqr - (Board::width - 2)),
      static_cast<Square>(sqr - (Board::width + 2)),
      static_cast<Square>(sqr + (Board::width - 2)),
      static_cast<Square>(sqr + (Board::width + 2)),
      static_cast<Square>(sqr + (Board::width * 2 + 1)),
      static_cast<Square>(sqr + (Board::width * 2 - 1))
    };

    for (Square target : squares) {
      if (board.isOnBoard(target) &&
          (board.isEmpty(target) || board.isEnemy(target)))
      {
        moves.push_back(Move(sqr, target));
      }
    }
  }

  return moves;
}

std::list<Move> Bishop::moves(const Board& board,
                                      Square sqr)
{
  Logger::debug("Getting moves for Bishop...");
  std::list<Move> moves;

  if (board.isWhite(sqr) == (board.turn() == Color::White)) {
    // Main diagonal
    constexpr int offsetMainDiag = Board::width + 1;
    for (int i = 1; ; ++i) {
      const Square target = sqr - i * offsetMainDiag;
      if (!board.isOnBoard(target)) {
        break;
      }
      if (board.isEmpty(target)) {
        moves.push_back(Move(sqr, target));
      } else {
        if (board.isEnemy(target)) {
          moves.push_back(Move(sqr, target));
        }
        break;
      }
    }
    for (int i = 1; ; ++i) {
      const Square target = sqr + i * offsetMainDiag;
      if (!board.isOnBoard(target)) {
        break;
      }
      if (board.isEmpty(target)) {
        moves.push_back(Move(sqr, target));
      } else {
        if (board.isEnemy(target)) {
          moves.push_back(Move(sqr, target));
        }
        break;
      }
    }

    // Secondary diagonal
    constexpr int offsetScndDiag = Board::width - 1;
    for (int i = 1; ; ++i) {
      const Square target = sqr - i * offsetScndDiag;
      if (!board.isOnBoard(target)) {
        break;
      }
      if (board.isEmpty(target)) {
        moves.push_back(Move(sqr, target));
      } else {
        if (board.isEnemy(target)) {
          moves.push_back(Move(sqr, target));
        }
        break;
      }
    }
    for (int i = 1; ; ++i) {
      const Square target = sqr + i * offsetScndDiag;
      if (!board.isOnBoard(target)) {
        break;
      }
      if (board.isEmpty(target)) {
        moves.push_back(Move(sqr, target));
      } else {
        if (board.isEnemy(target)) {
          moves.push_back(Move(sqr, target));
        }
        break;
      }
    }
  }

  return moves;
}

std::list<Move> Rook::moves(const Board& board,
                                    Square sqr)
{
  Logger::debug("Getting moves for Rook...");
  std::list<Move> moves;

  if (board.isWhite(sqr) == (board.turn() == Color::White)) {
    // Vertical
    for (int i = 1; ; ++i) {
      const Square target = sqr - i * Board::width;
      if (!board.isOnBoard(target)) {
        break;
      }
      if (board.isEmpty(target)) {
        moves.push_back(Move(sqr, target));
      } else {
        if (board.isEnemy(target)) {
          moves.push_back(Move(sqr, target));
        }
        break;
      }
    }
    for (int i = 1; ; ++i) {
      const Square target = sqr + i * Board::width;
      if (!board.isOnBoard(target)) {
        break;
      }
      if (board.isEmpty(target)) {
        moves.push_back(Move(sqr, target));
      } else {
        if (board.isEnemy(target)) {
          moves.push_back(Move(sqr, target));
        }
        break;
      }
    }

    // Horizontal
    for (int i = 1; ; ++i) {
      const Square target = sqr - i;
      if (!board.isOnBoard(target)) {
        break;
      }
      if (board.isEmpty(target)) {
        moves.push_back(Move(sqr, target));
      } else {
        if (board.isEnemy(target)) {
          moves.push_back(Move(sqr, target));
        }
        break;
      }
    }
    for (int i = 1; ; ++i) {
      const Square target = sqr + i;
      if (!board.isOnBoard(target)) {
        break;
      }
      if (board.isEmpty(target)) {
        moves.push_back(Move(sqr, target));
      } else {
        if (board.isEnemy(target)) {
          moves.push_back(Move(sqr, target));
        }
        break;
      }
    }
  }

  return moves;
}

std::list<Move> Queen::moves(const Board& board,
                                     Square sqr)
{
  Logger::debug("Getting moves for Queen...");
  std::list<Move> moves = Rook::moves(board, sqr);
  moves.splice(moves.begin(), Bishop::moves(board, sqr));

  return moves;
}

std::list<Move> King::moves(const Board& board,
                                    Square sqr)
{
  Logger::debug("Getting moves for King...");
  std::list<Move> moves;

  if (board.isWhite(sqr) == (board.turn() == Color::White)) {
    const Square squares[8] = {
      static_cast<Square>(sqr - Board::width - 1),
      static_cast<Square>(sqr - Board::width),
      static_cast<Square>(sqr - Board::width + 1),
      static_cast<Square>(sqr - 1),
      static_cast<Square>(sqr + 1),
      static_cast<Square>(sqr + Board::width - 1),
      static_cast<Square>(sqr + Board::width),
      static_cast<Square>(sqr + Board::width + 1),
    };

    for (Square target : squares) {
      if (board.isOnBoard(target) &&
          (board.isEmpty(target) || board.isEnemy(target)))
      {
        moves.push_back(Move(sqr, target));
      }
    }

    const bool white = board.isWhite(sqr);
    const Square expectedKing = white ? 95 : 25;

    if (sqr == expectedKing
        && board.canCastle(CastleSide::King)) {
      moves.push_back(Move(sqr, sqr + 2));
    }

    if (sqr == expectedKing
        && board.canCastle(CastleSide::Queen)) {
      moves.push_back(Move(sqr, sqr - 2));
    }
  }

  return moves;
}
