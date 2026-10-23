#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>

#include "chess/board.h"
#include "chess/move.h"

static unsigned long long perft(const Board& board, unsigned depth) {
  if (depth == 0) {
    return 1;
  }
  const std::list<Move> moves = board.legalMoves();
  if (depth == 1) {
    return moves.size();
  }
  unsigned long long nodes = 0;
  for (const Move& move : moves) {
    nodes += perft(board.makeMove(move), depth - 1);
  }

  return nodes;
}

static void expect(
    std::string_view fen, unsigned depth, unsigned long long expected)
{
  Board board;
  board.setFen(fen);
  const unsigned long long actual = perft(board, depth);
  if (actual != expected) {
    std::cerr << "perft depth " << depth << ": expected " << expected
              << ", got " << actual << "\nFEN: " << fen << '\n';
    std::exit(1);
  }
}

static bool hasMove(const Board& board, std::string_view notation) {
  for (const Move& move : board.legalMoves()) {
    if (move.str() == notation) {
      return true;
    }
  }

  return false;
}

int main() {
  const std::string start =
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  expect(start, 1, 20);
  expect(start, 2, 400);
  expect(start, 3, 8902);
  expect(start, 4, 197281);
  expect(start, 5, 4865609);

  // Standard positions from the Chess Programming Wiki perft suite.
  expect("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 4, 4085603);
  expect("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4, 422333);
  expect("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 4, 2103487);
  expect("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 4, 3894594);

  // A standard perft endgame exercises checks, en passant, and rook lines.
  const std::string endgame =
      "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
  expect(endgame, 1, 14);
  expect(endgame, 2, 191);
  expect(endgame, 3, 2812);
  expect(endgame, 4, 43238);
  expect(endgame, 5, 674624);

  // Promotions and en-passant-sensitive positions from the standard perft suite.
  expect("8/P7/8/8/8/8/7p/k6K w - - 0 1", 1, 6);

  Board enPassant;
  enPassant.setFen("7k/8/8/3Pp3/8/8/8/K7 w - e6 0 1");
  if (!hasMove(enPassant, "d5->e6")) {
    return 1;
  }

  Board castles;
  castles.setFen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
  if (!hasMove(castles, "e1->g1") || !hasMove(castles, "e1->c1")) {
    return 1;
  }
  if (castles.turn() != Color::White
      || castles.enemy() != Color::Black
      || !castles.hasCastleRight(Color::Black, CastleSide::Queen)
      || !castles.canCastle(CastleSide::King)) {
    return 1;
  }

  bool rejected = false;
  try {
    Board malformed;
    malformed.setFen("8/8/8/8/8/8/8/9 w - - 0 1");
  } catch (const FenError&) {
    rejected = true;
  }
  if (!rejected) {
    return 1;
  }

  rejected = false;
  try {
    Board malformed;
    malformed.setFen("7k/8/8/8/8/8/8/K7");
  } catch (const FenError&) {
    rejected = true;
  }
  if (!rejected) {
    return 1;
  }

  rejected = false;
  try {
    Board malformed;
    malformed.setFen("7k/8/8/3P4/8/8/8/K7 w - e6 0 1");
  } catch (const FenError&) {
    rejected = true;
  }
  if (!rejected) {
    return 1;
  }

  rejected = false;
  try {
    Board malformed;
    malformed.setFen("7k/8/8/8/8/8/8/K7 w - - 65536 1");
  } catch (const FenError&) {
    rejected = true;
  }
  if (!rejected) {
    return 1;
  }

  // En passant cannot expose the moving king to a rook.
  Board pinnedPawn;
  pinnedPawn.setFen("7k/8/8/r4pPK/8/8/8/8 w - f6 0 1");
  if (hasMove(pinnedPawn, "g5->f6")) return 1;

  // Castling through an attacked transit square is forbidden.
  Board attackedCastle;
  attackedCastle.setFen("k4r2/8/8/8/8/8/8/4K2R w K - 0 1");
  if (hasMove(attackedCastle, "e1->g1")) return 1;

  // Failed FEN loads leave the previous position unchanged.
  Board unchanged;
  unchanged.setFen(start);
  try { unchanged.setFen("7k/K7 w - - 0 1"); return 1; }
  catch (const FenError&) {}
  if (perft(unchanged, 3) != 8902) return 1;

  Board mate;
  mate.setFen("7k/6Q1/5K2/8/8/8/8/8 b - - 0 1");
  if (!mate.isInCheck() || !mate.legalMoves().empty()) return 1;
  Board stalemate;
  stalemate.setFen("7k/5Q2/5K2/8/8/8/8/8 b - - 0 1");
  if (stalemate.isInCheck() || !stalemate.legalMoves().empty()) return 1;

  std::cout << "All perft checks passed\n";
}
