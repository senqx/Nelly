#ifndef __ROOK__
#define __ROOK__

#include "../BoardAndPiece.hpp"

class Rook : public Piece {
public:
    std::list<Move> get_possible_moves() const override;
};

#endif
