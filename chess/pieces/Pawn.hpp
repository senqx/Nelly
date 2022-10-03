#ifndef __PAWN__
#define __PAWN__

#include "../BoardAndPiece.hpp"

class Pawn: public Piece {
public:
    std::vector<Move> get_possible_moves() const override;
};

#endif
