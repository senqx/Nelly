#ifndef __KNIGHT__
#define __KNIGHT__

#include "../BoardAndPiece.hpp"

class Knight : public Piece {
    void add_if_valid(std::list<Move> &moves, const Field &f,
        bool color) const;
public:
    std::list<Move> get_possible_moves() const override;
};

#endif
