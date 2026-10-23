# Nelly

A C++ chess-engine project in development. The current executable is a
**board and legal-move diagnostic tool**. It loads a FEN position and lists
legal moves; search, evaluation, and the UCI protocol are not implemented yet.

## Build and run

You need a C++17 compiler and GNU Make. Clone with the logger submodule:

```sh
git clone --recurse-submodules https://github.com/senqx/Nelly.git
cd Nelly
make
./bin/Nelly
```

For an existing clone, run `git submodule update --init --recursive` before
building. The logger is pinned to a reviewed revision of
[cpp-logger](https://github.com/senqx/cpp-logger).

By default, Nelly displays the starting position and its 20 legal moves.
Pass a full, quoted FEN to inspect another position:

```sh
./bin/Nelly "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1"
./bin/Nelly --help
```

Moves use diagnostic notation such as `e2->e4`, `e1->g1` for castling, and
`a7->a8=N` for promotion. A trailing `+` indicates check. This output is not
SAN or UCI notation. Invalid FEN input produces an error and a nonzero exit.

## Implementation

The board uses a 10 × 12 mailbox array: the outer squares act as borders around
the playable 8 × 8 board. A piece list and cached king squares support move
generation. Applying a move returns a new board, leaving its parent unchanged.

The core handles ordinary moves and captures, checks and pins, castling,
en passant, and all four promotion choices. Castling rights and en-passant
availability are updated when moves are applied. FEN loading validates the
six fields and several position constraints; it does not prove that a position
is reachable through a legal game.

## Tests

Python 3 is needed for the command-line checks:

```sh
make test
```

The perft tests compare legal move-sequence counts against the
[standard perft suite](https://www.chessprogramming.org/Perft_Results).
Perft tests move generation, not playing strength.

| Position | Depth | Expected nodes |
| --- | ---: | ---: |
| Starting position | 5 | 4,865,609 |
| Kiwipete | 4 | 4,085,603 |
| Rook/pawn endgame | 5 | 674,624 |
| Castling and promotions | 4 | 422,333 |
| Promotion and tactical checks | 4 | 2,103,487 |
| Middlegame | 4 | 3,894,594 |

Additional checks cover promotions, pinned en passant, attacked castling
squares, checkmate, stalemate, malformed FEN, and preservation of a board after
a failed FEN load. GitHub Actions runs the tests normally and with address and
undefined-behavior sanitizers.

## Next steps

- Add search and position evaluation.
- Add game history and draw handling (repetition and move-count rules).
- Implement UCI for use with a chess GUI.

There is currently no computer opponent, interactive game loop, or chess-GUI
integration. `makeMove` expects a move generated for the current board; it is
not a public validator for arbitrary moves. Move counters use 16-bit fields.

Nelly is licensed under [GPL-3.0-or-later](Copying.txt). The logger dependency
has its own MIT license.
