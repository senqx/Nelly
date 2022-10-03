#ifndef __NELLY__
#define __NELLY__

#include "chess/Chess.hpp"

class Nelly {
    // The board to analyse
    Board* _board;
public:
    Nelly();

    void load_board_from_fen(std::string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    std::string get_fen() const;

    std::string get_board_string() const;

    std::vector<Move> get_valid_moves() const;

    void move(const Move &m);

    ~Nelly();
};

#endif