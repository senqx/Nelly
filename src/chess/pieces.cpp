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

#include "../cpp-logger/logger.h"
#include "board.h"
#include "chess.h"
#include "move.h"

std::list<Move> Pawn::getValidMoves(const Board& board,
                                    const BoardSquare& sqr) noexcept
{
  Logger::debug("Getting moves for Pawn...");
  const int forward = board.isWhitesMove()? -Board::WIDTH : Board::WIDTH;

  std::list<Move> moves;
  if (!board.isWhite(sqr) ^ board.isWhitesMove()) {
    if (board.isEmpty(sqr + forward)) {
      moves.push_back(Move(sqr, sqr + forward));

      if ((sqr / 10 == (3 + board.isWhitesMove() * 5)) &&
           board.isEmpty(sqr + 2 * forward))
      {
        moves.push_back(Move(sqr, sqr + 2 * forward));
      }
    }
    if (board.isValid(sqr + forward - 1) &&
        (board.isEnemyPiece(sqr + forward - 1) ||
         board.isEnPass(sqr + forward - 1)))
    {
      moves.push_back(Move(sqr, sqr + forward - 1));
    }
    if (board.isValid(sqr + forward + 1) &&
        (board.isEnemyPiece(sqr + forward + 1) ||
         board.isEnPass(sqr + forward + 1)))
    {
      moves.push_back(Move(sqr, sqr + forward + 1));
    }
  }

  return moves;
}

std::list<Move> Knight::getValidMoves(const Board& board,
                                      const BoardSquare& sqr) noexcept
{
  Logger::debug("Getting moves for Knight...");
  std::list<Move> moves;

  if (!board.isWhite(sqr) ^ board.isWhitesMove()) {
    const BoardSquare squares[8] = {
      static_cast<BoardSquare>(sqr - (Board::WIDTH * 2 + 1)),
      static_cast<BoardSquare>(sqr - (Board::WIDTH * 2 - 1)),
      static_cast<BoardSquare>(sqr - (Board::WIDTH - 2)),
      static_cast<BoardSquare>(sqr - (Board::WIDTH + 2)),
      static_cast<BoardSquare>(sqr + (Board::WIDTH - 2)),
      static_cast<BoardSquare>(sqr + (Board::WIDTH + 2)),
      static_cast<BoardSquare>(sqr + (Board::WIDTH * 2 + 1)),
      static_cast<BoardSquare>(sqr + (Board::WIDTH * 2 - 1))
    };

    for (const BoardSquare& target : squares) {
      if (board.isValid(target) &&
          (board.isEmpty(target) || board.isEnemyPiece(target)))
      {
        moves.push_back(Move(sqr, target));
      }
    }
  }

  return moves;
}

std::list<Move> Bishop::getValidMoves(const Board& board,
                                      const BoardSquare& sqr) noexcept
{
  Logger::debug("Getting moves for Bishop...");
  std::list<Move> moves;

  if (!board.isWhite(sqr) ^ board.isWhitesMove()) {
    // Main diagonal
    constexpr int offsetMainDiag = Board::WIDTH + 1;
    for (int i = 1; ; ++i) {
      const BoardSquare target = sqr - i * offsetMainDiag;
      if (!board.isValid(target)) {
        break;
      }
      if (board.isEmpty(target)) {
        moves.push_back(Move(sqr, target));
      } else {
        if (board.isEnemyPiece(target)) {
          moves.push_back(Move(sqr, target));
        }
        break;
      }
    }
    for (int i = 1; ; ++i) {
      const BoardSquare target = sqr + i * offsetMainDiag;
      if (!board.isValid(target)) {
        break;
      }
      if (board.isEmpty(target)) {
        moves.push_back(Move(sqr, target));
      } else {
        if (board.isEnemyPiece(target)) {
          moves.push_back(Move(sqr, target));
        }
        break;
      }
    }

    // Secondary diagonal
    constexpr int offsetScndDiag = Board::WIDTH - 1;
    for (int i = 1; ; ++i) {
      const BoardSquare target = sqr - i * offsetScndDiag;
      if (!board.isValid(target)) {
        break;
      }
      if (board.isEmpty(target)) {
        moves.push_back(Move(sqr, target));
      } else {
        if (board.isEnemyPiece(target)) {
          moves.push_back(Move(sqr, target));
        }
        break;
      }
    }
    for (int i = 1; ; ++i) {
      const BoardSquare target = sqr + i * offsetScndDiag;
      if (!board.isValid(target)) {
        break;
      }
      if (board.isEmpty(target)) {
        moves.push_back(Move(sqr, target));
      } else {
        if (board.isEnemyPiece(target)) {
          moves.push_back(Move(sqr, target));
        }
        break;
      }
    }
  }

  return moves;
}

std::list<Move> Rook::getValidMoves(const Board& board,
                                    const BoardSquare& sqr) noexcept
{
  Logger::debug("Getting moves for Rook...");
  std::list<Move> moves;

  if (!board.isWhite(sqr) ^ board.isWhitesMove()) {
    // Vertical
    for (int i = 1; ; ++i) {
      const BoardSquare target = sqr - i * Board::WIDTH;
      if (!board.isValid(target)) {
        break;
      }
      if (board.isEmpty(target)) {
        moves.push_back(Move(sqr, target));
      } else {
        if (board.isEnemyPiece(target)) {
          moves.push_back(Move(sqr, target));
        }
        break;
      }
    }
    for (int i = 1; ; ++i) {
      const BoardSquare target = sqr + i * Board::WIDTH;
      if (!board.isValid(target)) {
        break;
      }
      if (board.isEmpty(target)) {
        moves.push_back(Move(sqr, target));
      } else {
        if (board.isEnemyPiece(target)) {
          moves.push_back(Move(sqr, target));
        }
        break;
      }
    }

    // Horizontal
    for (int i = 1; ; ++i) {
      const BoardSquare target = sqr - i;
      if (!board.isValid(target)) {
        break;
      }
      if (board.isEmpty(target)) {
        moves.push_back(Move(sqr, target));
      } else {
        if (board.isEnemyPiece(target)) {
          moves.push_back(Move(sqr, target));
        }
        break;
      }
    }
    for (int i = 1; ; ++i) {
      const BoardSquare target = sqr + i;
      if (!board.isValid(target)) {
        break;
      }
      if (board.isEmpty(target)) {
        moves.push_back(Move(sqr, target));
      } else {
        if (board.isEnemyPiece(target)) {
          moves.push_back(Move(sqr, target));
        }
        break;
      }
    }
  }

  return moves;
}

std::list<Move> Queen::getValidMoves(const Board& board,
                                     const BoardSquare& sqr) noexcept
{
  Logger::debug("Getting moves for Queen...");
  std::list<Move> moves = Rook::getValidMoves(board, sqr);
  moves.splice(moves.begin(), Bishop::getValidMoves(board, sqr));

  return moves;
}

std::list<Move> King::getValidMoves(const Board& board,
                                    const BoardSquare& sqr) noexcept
{
  Logger::debug("Getting moves for King...");
  std::list<Move> moves;

  if (!board.isWhite(sqr) ^ board.isWhitesMove()) {
    const BoardSquare squares[8] = {
      static_cast<BoardSquare>(sqr - Board::WIDTH - 1),
      static_cast<BoardSquare>(sqr - Board::WIDTH),
      static_cast<BoardSquare>(sqr - Board::WIDTH + 1),
      static_cast<BoardSquare>(sqr - 1),
      static_cast<BoardSquare>(sqr + 1),
      static_cast<BoardSquare>(sqr + Board::WIDTH - 1),
      static_cast<BoardSquare>(sqr + Board::WIDTH),
      static_cast<BoardSquare>(sqr + Board::WIDTH + 1),
    };

    for (const BoardSquare& target : squares) {
      if (board.isValid(target) &&
          (board.isEmpty(target) || board.isEnemyPiece(target)))
      {
        moves.push_back(Move(sqr, target));
      }
    }

    if (board.isShortCastleAvailable() &&
        board.isEmpty(sqr + 1) && // Is also not under attack.
        board.isEmpty(sqr + 2))
    {
      moves.push_back(Move(sqr, sqr + 2));
    }

    if (board.isLongCastleAvailable() &&
        board.isEmpty(sqr - 1) && // Is also not under attack.
        board.isEmpty(sqr - 2) &&
        board.isEmpty(sqr - 3))
    {
      moves.push_back(Move(sqr, sqr - 2));
    }
  }

  return moves;
}

