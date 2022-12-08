#ifndef __BISHOP__
#define __BISHOP__

#include "../BoardAndPiece.hpp"

class Bishop : public Piece {
public:
    std::list<Move> get_possible_moves() const override;
};

#endif
