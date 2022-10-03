#ifndef __QUEEN__
#define __QUEEN__

#include "../BoardAndPiece.hpp"

class Queen: public Piece {
public:
    std::vector<Move> get_possible_moves() const override;
};

#endif
