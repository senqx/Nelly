#ifndef __NELLY__
#define __NELLY__

#include "chess/Chess.hpp"

class Nelly {
    /// The board to analyse
    Board* _board;
public:
    Nelly();

    /// Load board with given position
    void load_board_from_fen(std::string FEN =
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    /// Return FEN of the current position
    std::string get_fen() const;

    /// Graphical representation of board
    std::string get_board_string() const;

    /// Get all valid moves of current position
    std::vector<Move> get_valid_moves() const;

    /// Make given move (Deletes previous Board and sets current board)
    void move(const Move &m);

    ~Nelly();
};

#endif
